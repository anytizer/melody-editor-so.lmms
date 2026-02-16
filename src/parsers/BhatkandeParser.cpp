/**
 * BhatkandeParser.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include "BhatkandeParser.h"

#include "AutomationClip.h"
#include "DetuningHelper.h"
#include "MidiClip.h"


namespace
{
static constexpr double DEFAULT_STEP = 0.25; // one quater note
}


namespace lmms::melodyeditor
{


// --------------- LEXING ---------------



char BhatkandeLexer::read()
{
	char c = peek();

	if (c == '\n')
	{
		m_line++;
		m_column = 0;
	}
	else
	{
		m_column++;
	}

	m_index++;

	return c;
}




void BhatkandeLexer::flushBuffer()
{
	static const std::vector<std::pair<std::string, Token::Type>> keywords = {
		{"key", Token::Type::_key},
        {"rest", Token::Type::_rest},
		{"transpose", Token::Type::_transpose},
	};

	if (m_buffer.empty()) { return; }

	// Remeber the length of the original string so it can be recalled for error messages etc
	m_bufferStartPos.length = m_buffer.size();

	// Clear buffer before calling addToken() to avoid infinite recursion
	std::string string;
	std::swap(m_buffer, string);

	// Is it a number?
	if ('0' <= string[0] && string[0] <= '9')
	{
		Token token = {Token::Type::Number, m_bufferStartPos};
		try
		{
			token.integerValue = stoi(string);
		}
		catch (std::invalid_argument& e)
		{
			throw ParserError("invalid number", m_bufferStartPos);
		}
		catch (const std::out_of_range& e) {
			throw ParserError("number out of range", m_bufferStartPos);
		}
		addToken(token);
		return;
	}

	// Is it a keyword?
	for (const auto& [keyword, type]: keywords)
	{
		if (string == keyword)
		{
			addToken({type, m_bufferStartPos});
			return;
		}
	}

	// Everything else is a variable
	addToken({Token::Type::Name, m_bufferStartPos, 0, string});
}




void BhatkandeLexer::addToken(const Token& token)
{
	if (!m_buffer.empty()) { flushBuffer(); }

	m_tokens.push_back(token);
}




void BhatkandeLexer::handleHash()
{
	if (m_buffer.empty())
	{
		while (!eof() && read() != '\n') {}
	}
	else
	{
		m_buffer += '#';
	}
}




void BhatkandeLexer::handleWhitespace()
{
	flushBuffer();
	if (!m_tokens.empty() && m_tokens.back().type != Token::Type::Whitespace)
	{
		addToken({Token::Type::Whitespace});
	}
}





std::vector<BhatkandeLexer::Token> BhatkandeLexer::scan(const std::string& string)
{
	m_string = string;

	while (!eof())
	{
		SourcePos pos = sourcePos();
		char c = read();

		switch (c)
		{
		case '{': addToken({Token::Type::CurlyLeft, pos}); break;
		case '}': addToken({Token::Type::CurlyRight, pos}); break;
		case '-': addToken({Token::Type::Dash, pos}); break;
        case '.': addToken({Token::Type::Dot, pos}); break;
		case '=': addToken({Token::Type::Equal, pos}); break;
		case '\n': addToken({Token::Type::Newline, pos}); break;
		case '(': addToken({Token::Type::ParenLeft, pos}); break;
		case ')': addToken({Token::Type::ParenRight, pos}); break;
        case '\'': addToken({Token::Type::SingleQuote, pos}); break;
        case '[': addToken({Token::Type::SquareLeft, pos}); break;
		case ']': addToken({Token::Type::SquareRight, pos}); break;
        case '*': addToken({Token::Type::Star, pos}); break;

		case '#':
			handleHash();
			break;
		case ' ': case '\r': case '\t':
			handleWhitespace();
			break;
		default:
			if (m_buffer.empty()) { m_bufferStartPos = pos; }
			m_buffer += c;
		}
	}

	addToken({Token::Type::EndOfFile, sourcePos()});

	return m_tokens;
}



// --------------- PARSE HELPERS ---------------



bool BhatkandeParserPrivate::isFollowedByEqualSign()
{
	for (size_t i = 0; m_index + i < m_tokens.size(); i++)
	{
		switch (m_tokens[m_index + i].type)
		{
		case Token::Type::Whitespace: continue;
		case Token::Type::Equal: return true;
		default: return false;
		}
	}
	return false;
}



int BhatkandeParserPrivate::readNumber()
{
	skipWhitespace();

	auto token = read();
	if (token.type != Token::Type::Number)
	{
		throw ParserError("expected a number", token.pos);
	}
	return token.integerValue;
}


void BhatkandeParserPrivate::skipWhitespace()
{
	while (!eof() && peek().type == Token::Type::Whitespace)
	{
		read();
	}
}


BhatkandeParserPrivate::Macro& BhatkandeParserPrivate::getMacro(const std::string& name)
{
	// Find an existing macro with the same name
	auto it = std::find_if(m_macros.begin(), m_macros.end(), [&](Macro& m){
		return m.name == name;
	});

	if (it != m_macros.end())
	{
		return *it;
	}

	// Insert a new macro is there was no match
	// m_macros must be sorted longest name to shortest name
	// Find the first name that is shorter and insert a new macro before it
	it = std::find_if(m_macros.begin(), m_macros.end(), [&](Macro& m){
		return m.name.size() < name.size();
	});

	return *m_macros.emplace(it, name);

}











// --------------- PARSING ---------------




void BhatkandeParserPrivate::parseContainer(Node& container, Token::Type endType)
{
    // By default child nodes are added directly into the container,
    // but if they are written without whitespace inbetween we "concatenate" them
    // by creating an implicit Tuple node and adding them to that instead.
    //
    // This is how we expect implicit tuples to work:
    // Sa Re          => Sa, Re
    // SaRe           => tuple(Sa, Re)
    // [Sa][Re]       => tuple(glide(Sa), glide(Re))
    // [Sa Re]SaRe    => tuple(glide(Sa, Re), Sa, Re)
    // [SaRe] Sa Re   => glide(tuple(Sa, Re)), Sa, Re
    bool concatenate = false;
    bool lastWasConcatenated = false;

	// Lambda to add a new child to the container or tuple
	auto addChild = [&](const Node& child) -> Node&
	{
        // If we want to concatenate, but the last item wasn't a Tuple
        // create an empty Tuple, move the last item into it
        // and replace the last item in the container with the Tuple
        if (concatenate && !lastWasConcatenated)
        {
            Node tuple = {Node::Type::Tuple};
            Node& previousNode = container.children.back();
            tuple.children.push_back(std::move(previousNode));
            std::swap(previousNode, tuple);
        }
        Node& dest = concatenate ? container.children.back() : container;

        // Concatenate next time
        lastWasConcatenated = concatenate;
        concatenate = true;

        // Copy the child into the container
        return dest.children.emplace_back(child);


	};

    auto newChild = [&](Node::Type type) -> Node&
    {
        return addChild(Node{type});
    };


	while (!eof())
	{
		Token token = read();

		if (token.type == endType) { return; }

		switch (token.type)
		{
		case Token::Type::CurlyLeft:
            parseContainer(newChild(Node::Type::Group), Token::Type::CurlyRight);
			break;

		case Token::Type::Dash:
            newChild(Node::Type::Extend);
			break;

        case Token::Type::Dot:
        case Token::Type::SingleQuote:
        {
            if (container.children.empty())
            {
                throw ParserError("unexpected " + token.displayName(), token.pos);
            }
            int direction = token.type == Token::Type::Dot ? -1 : 1;
            Node& last = lastWasConcatenated ? container.children.back().children.back() : container.children.back();
            last.value += KeysPerOctave * direction;
            break;
        }

		case Token::Type::EndOfFile:
			// Treat the container as closed when we reach EOF, even if closing brackets are missing
			return;

		case Token::Type::Name:
			if (token.stringValue.empty())
			{
				throw ParserError("empty name (should not happen)", token.pos);
			}
			if (isFollowedByEqualSign())
			{
				parseDefinition(token);
			}
			else
			{
                for (const Node* expanded : parseName(token))
                {
                    addChild(*expanded);
                }
			}
			break;

		case Token::Type::Newline:
			// Newline is used as the endToken in definitions, otherwise ignored
            concatenate = false;
			break;

		case Token::Type::ParenLeft:
            parseContainer(newChild(Node::Type::Tuple), Token::Type::ParenRight);
			break;

        case Token::Type::Star:
            if (container.children.empty())
            {
                throw ParserError("unexpected " + token.displayName(), token.pos);
            }
            container.children.back().repeat *= readNumber();
            break;

		case Token::Type::SquareLeft:
            parseContainer(newChild(Node::Type::Glide), Token::Type::SquareRight);
			break;

		case Token::Type::Whitespace:
            concatenate = false;
			break;

        case Token::Type::_key:
            newChild(Node::Type::Note).value = readNumber();
            break;

		case Token::Type::_rest:
			newChild(Node::Type::Rest);
			break;

		case Token::Type::_transpose:
			newChild(Node::Type::Transpose).value = readNumber(); // @todo support negative numbers
            break;

		default:
			throw ParserError("unexpected " + token.displayName(), token.pos);
		}
	}
}




void BhatkandeParserPrivate::parseDefinition(const Token& nameToken)
{
	skipWhitespace();

	auto equalToken = read();
	if (equalToken.type != Token::Type::Equal)
	{
		throw ParserError("missing = in definition (should not happen)", equalToken.pos);
	}

	skipWhitespace();

	Macro& macro = getMacro(nameToken.stringValue);
	macro.value = Node{Node::Type::Group};
	parseContainer(macro.value, Token::Type::Newline);
}





std::vector<const BhatkandeParserPrivate::Node*> BhatkandeParserPrivate::parseName(const Token& token)
{
    std::string name = token.stringValue;
    std::vector<const Node*> result;

	// Lambda that checks if the current name starts with the name of a given macro
	auto startsWithMacro = [&](Macro& m) { return name.starts_with(m.name); };

	while (!name.empty())
	{
		// Find the a macro that matches the start of the name
		// (m_macros is sorted by longest first, so it matches as much as possible)
		auto macro = std::find_if(m_macros.begin(), m_macros.end(), startsWithMacro);
		if (macro == m_macros.end())
		{
			throw ParserError("undefined name: " + name, token.pos);
		}

		// Assert that macro name is non-zero to avoid infinite loop
		if (macro->name.empty())
		{
			throw ParserError("there has been a macro defined without a name (should not happen)", token.pos);
		}
		// Remove the leftmost part of the name (the part the matched the macro)
		else if (macro->name.size() < name.size())
		{
			name = name.substr(macro->name.size());
		}
		// Or remove the entire name if all of it matched
		else
		{
			name.clear();
		}

		// Expand the macro
		result.push_back(&macro->value);
	}

    return result;
}


BhatkandeParserPrivate::Node BhatkandeParserPrivate::parse(const std::vector<Token>& tokens)
{
    m_tokens = tokens;
    m_index = 0;
    m_macros.clear();

    auto root = Node{Node::Type::Group};
    parseContainer(root, Token::Type::EndOfFile);

    return root;
}



// --------------- EVALUATION ---------------


std::vector<Note> BhatkandeEvaluator::evaluate(const Node& node)
{
    std::vector<Note> result;
    evalNode(node, 0, DEFAULT_STEP, result);
    return result;
}


int BhatkandeEvaluator::countSteps(const Node& node)
{
    switch (node.type)
    {
    case Node::Type::Extend:
    case Node::Type::Note:
    case Node::Type::Rest:
    case Node::Type::Tuple:
        return 1;
    case Node::Type::Glide:
    case Node::Type::Group:
    {
        int result = 0;
        for (const Node& child: node.children)
        {
            result += countSteps(child);
        }
        return result;
    }
    default:
        return 0;
    }
}





double BhatkandeEvaluator::evalGlide(const Node& glide, double pos, double step, std::vector<Note>& result)
{
    std::vector<Note> tmpNotes;
    double glideDuration = evalGroup(glide, pos, step, tmpNotes);

    if (tmpNotes.empty()) { return 0; }

    Note& firstNote = tmpNotes.front();

    firstNote.createDetuning();
    auto detuning = firstNote.detuning();
    if (!detuning) { return 0; }

    AutomationClip* autoClip = detuning->automationClip();
    if (!autoClip) { return 0; }

    autoClip->clear();
    for (const Note& note: tmpNotes)
    {
        autoClip->putValue(note.pos() - firstNote.pos(), note.key() - firstNote.key(), /*quantize*/ false);
        autoClip->putValue(note.pos() - firstNote.pos() + note.length() / 2, note.key() - firstNote.key(), /*quantize*/ false);
    }

    firstNote.setLength(tmpNotes.back().endPos()- firstNote.pos());
    result.push_back(std::move(firstNote));

    return glideDuration;
}

