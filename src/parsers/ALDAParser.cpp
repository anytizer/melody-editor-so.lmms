#include "AldaParser.h"

#include <exception>
#include <set>
#include <vector>

#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QSet>
#include <QTextStream>

#include "Engine.h"
#include "Instrument.h"
#include "InstrumentTrack.h"
#include "MidiClip.h"
#include "Song.h"

#include "../includes/Utilities.h"


namespace lmms::melodyeditor
{


namespace
{


static const QRegularExpression ATTRIBUTE("[\\w-]+");
static const QRegularExpression FLOAT("\\d+[.]");
static const QRegularExpression FLUFF("([\\s|]|#[^\\n]*)+"); // whitespace or comment
static const QRegularExpression KEY_SIG_PART("[a-g][+-]+");
static const QRegularExpression NAME("[a-zA-Z]{2}[a-zA-Z0-9'()_+-]*"); // part name, alias or marker
static const QRegularExpression START_OF_PART(NAME.pattern() + "(" + FLUFF.pattern() + ")?[\"/:]");
static const QRegularExpression START_OF_TIE("~\\s*\\d"); // differentiates a ties from a slur
static const QRegularExpression VARIABLE("[a-zA-Z]{2}[a-zA-Z0-9_]*");
static const QRegularExpression WHITESPACE_ONLY("[\\s]+");

// Copied from alda/client/model/instruments.go
static const std::map<QString, int> SOUNDFONT_PATCH_NAMES{
	{"acoustic-grand-piano", 0},
	{"piano", 0},
	{"bright-acoustic-piano", 1},
	{"electric-grand-piano", 2},
	{"honky-tonk-piano", 3},
	{"electric-piano-1", 4},
	{"electric-piano-2", 5},
	{"harpsichord", 6},
	{"clavi", 7},
	{"clavinet", 7},
	{"celesta", 8},
	{"celeste", 8},
	{"glockenspiel", 9},
	{"music-box", 10},
	{"vibraphone", 11},
	{"vibes", 11},
	{"marimba", 12},
	{"xylophone", 13},
	{"tubular-bells", 14},
	{"dulcimer", 15},
	{"drawbar-organ", 16},
	{"percussive-organ", 17},
	{"rock-organ", 18},
	{"church-organ", 19},
	{"organ", 19},
	{"reed-organ", 20},
	{"accordion", 21},
	{"harmonica", 22},
	{"tango-accordion", 23},
	{"acoustic-guitar-nylon", 24},
	{"acoustic-guitar", 24},
	{"guitar", 24},
	{"acoustic-guitar-steel", 25},
	{"electric-guitar-jazz", 26},
	{"electric-guitar-clean", 27},
	{"electric-guitar-palm-muted", 28},
	{"electric-guitar-overdrive", 29},
	{"electric-guitar-distorted", 30},
	{"electric-guitar-harmonics", 31},
	{"acoustic-bass", 32},
	{"upright-bass", 32},
	{"electric-bass-finger", 33},
	{"electric-bass", 33},
	{"electric-bass-pick", 34},
	{"fretless-bass", 35},
	{"bass-slap", 36},
	{"bass-pop", 37},
	{"synth-bass-1", 38},
	{"synth-bass-2", 39},
	{"violin", 40},
	{"viola", 41},
	{"cello", 42},
	{"contrabass", 43},
	{"string-bass", 43},
	{"arco-bass", 43},
	{"double-bass", 43},
	{"tremolo-strings", 44},
	{"pizzicato-strings", 45},
	{"orchestral-harp", 46},
	{"harp", 46},
	{"timpani", 47},
	{"string-ensemble-1", 48},
	{"string-ensemble-2", 49},
	{"synth-strings-1", 50},
	{"synth-strings-2", 51},
	{"choir-aahs", 52},
	{"voice-oohs", 53},
	{"synth-voice", 54},
	{"orchestra-hit", 55},
	{"trumpet", 56},
	{"trombone", 57},
	{"tuba", 58},
	{"muted-trumpet", 59},
	{"french-horn", 60},
	{"brass-section", 61},
	{"synth-brass-1", 62},
	{"synth-brass-2", 63},
	{"soprano-saxophone", 64},
	{"soprano-sax", 64},
	{"alto-saxophone", 65},
	{"alto-sax", 65},
	{"tenor-saxophone", 66},
	{"tenor-sax", 66},
	{"baritone-saxophone", 67},
	{"baritone-sax", 67},
	{"bari-sax", 67},
	{"oboe", 68},
	{"english-horn", 69},
	{"bassoon", 70},
	{"clarinet", 71},
	{"piccolo", 72},
	{"flute", 73},
	{"recorder", 74},
	{"pan-flute", 75},
	{"bottle", 76},
	{"shakuhachi", 77},
	{"whistle", 78},
	{"ocarina", 79},
	{"square-lead", 80},
	{"square", 80},
	{"square-wave", 80},
	{"saw-wave", 81},
	{"sawtooth", 81},
	{"saw-lead", 81},
	{"calliope-lead", 82},
	{"calliope", 82},
	{"chiffer-lead", 83},
	{"chiffer", 83},
	{"chiff", 83},
	{"charang", 84},
	{"solo-vox", 85},
	{"fifths", 86},
	{"sawtooth-fifths", 86},
	{"bass-and-lead", 87},
	{"bass+lead", 87},
	{"synth-pad-new-age", 88},
	{"pad-new-age", 88},
	{"new-age-pad", 88},
	{"synth-pad-warm", 89},
	{"pad-warm", 89},
	{"warm-pad", 89},
	{"synth-pad-polysynth", 90},
	{"pad-polysynth", 90},
	{"polysynth-pad", 90},
	{"synth-pad-choir", 91},
	{"pad-choir", 91},
	{"choir-pad", 91},
	{"synth-pad-bowed", 92},
	{"pad-bowed", 92},
	{"bowed-pad", 92},
	{"pad-bowed-glass", 92},
	{"bowed-glass-pad", 92},
	{"synth-pad-metallic", 93},
	{"pad-metallic", 93},
	{"metallic-pad", 93},
	{"pad-metal", 93},
	{"metal-pad", 93},
	{"synth-pad-halo", 94},
	{"pad-halo", 94},
	{"halo-pad", 94},
	{"synth-pad-sweep", 95},
	{"pad-sweep", 95},
	{"sweep-pad", 95},
	{"fx-rain", 96},
	{"fx-ice-rain", 96},
	{"rain", 96},
	{"ice-rain", 96},
	{"fx-soundtrack", 97},
	{"soundtrack", 97},
	{"fx-crystal", 98},
	{"crystal", 98},
	{"fx-atmosphere", 99},
	{"atmosphere", 99},
	{"fx-brightness", 100},
	{"brightness", 100},
	{"fx-goblins", 101},
	{"fx-goblin", 101},
	{"goblins", 101},
	{"goblin", 101},
	{"fx-echoes", 102},
	{"fx-echo-drops", 102},
	{"echoes", 102},
	{"echo-drops", 102},
	{"fx-sci-fi", 103},
	{"sci-fi", 103},
	{"sitar", 104},
	{"banjo", 105},
	{"shamisen", 106},
	{"koto", 107},
	{"kalimba", 108},
	{"bagpipes", 109},
	{"fiddle", 110},
	{"shehnai", 111},
	{"shahnai", 111},
	{"shenai", 111},
	{"shanai", 111},
	{"tinkle-bell", 112},
	{"tinker-bell", 112},
	{"agogo", 113},
	{"steel-drums", 114},
	{"steel-drum", 114},
	{"woodblock", 115},
	{"taiko-drum", 116},
	{"melodic-tom", 117},
	{"synth-drum", 118},
	{"reverse-cymbal", 119},
	{"guitar-fret-noise", 120},
	{"breath-noise", 121},
	{"seashore", 122},
	{"bird-tweet", 123},
	{"telephone-ring", 124},
	{"helicopter", 125},
	{"applause", 126},
	{"gunshot", 127},
	{"gun-shot", 127},
	};




bool inRange(int i, const RepeatRanges& ranges)
{
	if (ranges.empty()) { return true; }

	for (const auto& [first, last]: ranges)
	{
		if (first <= i && i <= last) { return true; }
	}
	return false;
}


int toTicks(float bars)
{
	return static_cast<int>(std::round(DefaultTicksPerBar * bars));
}


int noteNumber(char c)
{
	switch (c)
	{
	case 'c': return 0;
	case 'd': return 1;
	case 'e': return 2;
	case 'f': return 3;
	case 'g': return 4;
	case 'a': return 5;
	case 'b': return 6;
	default: return -1;
	}
}


} // end of anonymous namespace









int AldaStringReader::readAccidentals()
{
	int offset = 0;
	while (!reachedEOF())
	{
		switch (peek())
		{
		case '-': offset -= 1; break;
		case '+': offset += 1; break;
		case '_': offset = 0; break;
		default: return offset;
		}
		advance();
	}
	return offset;
}




float AldaStringReader::readDuration()
{
	float denominator = readFloat();
	if (denominator == 0.f) { throw ParserError(pos() + "duration cannot be zero"); }

	float duration = 1 / denominator;

	float dotDuration = duration / 2;
	while (consume('.'))
	{
		duration += dotDuration;
		dotDuration /= 2;
	}

	consume(FLUFF);

	if (matches(START_OF_TIE))
	{
		consume('~');
		consume(FLUFF);
		duration += readDuration();
	}

	return duration;
}




KeySignature AldaStringReader::readKeySig()
{
	KeySignature keySig = DEFAULT_KEY_SIG;
	consume(WHITESPACE_ONLY);
	while (!reachedEOF() && !consume('"'))
	{
		auto letterAndAccidentals = readString(KEY_SIG_PART, "expected note followed by + or -");

		int note = noteNumber(letterAndAccidentals[0].toLatin1());
		if (note < 0) { throw ParserError(pos() + "expected note letter"); }

		keySig[note] += letterAndAccidentals.count("+");
		keySig[note] -= letterAndAccidentals.count("-");
		consume(WHITESPACE_ONLY);
	}
	return keySig;
}




int AldaStringReader::readRepetitions()
{
	if (!consume('*')) { return 1; }
	consume(FLUFF);
	int repetitions = readInt("expected a number after *");
	consume(FLUFF);
	return repetitions;
}




EventPtr readAndCreateEvent(AldaStringReader& s)
{
	char c = s.peek();
	if (('a' <= c && c <= 'g') || c == 'r')
	{
		return std::make_shared<ChordEvent>(s);
	}

	switch (s.advance())
	{
	case '(': return std::make_shared<AttributeEvent>(s);
	case '%': return std::make_shared<MarkerEvent>(s, /*definition*/ true);
	case '@': return std::make_shared<MarkerEvent>(s);
    case '>': return std::make_shared<OctaveChangeEvent>(s, 1);
	case '<': return std::make_shared<OctaveChangeEvent>(s, -1);
    case 'o': return std::make_shared<OctaveChangeEvent>(s);
	case '[': return std::make_shared<SequenceEvent>(s);
	case '{': return std::make_shared<SequenceEvent>(s, /*cram*/ true);
	case 'v': return std::make_shared<VoiceChangeEvent>(s);
	default: throw ParserError(s.pos() + "unexpected character: " + c);
	}
}




void Event::repeatedEval(Score& score, Part& part, int currentRepetition)
{
	for (int i = 1; i <= m_repeats; i++)
	{
		if (inRange(i, m_ranges))
		{
			eval(score, part, currentRepetition);
		}
	}
}




AttributeEvent::AttributeEvent(AldaStringReader& s)
{
	m_name = s.readString(ATTRIBUTE, "expected an attribute name inside ()");
	s.consume(FLUFF);

	// TODO
}




void AttributeEvent::eval(Score& score, Part& part, int currentRepetition)
{
	QString word;


	if (word == "tempo")
	{
		//tempo(float denom, float bpm)
		//tempo(string duration, float bpm)
	}
	else if (word == "")
	{
		//metric-modulation(float|string denomOrDuration, float|string denomOrDuration)
	}
	else if (word == "")
	{
		//quantization|quantize|quant(float percentage)
	}
	else if (word == "")
	{
		//volume|vol(float percentage)
	}
	else if (word == "")
	{
		//track-volume|track-vol(float percentage)
	}
	else if (word == "")
	{
	  // "pppppp": 0.00787, // 1
	  // "ppppp":  0.08419, // 11
	  // "pppp":   0.16051, // 20
	  // "ppp":    0.23683, // 30
	  // "pp":     0.31314, // 40
	  // "p":      0.38946, // 49
	  // "mp":     0.46578, // 59
	  // "mf":     0.54210, // 69
	  // "f":      0.61841, // 79
	  // "ff":     0.69473, // 88
	  // "fff":    0.77105, // 98
	  // "ffff":   0.84737, // 108
	  // "fffff":  0.92368, // 117
	  // "ffffff": 1.00000, // 127
	}
	else if (word == "")
	{
		//panning|pan(float percentage)
	}
	else if (word == "")
	{
		//set-duration(float beats | float ms)
	}
	else if (word == "")
	{
		// set-note-length(float denom | string duration)
	}
	else if (word == "")
	{
		// key-signature|key-sig(string | list)
	}
	else if (word == "")
	{
		//transposition|transpose(int semitones)
	}
	else if (word == "")
	{
		// reference-pitch|tuning-constant(float freq)
	}
}




ChordEvent::ChordEvent(AldaStringReader& s)
{
	while (!s.reachedEOF())
	{
		m_children.push_back(std::make_shared<NoteOrRestEvent>(s));

		if (!s.consume('/')) { break; }
		s.consume(FLUFF);

		// As an extension to alda: set the length of a chord at the end using c/e/g/4
		if (s.isDigit())
		{
			m_duration = s.readDuration();
		}
	}
	m_repeats = s.readRepetitions();
}




void ChordEvent::eval(Score& score, Part& part, int currentRepetition)
{
	// Set default note duration according to non-standard c/e/g/4 syntax
	if (m_duration > 0)
	{
		part.currentVoice->defaultDuration = m_duration;
	}

	float startOfChord = part.currentVoice->pos;

	for (auto& child: m_children)
	{
		// Move back to the start before each note
		part.currentVoice->pos = startOfChord;
		child->repeatedEval(score, part, currentRepetition);
	}
}




MarkerEvent::MarkerEvent(AldaStringReader& s, bool isDefinition)
{
	m_isDefinition = isDefinition;
	m_name = s.readString(NAME, "expected a name after % or @");
	s.consume(FLUFF);
}




void MarkerEvent::eval(Score& score, Part& part, int)
{
	if (m_isDefinition)
	{
		score.markers[m_name] = part.currentVoice->pos;
	}
	else
	{
		part.currentVoice->pos = score.markers[m_name];
	}
}




NoteOrRestEvent::NoteOrRestEvent(AldaStringReader& s)
{
	char letter = s.advance();
	m_noteNumber = noteNumber(letter);

	if (m_noteNumber < 0 && letter != 'r') { throw ParserError("expected note letter or rest"); }

	s.consume(FLUFF);

    if (s.isDigit())
	{
		m_duration = s.readDuration();
	}

    if (s.consume('~'))
    {
        m_slur = true;
        s.consume(FLUFF);
    }

    m_repeats = s.readRepetitions();
}




void NoteOrRestEvent::eval(Score& score, Part& part, int)
{
	Voice* v = part.currentVoice;
	float fullDuration = m_duration >= 0 ? m_duration : v->defaultDuration;

	if (m_noteNumber < 0)
	{
		v->pos += fullDuration;
		return;
	}

	int audibleDuration = toTicks(fullDuration * v->gate);
	if (audibleDuration <= 0)
	{
		score.warnings.append("skipping zero-length note");
		return;
	}

	TimePos pos = toTicks(v->pos);

	int key = v->keySig[m_noteNumber] + v->octave * KeysPerOctave;
	if (key < 0 || key >= NumKeys)
	{
		score.warnings.append("key out of range: " + key);
		return;
	}

	part.notes.emplace_back(audibleDuration, pos, key, v->volume, v->panning);

	v->pos += fullDuration;
}




OctaveChangeEvent::OctaveChangeEvent(AldaStringReader& s, int relativeChange)
{
	if (relativeChange != 0)
	{
		m_value = relativeChange;
		m_relative = true;
	}
	else
	{
		s.consume(FLUFF);
		m_value = s.readInt("expected number after o");
	}
	s.consume(FLUFF);
}



void OctaveChangeEvent::eval(Score&, Part& part, int)
{
	if (m_relative)
	{
		part.currentVoice->octave += m_value;
	}
	else
	{
		part.currentVoice->octave = m_value;
	}
}


SequenceEvent::SequenceEvent(AldaStringReader& s, bool cram)
{
	m_cram = cram;

	while (!s.consume(cram ? '}' : ']'))
	{
		s.consume(FLUFF);
		m_children.push_back(readAndCreateEvent(s));
	}

	s.consume(FLUFF);
	if (cram && s.isDigit())
	{
		m_cramDuration = s.readDuration();
		s.consume(FLUFF);
	}

	m_repeats = s.readRepetitions();
}




void SequenceEvent::eval(Score& score, Part& part, int currentRepetition)
{
	// TODO create separate CramEvent
	if (!m_cram)
	{
		for (auto& child: m_children)
		{
			child->repeatedEval(score, part, currentRepetition);
		}
		return;
	}

	float wantedDuration = m_cramDuration > 0 ? m_cramDuration : part.currentVoice->defaultDuration;
	float startOfCram = part.currentVoice->pos;

	std::vector<Note> cramNotes;

	std::swap(cramNotes, part.notes);
	for (auto& child: m_children)
	{
		child->repeatedEval(score, part, currentRepetition);
	}
	std::swap(cramNotes, part.notes);

	float endOfCram = part.currentVoice->pos;
	float actualDuration = endOfCram - startOfCram;
	float scaleFactor = wantedDuration / actualDuration;

	 // Move the time cursor to the expected position
	part.currentVoice->pos = startOfCram + wantedDuration;
	part.currentVoice->defaultDuration = wantedDuration;

	// Resize all notes to match the wanted duration,
	// and move them into the regular note vector
	TimePos cramStartPos = toTicks(startOfCram);
	for (auto& note : cramNotes)
	{
		float noteStart = std::round((note.pos() - cramStartPos) * scaleFactor + cramStartPos);
		float end = std::round((note.endPos() - cramStartPos) * scaleFactor + cramStartPos);
		note.setPos(noteStart);
		note.setLength(end - noteStart);
		part.notes.push_back(std::move(note));
	}

}





VariableEvent::VariableEvent(AldaStringReader& s)
{
	m_name = s.readString(NAME);
	s.consume(FLUFF);
	if (s.consume('='))
	{

		s.consume(FLUFF);
		int lineOfDefinition = s.line();
		while (s.line() == lineOfDefinition)
		{
			m_children.push_back(readAndCreateEvent(s));
			s.consume(FLUFF);
		}
		if (m_children.empty()) { throw ParserError(s.pos() + "expected something after ="); }
	}
	m_repeats = s.readRepetitions();
}



void VariableEvent::eval(Score& score, Part& part, int currentRepetition)
{
	// Definition
	if (!m_children.empty())
	{
		score.variables[m_name] = m_children;
		return;
	}

	// Expansion
	if (!score.variables.contains(m_name))
	{
		score.warnings.append("no variable called " + m_name);
		return;
	}
	for (auto& child: score.variables[m_name])
	{
		child->repeatedEval(score, part, currentRepetition);
	}
}



VoiceChangeEvent::VoiceChangeEvent(AldaStringReader& s)
{
	m_number = s.readInt("expected number after V");
	if (!s.consume(':')) { throw ParserError(s.pos() + "expected : after voice number"); }
	s.consume(FLUFF);
}



void VoiceChangeEvent::eval(Score& score, Part& part, int currentRepetition)
{
	if (!part.voices.contains(m_number))
	{
		part.voices[m_number] = Voice{part.voices[0]}; // copy values from V0
	}

	// Moving to 0 from another voice moves the cursor to the last voice to finish
	if (m_number == 0 && part.currentVoice != &part.voices[0])
	{
		// Get the voice with the largest pos, or the one with the highest ID number
		// Alda works by using the larges pos, or last created voice (which has the highest number presumably)
		auto lastVoiceToFinish = std::max_element(part.voices.begin(), part.voices.end(),
			[](auto& v1, auto& v2)
			{
				return std::tie(v1.second.pos, v1.first) < std::tie(v2.second.pos, v2.first);
			}
			);

		part.voices[0] = lastVoiceToFinish->second;
	}

	part.currentVoice = &part.voices[m_number];
}






std::vector<Part*> AldaParser::parsePartDefinition()
{
	StringReader& s = m_stringReader;

	//
	// Step 1: read names separated by /
	//

	QStringList names;
	while (!s.reachedEOF())
	{
		auto nameOrAlias = s.readString(NAME, "expected instrument name");

		std::vector<QString> trueNames {nameOrAlias};

		// If the name is a known alias, expand it to a list of true names
		if (m_score.aliases.contains(nameOrAlias))
		{
			trueNames = std::vector<QString>(m_score.aliases.at(nameOrAlias));
		}

		// Append names but skip duplicates
		for (const auto& trueName: trueNames)
		{
			if (!names.contains(trueName))
			{
				names.push_back(trueName);
			}
		}
		s.consume(FLUFF);
		if (!s.consume('/')) { break; }
	}
	s.consume(FLUFF);

	//
	// Step 2: save an alias for this collection of names
	//

	if (s.consume('"'))
	{
		auto alias = s.readString(NAME, "expected instrument alias");
		auto& trueNames = m_score.aliases[alias];
		trueNames.insert(trueNames.end(), names.begin(), names.end());
		s.consume('"');
	}
	s.consume(FLUFF);

	if (!s.consume(':')) { throw ParserError(s.pos() + "missing : after part definition"); }
	s.consume(FLUFF);

	//
	// Step 3: create Parts for each name if needed
	//
	std::vector<Part*> parts;
	for (const auto& name: names)
	{
		parts.push_back(&m_score.parts[name]);
	}

	return parts;
}




InstrumentTrack* createTrackForPart(const QString& partName)
{
	// TODO error message for this
	if (!SOUNDFONT_PATCH_NAMES.contains(partName)) { throw ParserError("invalid instrument name: " + partName); }

	int patch = SOUNDFONT_PATCH_NAMES.contains(partName) ? SOUNDFONT_PATCH_NAMES.at(partName) : 0;

	auto track = new InstrumentTrack(Engine::getSong());
	track->loadInstrument("sf2player");

	QDomDocument preset;
	auto element = preset.createElement("sf2player"); // name is unimportant
	element.setAttribute("bank", 0);
	element.setAttribute("patch", patch);
	element.setAttribute("src", "soundfonts/SGM-V2.01.sf2");
	track->instrument()->restoreState(element);
	track->setName(partName);

	return track;
}




MidiClip* getClipForPart(const QString& partName, bool createMissing)
{
	// Return the clip at the beginning of the first track named partName
	for (Track* track : Engine::getSong()->tracks())
	{
		if (track->type() != Track::Type::Instrument || track->name() != partName) { continue; }
		if (track->numOfClips() == 0 && !createMissing) { return nullptr; }
		auto instrumentTrack = static_cast<InstrumentTrack*>(track);
		for (Clip* clip: track->getClips())
		{
			if (clip->startPosition() == 0)
			{
				return static_cast<MidiClip*>(clip);
			}
		}
		return createMissing ? new MidiClip(instrumentTrack) : nullptr;
	}

	// No track found with that name, create one with and add a clip
	if (createMissing)
	{
		if (auto track = createTrackForPart(partName))
		{
			return new MidiClip(track);
		}
	}
	return nullptr;
}




void AldaParser::parse(const QString& string)
{
	AldaStringReader& s = m_stringReader;
	m_stringReader = {string};
	m_score = {};

	// Create the default part (null-named) and set it to be the one we're currently writing to
	std::vector<Part*> currentParts = { &m_score.parts[QString()] };

	// Step 1: Parse the string and create Events.
	// Each event gets added to one or more Parts of the Score, depending on the part definition.
	// The hierarchy of Events pretty much correspond to what the user wrote. No repetition or
	// syncing happens in this step.

	s.consume(FLUFF);
	while (!s.reachedEOF())
	{
		if (s.matches(START_OF_PART))
		{
			currentParts = parsePartDefinition();
			if (currentParts.empty()) { throw ParserError("unknown error during part def parsing"); }
			continue;
		}
		for (Part* part: currentParts)
		{
			part->events.push_back(readAndCreateEvent(s));
		}
	}

	// Step 2: Evaluate all events.
	// This is done by the Event subclasses, and they do whatever they want to the Score.
	// The interesting result is that they can add Notes to each Part, but to keep track
	// of what octave and time position, they rely on the values found in each Voice

	for (auto& [partName, part]: m_score.parts)
	{
		for (EventPtr event: part.events)
		{
			event->repeatedEval(m_score, part, 1);
		}
	}
}




bool AldaParser::isSafeToWrite(const MidiClip* clipInPianoRoll, const std::vector<const Model*>& previouslyChanged)
{
	// TODO
	return true;
}




std::vector<const Model*> AldaParser::write(MidiClip* clipInPianoRoll)
{
	std::vector<const Model*> changedClips;

	for (const auto& [name, part]: m_score.parts)
	{
		if (part.notes.empty()) { continue; }

		auto clip = name.isNull()
			? clipInPianoRoll
			: getClipForPart(name, /*createMissing*/ true);

		if (clip == nullptr)
		{
			m_score.warnings.append("no clip to write to, skipping part " + name);
			continue;
		}


		writeNotesToMidiClip(part.notes, clip);
		changedClips.push_back(clip);
	}

	return changedClips;
}

}
