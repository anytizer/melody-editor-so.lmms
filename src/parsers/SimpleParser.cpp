/**
 * SimpleParser.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include "SimpleParser.h"

#include <QString>
#include <QList>
#include <QRegularExpression>

#include "MidiClip.h"


#include "../includes/Utilities.h"


namespace
{

constexpr int DEFAULT_LENGTH = 48;
constexpr int DEFAULT_OCTAVE = 5;

constexpr auto OCTAVE_DOWN = ".";
constexpr auto OCTAVE_UP = "*";
constexpr auto STEP = "-";

static const QRegularExpression ALTERNATIVE_OCTAVE_UP("[º°’]");
static const QRegularExpression ALTERNATIVE_STEP("[_—~ऽ]");

static const QRegularExpression CHORD_ATTRIBUTES("[- \t]*");
static const QRegularExpression NOTE_ATTRIBUTES("[#b]*[*.]*[- \t]*");
static const QRegularExpression REST_ATTRIBUTES("[- \t]*");
static const QRegularExpression UNTIL_END_OF_LINE("[^\\n]+");


//! Convert letter to a relative key
int pianoKey(const char c)
{
	switch (c)
	{
	case 'C': case 'c': return 0;
	case 'D': case 'd': return 2;
	case 'E': case 'e': return 4;
	case 'F': case 'f': return 5;
	case 'G': case 'g': return 7;
	case 'A': case 'a': return 9;
	case 'B': case 'b': return 11;
	case 'H': case 'h': return 11;
	default: return -1;
	}
}

}


namespace lmms::melodyeditor
{


void SimpleParser::parse(const QString& string)
{
	m_notes.clear();
	m_chord.clear();
	m_timePos = 0;
	m_insideChord = false;

	// Translate string from the selected dialect to English note names
	QString translated = string;
	for (const auto& [match, replacement]: m_dialect.replacements)
	{
		translated.replace(match, replacement);
	}

	// Some extra translations
	translated
		.replace(ALTERNATIVE_OCTAVE_UP, OCTAVE_UP)
		.replace(ALTERNATIVE_STEP, STEP)
		.replace("™", "#")
		.replace("।", "|");

	m_reader = StringReader(translated);

	while (!m_reader.reachedEOF())
	{
		process();
	}
}


void SimpleParser::process()
{
	StringReader& s = m_reader;
	char c = s.advance();
	switch (c)
	{	
	case '\r':
	case '\n':
	case '\t':
	case ' ':
	case '|':
	case '/':
	case '\\':
	{
		break;
	}
	case ',':
	{
		// time shared
		// 1 comma = 2 pieces = 50% each note time
		// 2 comma = 3 pieces = 33% each note time
		// 3 comma = 4 pieces = 25% each note time
		break;
	}
	case '-':
		//@todo continuation of last note in this time slot
		break;
	case 'r':
	case 'x':
	case 'X':
	{
		int steps = 1 + s.readString(REST_ATTRIBUTES).count(STEP);
		m_timePos += DEFAULT_LENGTH * steps;
		break;
	}
	case '#':
	{
		s.consume(UNTIL_END_OF_LINE);
		break;
	}
	case '[':
	{
		if (m_insideChord) { throw ParserError(s.pos() + "cannot nest chords"); }
		m_insideChord = true;
		break;
	}
	case ']':
	{
		if (!m_insideChord) { throw ParserError(s.pos() + "unexpected ]"); }
		m_insideChord = false;

		int steps = s.readString(CHORD_ATTRIBUTES).count(STEP);
		int chordLength = DEFAULT_LENGTH * (1 + steps);

		// If we have set an explicit chord length, apply it to all notes in the chord
		if (steps)
		{
			for (auto& note: m_chord)
			{
				note.setLength(chordLength);
			}
		}

		// Or find the shortest note in the chord
		else if (!m_chord.empty())
		{
			chordLength = m_chord[0].length();
			for (const auto& note: m_chord)
			{
				chordLength = std::min<int>(chordLength, note.length());
			}
		}

		// Move the time cursor
		m_timePos += chordLength;

		// Move the chord notes into the regular note vector
		std::move(m_chord.begin(), m_chord.end(), std::back_inserter(m_notes));

		break;
	}
	default:
	{
		int key = pianoKey(c);

		if (key == -1) { throw ParserError(s.pos() + "unexpected character: " + c); }

		auto attribs = s.readString(NOTE_ATTRIBUTES);

		key += attribs.count('#');
		key -= attribs.count('b');

		int octave = DEFAULT_OCTAVE;
		octave += attribs.count(OCTAVE_UP);
		octave -= attribs.count(OCTAVE_DOWN);
		key += octave * 12;

		int length = DEFAULT_LENGTH * (1 + attribs.count(STEP));

		auto& destination = m_insideChord ? m_chord : m_notes;
		destination.emplace_back(length, m_timePos, key);

		if (!m_insideChord) { m_timePos += length; }
	}

	} // switch
}


std::vector<const Model*> SimpleParser::write(MidiClip* clipInPianoRoll)
{
	writeNotesToMidiClip(m_notes, clipInPianoRoll);
	return {clipInPianoRoll};
}

} // namespace lmms::melodyeditor
