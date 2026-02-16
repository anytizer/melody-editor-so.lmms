/**
 * BhatkhandeParser.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#pragma once

#include "Note.h"

#include "AbstractParser.h"

namespace lmms::melodyeditor
{


class BhatkhandeLexer
{
public:
	struct Token
	{
		enum class Type
		{
			CurlyLeft = '{',
			CurlyRight = '}',
			Dash = '-',
            Dot = '.',
            Equal = '=',
			ParenLeft = '(',
			ParenRight = ')',
            SingleQuote = '\'',
			SquareLeft = '[',
			SquareRight = ']',
			Star = '*',
            EndOfFile = 128,
            Name,
            Newline,
            Number,
            Whitespace,
			_key,
			_rest,
			_transpose,
		};

		Type type;
		SourcePos pos;

		int integerValue = 0;
		std::string stringValue = "";

        std::string displayName()
        {
            switch (type)
            {
            case Type::EndOfFile: return "end of file";
            case Type::Name: return "name";
            case Type::Newline: return "newline";
            case Type::Number: return "number";
            case Type::Whitespace: return "whitespace";
            case Type::_key: return "key";
            case Type::_rest: return "rest";
            case Type::_transpose: return "transpose";
            default: return std::string(1, static_cast<char>(type));
            }
        }
	};

	std::vector<Token> scan(const std::string& string);

private:
	SourcePos sourcePos()
	{
		return SourcePos{m_line, m_column, m_index};
	}

	//! True if there are no more characters to parse
	bool eof()
	{
		return m_index >= m_string.length();
	}

	char peek()
	{
		if (eof())
		{
			throw ParserError("unexpected end of file", sourcePos());
		}
		return m_string[m_index];
	}

	char read();

	void addToken(const Token& token);
	void flushBuffer();

	void handleHash();
	void handleWhitespace();

	std::string m_string;
	size_t m_index = 0;

	int m_line = 1;
	int m_column = 0;

	std::vector<Token> m_tokens;

	// String buffer, used to read long tokens like names or numbers
	std::string m_buffer;

	// Line and column where the buffered string started
	SourcePos m_bufferStartPos;
};




class BhatkhandeParserPrivate
{
public:
    using Token = BhatkhandeLexer::Token;

    struct Node
    {
        enum class Type
        {
            Extend,
            Glide,
            Group,
            Note,
            Rest,
            Tuple,
            Transpose,
        };

        Type type;
        int value = 0;
        int repeat = 1;

        std::vector<Node> children = {};
    };

	Node parse(const std::vector<Token>& tokens);

private:
	struct Macro
	{
		std::string name;
		Node value = {};
	};


	// --- Helpers ---


	bool eof()
	{
		return m_index >= m_tokens.size();
	}

	Token peek()
	{
		if (eof())
		{
			throw ParserError("unexpected end of file", SourcePos{});
		}
		return m_tokens[m_index];
	}

	Token read()
	{
		Token token = peek();
		m_index++;
		return token;
	}

	bool isFollowedByEqualSign();
	int readNumber();
	void skipWhitespace();

	//! Return existing Macro with the given name, or create a new one
	Macro& getMacro(const std::string& name);

	//! Return a human readable representation of the given token
	std::string tokenString(const Token& token);


	// --- Parsing ---


	//! Main parse function - append child nodes to the container until we hit endToken
	void parseContainer(Node& container, Token::Type endToken);

	//! Save a macro definition to m_macros
	void parseDefinition(const Token& name);

	///! Expand a macro name to a list of nodes
	std::vector<const Node*> parseName(const Token& name);


	// --- Data members ---

	std::vector<Token> m_tokens;
	size_t m_index;

	/*! \brief Macro names and tokens
	 *
	 *  Must be sorted longest name to shortest name, because it determines how strings are split.
	 *  For example we want the string "SaRe" to match "Sa" before "S".
	 *  If it matched "S" first, it would leave behind "aRe", which is invalid.
	 */
	std::vector<Macro> m_macros;
};


class BhatkhandeEvaluator
{
public:
    using Node = BhatkhandeParserPrivate::Node;

    std::vector<Note> evaluate(const Node& node);

private:
    int toTicks(double bars)
    {
        return TimePos(static_cast<int>(std::round(DefaultTicksPerBar * bars)));
    }

    //! Relative node duration in beats (don't count empty Tuples, Transpose, etc)
    int countSteps(const Node& node);

    double evalGlide(const Node& glide, double pos, double step, std::vector<Note>& result);
    double evalGroup(const Node& group, double pos, double step, std::vector<Note>& result);
    double evalNode(const Node& node, double pos, double step, std::vector<Note>& result);
    double evalNodeOnce(const Node& node, double pos, double step, std::vector<Note>& result);
    double evalTuple(const Node& tuple, double pos, double step, std::vector<Note>& result);

    std::vector<Note> m_result;

    int m_transposition = 0;
};


class BhatkhandeParser : public AbstractParser
{
public:
    // @todo dialects
    BhatkhandeParser();

    QString name() const override { return "Bhatkhande"; }
    std::string icon() const override { return "ns-hindustani"; }
    QString help() const override { return ""; } // @todo
    QString logMessages() const override { return ""; }  // @todo?

    bool isSafeToWrite(const MidiClip* clipInPianoRoll,
                       const std::vector<const Model*>& previouslyChanged) override;

    void parse(const QString& string) override;

    std::vector<const Model*> write(MidiClip* clipInPianoRoll) override;

private:
    BhatkhandeParserPrivate m_parser;
    std::vector<Note> m_notes;
};


} // namespace lmms::melodyeditor
