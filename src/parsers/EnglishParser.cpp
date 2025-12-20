/**
 * EnglishParser.cpp
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGIN_MELODYEDITOR_ENGLISHPARSER_CPP
#define LMMS_PLUGIN_MELODYEDITOR_ENGLISHPARSER_CPP

#include "../includes/DataStructures.h"
#include "AbstractParser.h"
#include "EnglishParser.h"

#include <QList>

using lmms::gui::editor::pianoroll::parsing::NotationCell;

namespace lmms::gui::editor::pianoroll::parsing
{
    void EnglishParser::setup()
    {
        this->_name = "Western / English";
        this->_identifier = "english";
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
    QList<NotationCell *> EnglishParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        QString un = this->replace(text).toUpper(); // English notes should be capitalized
        un.replace("-1", "....."); // @todo: Possibly does not support | requires testing
        un.replace("0", "....");
        un.replace("1", "...");
        un.replace("2", "..");
        un.replace("3", ".");
        un.replace("4", ""); // Octave marker is optional for 4th octave notes
        un.replace("5", "*");
        un.replace("6", "**");
        un.replace("7", "***");
        un.replace("8", "****");
        un.replace("9", "*****");

        int errors = 0;
        int position = 0;
        QList<NotationCell *> cells = {};
        QStringList blocks = un.split("#//");
        for (const QString& block : blocks)
        {
            errors += this->process_block(block, cells, position);
        }

        return cells;
    }
}

#endif // LMMS_PLUGIN_MELODYEDITOR_ENGLISHPARSER_CPP