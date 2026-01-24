#ifndef LMMS_MELODY_EDITOR_PARSER_H
#define LMMS_MELODY_EDITOR_PARSER_H

#include "AbstractParser.h"

#include <map>
#include <vector>

#include <QRegularExpression>
#include <QString>
#include <QVariant>

#include "Note.h"

#include "StringReader.h"


namespace lmms
{
class InstrumentTrack;
}


namespace lmms::melodyeditor
{

//
// Data types
//

using KeySignature = std::array<int, 7>;
using RepeatRanges = std::vector<std::pair<int, int>>;

//
// String reader with alda-specific extensions
//


class AldaStringReader : public StringReader
{
public:
	int readAccidentals();
	float readDuration();
	KeySignature readKeySig();
	int readRepetitions();
};


//
// Event classes, created by parsing the string
//

struct Score;
struct Part;
struct Event
{
	std::vector<std::shared_ptr<Event>> m_children = {};
	RepeatRanges m_ranges = {};
	int m_repeats = 1;

	//! Evaluate this as many times as repeat/range says
	void repeatedEval(Score& score, Part& part, int currentRepetition);

private:
	//! To be implemented by child classes to edit the score or part
	//! (private because you should only use repeatedEval in code)
	virtual void eval(Score& score, Part& part, int currentRepetition) {}
};


using EventPtr = std::shared_ptr<Event>;


struct AttributeEvent : public Event
{
	QString m_name;
	std::vector<QVariant> m_args = {};
	bool m_global = false;

	AttributeEvent(AldaStringReader& s);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct ChordEvent : public Event
{
	float m_duration = -1;

	ChordEvent(AldaStringReader& s);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct MarkerEvent : public Event
{
	QString m_name;
	bool m_isDefinition = false;

	MarkerEvent(AldaStringReader& s, bool isDefinition = false);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct NoteOrRestEvent : public Event
{
	int m_noteNumber = -1;
	float m_duration = -1;
	bool m_slur = false;

	NoteOrRestEvent(AldaStringReader& s);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct OctaveChangeEvent : public Event
{
	int m_value;
	bool m_relative = false;

	OctaveChangeEvent(AldaStringReader& s, int relativeChange = 0);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct SequenceEvent : public Event
{
    bool m_cram = false;
	float m_cramDuration = -1;

	SequenceEvent(AldaStringReader& s, bool isCram = false);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct VariableEvent : public Event
{
	QString m_name;
    // bool m_isDefinition = false; by default true if it has children

	VariableEvent(AldaStringReader& s);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


struct VoiceChangeEvent : public Event
{
	int m_number;

	VoiceChangeEvent(AldaStringReader& s);
	void eval(Score& score, Part& part, int currentRepetition) override;
};


//
// Score structures, created by evaluating events
//


constexpr float DEFAULT_DURATION = 1 / 4.f;
constexpr KeySignature DEFAULT_KEY_SIG {0, 2, 4, 5, 7, 9, 11};
constexpr int DEFAULT_OCTAVE = 5;


struct Voice
{
	float pos = 0;
	int octave = DEFAULT_OCTAVE;
	float defaultDuration = DEFAULT_DURATION; // note duration if not specified
	float gate = 1; // 0-1 how much of a note should be audible
	volume_t volume = 100; // 0-200
	panning_t panning = 0; // -100-100
	KeySignature keySig = DEFAULT_KEY_SIG;
};


struct Part
{
	QString instrument;
	std::vector<EventPtr> events = {};
	std::vector<lmms::Note> notes = {};
	std::map<int, Voice> voices = {};
	Voice* currentVoice = &voices[0]; // creates voice 0
};


struct Score
{
	std::map<QString, std::vector<QString>> aliases = {};
	std::map<float, EventPtr> globalEvents = {};
	std::map<QString, float> markers = {};
	std::map<QString, Part> parts = {};
	std::map<QString, std::vector<EventPtr>> variables = {};
	QStringList warnings = {};
};


//
// Main parser class
//


class AldaParser : public AbstractParser
{
public:
	QString name() const override { return "Alda"; }
	std::string icon() const override { return "ns-english"; }
	QString help() const override { return "TODO"; }
	QString logMessages() const override { return m_score.warnings.join("\n"); }

	void parse(const QString& string) override;
	bool isSafeToWrite(const MidiClip* clipInPianoRoll, const std::vector<const Model*>& previouslyChanged);
	std::vector<const Model*> write(MidiClip* clipInPianoRoll) override;

private:
	//! Read part definition and return a list of Part names
	//! (new parts will be created and new aliases saved)
	std::vector<Part*> parsePartDefinition();

	AldaStringReader m_stringReader;
	Score m_score = {};
};

}

#endif // PARSER_H
