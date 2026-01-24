#ifndef LMMS_MELODY_EDITOR_STRING_READER_H
#define LMMS_MELODY_EDITOR_STRING_READER_H

#include <QRegularExpression>


namespace lmms::melodyeditor
{


class ParserError : public std::runtime_error
{
public:
	ParserError(const QString& msg)
		: std::runtime_error(msg.toStdString())
	{}
};



class StringReader
{
public:
	StringReader(const QString& string = {})
		: m_string(string)
	{}

	//! True if there are no more characters to parse
	bool reachedEOF()
	{
		return m_pos >= m_string.length();
	}

	//! Return the current character, but don't consume it
	char peek();

	//! Consume and return the current character
	char advance();

	//! True if the current character is a digit
	bool isDigit()
	{
		return !reachedEOF() && '0' <= peek() && peek() <= '9';
	}

	//! Return a regex match, starting from the current position
	QRegularExpressionMatch match(const QRegularExpression& regex, bool consume);

	//! Consume a matching character
	bool consume(char c)
	{
		if (!reachedEOF() && peek() == c) { advance(); return true; }
		else { return false; }
	}

	//! Consume a regex (convenience function)
	bool consume(const QRegularExpression& regex)
	{
		return match(regex, true).capturedLength() > 0;
	}

	//! True if the regex matches (convenience function)
	bool matches(const QRegularExpression& regex)
	{
		return match(regex, false).capturedLength() > 0;
	}

	//! Consume digits and decimal point and return float (0.f if there's no digit)
	float readFloat(const QString& errorMsg = {});

	//! Consume digits and return int (0 if there's no digit)
	int readInt(const QString& errorMsg = {});

	//! Consume and return a string that matchs the regex (empty string if there's no match)
	QString readString(const QRegularExpression& regex, const QString& errorMsg = {});

	QString pos()
	{
		return QString("%1:%2: ").arg(m_line).arg(m_column);
	}

	int line() const { return m_line; }
	int column() const { return m_column; }

private:
	QString m_string;
	int m_pos = 0;
	int m_line = 1;
	int m_column = 0;
};


} // namespace lmms::melodyeditor


#endif // LMMS_MELODY_EDITOR_STRING_READER_H