double BhatkandeEvaluator::evalGroup(const Node& group, double pos, double step, std::vector<Note>& result)
{
    double startPos = pos;
    for (const Node& child: group.children)
    {
        m_transposition += group.value;
        pos += evalNode(child, pos, step, result);
        m_transposition -= group.value;
    }
    return pos - startPos;
}

double BhatkandeEvaluator::evalNode(const Node& node, double pos, double step, std::vector<Note>& result)
{
    if (node.repeat < 1) { return 0; }

    double startPos = pos;
    for (int i = 0; i < node.repeat; i++)
    {
        pos += evalNodeOnce(node, pos, step, result);
    }
    return pos - startPos;
}


double BhatkandeEvaluator::evalNodeOnce(const Node& node, double pos, double step, std::vector<Note>& result)
{
    double startPos = pos;

    switch (node.type)
    {
    case Node::Type::Extend:
        // If the song starts with - it will be treated as a rest
        if (!result.empty())
        {
            result.back().setLength(result.back().length() + toTicks(step));
        }
        pos += step;
        break;
    case Node::Type::Glide:
        pos += evalGlide(node, pos, step, result);
        break;
    case Node::Type::Group:
        pos += evalGroup(node, pos, step, result);
        break;
    case Node::Type::Tuple:
        pos += evalTuple(node, pos, step, result);
        break;
    case Node::Type::Note:
        result.emplace_back(Note(toTicks(step), toTicks(pos), node.value + m_transposition));
        pos += step;
        break;
    case Node::Type::Rest:
        pos += step;
        break;
    case Node::Type::Transpose:
        m_transposition = node.value;
        break;
    default:
        throw ParserError("eval not implemented (should not happen)", SourcePos{});
    }

    return pos - startPos;
}




