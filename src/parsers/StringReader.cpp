/**
 * StringReader.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include "StringReader.h"


namespace
{
constexpr auto LAST_PRINTABLE_ASCII_CHAR = '~'; // 0x7E?
const auto INTEGER_PATTERN = QRegularExpression("\\d+");
const auto FLOAT_PATTERN = QRegularExpression("\\d+([.]\\d+)?");
static const QRegularExpression WORD_PATTERN("[^\r\n\t ]+"); // SPACE included
}


namespace lmms::melodyeditor
{


char StringReader::peek()
{
	if (reachedEOF())
	{
		throw ParserError("unexpected end of file");
	}
	else if (m_string.at(m_pos) > LAST_PRINTABLE_ASCII_CHAR)
	{
		// @todo Should support unicode comments too, after a # symbol
		throw ParserError(pos() + "unsupported character: " + m_string.at(m_pos)+" in: "+m_string);
	}

	return m_string.at(m_pos).toLatin1();
}


char StringReader::advance()
{
	auto c = peek();
	m_pos++;

	if (c == '\n')
	{
		m_line++;
		m_column = 0;

		return advance();
	}
	else
	{
		m_column++;
	}

	return c;
}


QString StringReader::word()
{
	QString word = "";

	QRegularExpressionMatch match = WORD_PATTERN.match(m_string);
	if (match.hasMatch()) {
		word = match.captured(0); 
	}

	return word;
}


QRegularExpressionMatch StringReader::match(const QRegularExpression& regex, bool consume)
{
	if (reachedEOF()) { return {}; }

	const auto m = regex.match(
		m_string,
		m_pos,
		QRegularExpression::NormalMatch,
		QRegularExpression::AnchoredMatchOption
		);

	if (consume)
	{
		m_pos += m.capturedLength();
		if (auto lastIndexOfNewline = m.captured().lastIndexOf('\n') != -1)
		{
			// Update line and column - example:
			// captured = "one\ntwo"
			// capturedLength = 7
			// lastIndexOf(\n) = 3
			// lettersAfterN = 7 - 3 - 1 = 3
			m_column = m.capturedLength() - lastIndexOfNewline - 1;
			m_line += m.captured().count('\n');
		}
	}

	return m;
}


float StringReader::readFloat(const QString& errorMsg)
{
	return readString(FLOAT_PATTERN, errorMsg).toFloat();
}


int StringReader::readInt(const QString& errorMsg)
{
	return readString(INTEGER_PATTERN, errorMsg).toInt();
}


QString StringReader::readString(const QRegularExpression& regex, const QString& errorMsg)
{
	auto m = match(regex, true);

	if (!m.hasMatch() && !errorMsg.isNull())
	{
		throw ParserError(pos() + errorMsg);
	}

	return m.captured();
}


} // namespace lmms::melodyeditor
