/**
 * SimpleParserDialects.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef MELODY_EDITOR_SIMPLE_PARSER_DIALECTS_H
#define MELODY_EDITOR_SIMPLE_PARSER_DIALECTS_H

#include <vector>
#include <QString>


namespace lmms::melodyeditor
{


struct Dialect
{
	QString name;
	std::string icon;
	std::vector<std::pair<QString, QString>> replacements;
};

/**
 * Expects that user types notations in Western Notes using plain key names.
 *
 * Western notes already apppear as parser friendly / ready.
 *
 * Allowed letters:
 * eg. C* C** C*** C**** C***** (with higher octaves mark)
 * eg. D. D.. D... D.... (with lower octaves mark)
 * eg. C D E F G A B C# D# F# G# A# (4th octave without number)
 *
 * And with octave numbers as well:
 * eg. C8 D8 E8 F8 G8 A8 B8 C#8 D#8 F#8 G#8 A#8
 * eg. C7 D7 E7 F7 G7 A7 B7 C#7 D#7 F#7 G#7 A#7
 * eg. C6 D6 E6 F6 G6 A6 B6 C#6 D#6 F#6 G#6 A#6
 * eg. C5 D5 E5 F5 G5 A5 B5 C#5 D#5 F#5 G#5 A#5
 * eg. C4 D4 E4 F4 G4 A4 B4 C#4 D#4 F#4 G#4 A#4
 * eg. C3 D3 E3 F3 G3 A3 B3 C#3 D#3 F#3 G#3 A#3
 * eg. C2 D2 E2 F2 G2 A2 B2 C#2 D#2 F#2 G#2 A#2
 * eg. C1 D1 E1 F1 G1 A1 B1 C#1 D#1 F#1 G#1 A#1
 *
 * And the regular symbols: x, -, . (period), COMMA (,)
 *
 * @todo Flats are not parsed.
 * Rather use sharps only.
 */
const Dialect ENGLISH_DIALECT = {
	"Western / English",
	"ns-english",
	{
		{"-1", "....."}, // @todo: Possibly does not support | requires testing
		{"0", "...."},
		{"1", "..."},
		{"2", ".."},
		{"3", "."},
		{"4", ""}, // Octave marker is optional for 4th octave notes
		{"5", "*"},
		{"6", "**"},
		{"7", "***"},
		{"8", "****"},
		{"9", "*****"},
		}
};

const Dialect CARNATIC_DIALECT = {
	"SARGAM / Carnatic",
	"ns-carnatic",
	{
		{"S", "C"},
		{"R1", "C#"},
		{"R2", "D"},
		{"G1", "D"},
		{"R3", "D#"},
		{"G2", "D#"},
		{"G3", "E"},
		{"M1", "F"},
		{"M2", "F#"},
		{"P", "G"},
		{"D1", "G#"},
		{"D2", "A"},
		{"N1", "A"},
		{"D3", "A#"},
		{"N2", "A#"},
		{"N3", "B"},
		{"₁", "1"},
		{"₂", "2"},
		{"₃", "3"},
		{"₁", "1"}, // different set of sub-numbering?
		{"₂", "2"}, // different set of sub-numbering?
		{"₃", "3"}, // different set of sub-numbering?
		{"SA", "S"},
		{"RE", "R"},
		{"RI", "R"},
		{"GA", "G"},
		{"MA", "M"},
		{"PA", "P"},
		{"DHA", "D"},
		{"DA", "D"},
		{"NI", "N"},
		{"Ṡ", "S*"},
		{"Ṡ", "S*"},
		{"’", "*"},
		}
};

/**
 * @todo Reuse data from PianoRoll
 * @todo Use find-replace dataset
 * Usage purpose:
 *  - To validate a user input key.
 *  - To convert a key into MIDI key number
 * 	 * Expects that user types notations in Classical SARGAMS based on Roman letters.
 * 	 * Allowed letters: S, r, R, g, G, m, M, P, d, D, n, N
 * Allowed symbols: x, -, |, *, . (period), , (comma)
 * 	 * @todo Find an alternative way to replace strings
 * @see https://stackoverflow.com/questions/12048256/
 */
