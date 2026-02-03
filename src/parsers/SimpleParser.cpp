/**
 * SimpleParser.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */


#include <QString>
#include <QList>
#include <QRegularExpression>

#include "Note.h"
#include "MidiClip.h"

#include "SimpleParser.h"
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
static const QRegularExpression UNTIL_END_OF_LINE("[^\\r\\n]+");


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

} // namespace


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
		.replace("™", "#") // TradeMark => tivra ma => F#
		.replace("।", "|")
		.replace("\r", "\n");
	
	// temporarily strip non-ascii characters
	translated = translated.replace(QRegularExpression(QString("[^\\x00-\\x7F]")), "");

	QStringList lines = translated.split('\n');
	for(QString line: lines)
	{
		if(line.startsWith('#'))
			continue;

		QRegularExpression re("\\s+"); // linearly access
		QStringList words = line.split(re, Qt::SkipEmptyParts);
		for(QString word: words)
		{
			if(word.contains(","))
			{
				int NOTE_LENGTH = DEFAULT_LENGTH / (1+word.count(","));
				QStringList notes = word.split(",");
				for(QString note: notes)
				{
					process(note, NOTE_LENGTH);
				}
			}
			else
			{
				process(word, DEFAULT_LENGTH);
			}
		}
	}
}


void SimpleParser::process(QString note, int NOTE_LENGTH)
{
	m_reader = StringReader(note);
	StringReader& s = m_reader;
	
	char c = s.advance();

	switch (c)
	{	
	case '|':
	case '/':
	case '\\':
		break;
	case '-':
		// If we are inside a chord [CEG -], extend every note in the current chord
        if (m_insideChord)
        {
            if (!m_chord.empty())
            {
                for (auto& note : m_chord)
                {
                    note.setLength(note.length() + NOTE_LENGTH);
                }
                // We don't advance m_timePos here; it's handled when the chord ']' closes
            }
        }
        else
        {
            // If not in a chord, extend the last note added to the list
            if (!m_notes.empty())
            {
                // back() gives us a reference to the last Note object
                auto& lastNote = m_notes.back();
                lastNote.setLength(lastNote.length() + NOTE_LENGTH);
                
                // Move the cursor forward so the NEXT note doesn't overlap the extension
                m_timePos += NOTE_LENGTH;
            }
            else
            {
                // If '-' is the very first character, treat it as a rest
                m_timePos += NOTE_LENGTH;
            }
        }
        break;
	case 'r': // @toodo fix match with RE Komal in SARGAM
	case 'x':
	case 'X':
	{
		//int steps = 1 + s.readString(REST_ATTRIBUTES).count(STEP);
		m_timePos += NOTE_LENGTH; // * steps;
		// float steps = 1 / (1+s.readString(REST_ATTRIBUTES).count(STEP));
		break;
	}
	case '#':
	{
		// @todo needs testing
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
		int chordLength = NOTE_LENGTH * (1 + steps);

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

		if (key == -1) {
			throw ParserError(s.pos() + "unexpected character: " + c);
		}

		auto attribs = s.readString(NOTE_ATTRIBUTES);

		key += attribs.count('#');
		key -= attribs.count('b');

		int octave = DEFAULT_OCTAVE;
		octave += attribs.count(OCTAVE_UP);
		octave -= attribs.count(OCTAVE_DOWN);
		key += octave * 12;

		auto& destination = m_insideChord ? m_chord : m_notes;
		destination.emplace_back(NOTE_LENGTH, m_timePos, key);

		if (!m_insideChord) {
			m_timePos += NOTE_LENGTH;
		}
	}

	} // switch
}


std::vector<const Model*> SimpleParser::write(MidiClip* clipInPianoRoll)
{
	writeNotesToMidiClip(m_notes, clipInPianoRoll);
	return { clipInPianoRoll };
}


} // namespace lmms::melodyeditor