double BhatkandeEvaluator::evalTuple(const Node& tuple, double pos, double step, std::vector<Note>& result)
{
    int childCount = 0;
    double childDuration = step;

    for (const Node& child: tuple.children)
    {
        childCount += countSteps(child);
    }
    if (childCount > 0)
    {
        childDuration = step / childCount;
    }

    evalGroup(tuple, pos, childDuration, result);

    return step;
}






// --------------- PARSER PUBLIC API ---------------

BhatkandeParser::BhatkandeParser()
    : m_parser()
{
}

void BhatkandeParser::parse(const QString& string)
{
    auto tokens = BhatkandeLexer().scan(string.toStdString());
    auto tree = m_parser.parse(tokens);
    m_notes = BhatkandeEvaluator().evaluate(tree);
};

bool BhatkandeParser::isSafeToWrite(const MidiClip* clipInPianoRoll,
	const std::vector<const Model*>& previouslyChanged)
{
    return true; // @todo

	// True if previouslyChanged contains clipInPianoRoll
	return std::find(previouslyChanged.begin(), previouslyChanged.end(), clipInPianoRoll) != previouslyChanged.end();
}



std::vector<const Model*> BhatkandeParser::write(MidiClip* clipInPianoRoll)
{
    if (!clipInPianoRoll) { return {}; }

	clipInPianoRoll->clear();

	for (const Note& note: m_notes)
	{
		clipInPianoRoll->addNote(note, false);
	}

    return {static_cast<Model*>(clipInPianoRoll)};
}



} // namespace lmms::melodyeditor