const Dialect HINDUSTANI_DIALECT = {
	"SARGAM / Hindustani",
	"ns-hindustani",
	{
		// Replace characters not used in SARGAM
		{"a", ""},
		{"A", ""},
		{"e", ""}, // NOT a key name
		{"E", ""}, // Not a key name
		{"i", ""},
		{"I", ""},
		//notes.replace("h", ""); // Dha => h part
		//notes.replace("ha", ""); // dha
		//notes.replace("HA", ""); // DHA
		{"h", ""}, // dha
		{"H", ""}, // dha
		{"u", ""}, // ru

		{"(K)", "(k)"}, // komal | flat
		// r(k) => R(K) => r

		// to sargam standards - multi letters
		{"s", "S"},
		{"सां", "S*"},
		{"Ṡ", "S*"},
		{"सा", "S"},
		{"स", "S"},
		// some weird notations
		// notes.replace("ष", "S");
		// notes.replace("षा", "S");
		// notes.replace("शा", "S");
		// notes.replace("श", "S");
		////////////////////////
		{"S", "S+"},

		{"रें", "R*"},
		{"रे॒", "r"},
		{"री", "R"},
		{"रि", "R"},
		{"र॓", "R"},
		{"रे", "R"},
		{"र", "R"},
		{"ऋ", "R"},
		{"ॠ", "R"},
		{"R(k)", "r"},
		{"r(k)", "r"},
		{"Ṟ", "r"},
		{"Ṛ", "R."},
		{"Ṟ", "r"},
		{"ṝ", "r"},
		////////////////////////
		{"r", "r+"},
		{"R", "R+"},

		{"G̱", "g"},
		{"ग॒", "g"},
		{"G", "G"},
		{"गा", "G"},
		{"ग", "G"},
		{"गं", "G*"},
		{"G(k)", "g"},
		{"g(k)", "g"},
		////////////////////////
		{"g", "g+"},
		{"G", "G+"},

		{"म'", "M"},
		{"मे", "M"},
		{"म॑", "M"}, // with long vertical bar
		{"म", "m"},
		{"Ḿ", "M"},
		{"M̄", "M"},
		{"m'", "M"},
		{"M'", "M"},
		////////////////////////
		{"m", "m+"},
		{"M", "M+"},

		{"p", "P"},
		{"प़", "P."},
		{"प", "P"},
		{"पा", "P"},
		////////////////////////
		{"P", "P+"},

		{"dha", "d"},
		{"Dha", "D"},
		{"धै", "D"},
		{"ध॒", "d"}, // colliding with D?
		{"ध़", "D."}, // lower octave? or komal?
		{"ध़", "D."}, // lower octave? or komal?
		{"ध", "D"}, // colliding D?
		{"d(k)", "d"},
		{"d(k)", "d"},
		////////////////////////
		{"d", "d+"},
		{"D", "D+"},

		{"Ṉ", "n"},
		{"नि॒", "n"},
		{"नि", "N"},
		{"ऩि", "N."},
		{"ऩ", "n"}, // @todo: NI, nee or ni komal?
		{"नी", "N"},
		{"N(k)", "n"},
		{"n(k)", "n"},
		////////////////////////
		{"n", "n+"},
		{"N", "N+"},

		// convert to western notes
		// + sign is appended to lock replacing other notes containing same letters. eg. RE => D4 => DHA? => A
		{"S+", "C"},
		{"r+", "C#"},
		{"R+", "D"},
		{"g+", "D#"},
		{"G+", "E"},
		{"m+", "F"},
		{"M+", "F#"},
		{"P+", "G"},
		{"d+", "G#"},
		{"D+", "A"},
		{"n+", "A#"},
		{"N+", "B"},
		{"##", "#"}, // MA' => F## => F#
	}
};

}

#endif // MELODY_EDITOR_SIMPLE_PARSER_DIALECTS_H
