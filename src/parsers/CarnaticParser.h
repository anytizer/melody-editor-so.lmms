/**
 * CarnaticParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_EDITOR_PIANOROLL_PARSING_CARNATICPARSER_H
#define LMMS_GUI_EDITOR_PIANOROLL_PARSING_CARNATICPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

#include <QMap>
#include <QString>

using lmms::gui::editor::pianoroll::parsing::NotationCell;

namespace lmms::gui::editor::pianoroll::parsing
{

class CarnaticParser: public AbstractParser
{
    public:
        // Where can I find some notations?
        // https://carnaticnotations.blogspot.com/
        // https://www.shivkumar.org/music/index.html
        // https://www.karnatik.com/beginners.pdf

        // Some videos explaining how to write the notes
        // https://www.youtube.com/watch?v=Z0XXbU3NyLY
        // https://www.youtube.com/watch?v=kLopt9b66Dc

        // Educational Resources
        // https://ccrma.stanford.edu/~arvindh/cmt/the_12_notes.html
        // https://en.wikipedia.org/wiki/Melakarta
        // https://en.wikipedia.org/wiki/Kriti_(music)
        // https://en.wikipedia.org/wiki/Carnatic_music
        // https://www.melakarta.com/education.html
        const QMap<QString, QString> mapping = {
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
        };
        virtual void setup() override;        
        QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_GUI_EDITOR_PIANOROLL_PARSING_CARNATICPARSER_H