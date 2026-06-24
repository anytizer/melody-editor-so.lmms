/**
 * ABCParser.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include "ABCParser.h"

extern "C"
{
	// third party tool
     #include "../../ABC-parser/abc_parser.h"
}

#include "MidiClip.h"

namespace lmms::melodyeditor
{

static constexpr int MAX_VOICES = 20;
static constexpr int MAX_NOTES = 1024; // 512; // 256; // 128; // pool exhausted

void ABCParser::parse(const QString& string)
{
	static NotePool g_pools[MAX_VOICES];
	static struct note g_storage[MAX_VOICES][MAX_NOTES];
	static struct sheet g_sheet;

	m_notes.clear();

	// Initialize pools with storage
	for (int i = 0; i < MAX_VOICES; i++)
	{
		note_pool_init(&g_pools[i], g_storage[i], MAX_NOTES, ABC_MAX_CHORD_NOTES);
	}
	sheet_init(&g_sheet, g_pools, MAX_VOICES);

	// Parse ABC notation
	int result = abc_parse(&g_sheet, string.toStdString().c_str());
	if (result < 0)
	{
		// -1: invalid input
		// -2: pool exhausted
		throw ParserError("Too many notes.");
		return;
	}

	for (int poolNum = 0; poolNum < MAX_VOICES; poolNum++)
	{
		int timePos = 0;
		struct note *n = pool_first_note(&g_pools[poolNum]);
		while (n)
		{
			for (int noteNum=0; noteNum < n->chord_size; noteNum++)
			{
				m_notes.push_back(Note(n->duration, timePos, n->midi_note[noteNum]));
			}

			timePos += n->duration;
			n = note_next(&g_pools[poolNum], n);
		}
	}

	// Reuse memory for another parse
	sheet_reset(&g_sheet);
}

std::vector<const Model*> ABCParser::write(MidiClip* clipInPianoRoll)
{
	if (!clipInPianoRoll || m_notes.empty()) { return {}; }

	clipInPianoRoll->clear();
	for (auto note: m_notes)
	{
		clipInPianoRoll->addNote(note, false);
	}

	return {};
}


} // namespace lmms::melodyeditor
