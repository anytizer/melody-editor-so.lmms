/**
 * SimpleParser.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#pragma once

#include "Note.h"

#include "AbstractParser.h"
#include "SimpleDialects.h"
#include "StringReader.h"


namespace lmms::melodyeditor
{


class SimpleParser : public AbstractParser
{
public:
	SimpleParser(const Dialect& dialect) : m_dialect(dialect) {}

	QString name() const override { return m_dialect.name; }
	std::string icon() const override { return m_dialect.icon; }
	QString help() const override { return "TODO"; }
	QString logMessages() const override { return "TODO"; }

	void parse(const QString& string) override;

	// Since we only write to piano roll, it's safe to write if piano roll was previously changed
	bool isSafeToWrite(const MidiClip* clipInPianoRoll, const std::vector<const Model*>& previous) override
	{
		//@todo
		return true;

		// This caused the button not to send data to currently open MIDI clip
		// return std::find(previous.begin(), previous.end(), clipInPianoRoll) != previous.end();
	}

	std::vector<const Model*> write(MidiClip* clipInPianoRoll) override;

private:
	void process();

	Dialect m_dialect;

	std::vector<Note> m_notes {};
	std::vector<Note> m_chord {};

	StringReader m_reader;
	TimePos m_timePos;
	bool m_insideChord;
};


}