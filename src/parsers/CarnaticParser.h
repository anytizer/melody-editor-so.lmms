/**
 * CarnaticParser.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_CARNATICPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_CARNATICPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

#include <QMap>
#include <QString>

using lmms::gui::editor::pianoroll::parsing::NotationCell;

namespace lmms::gui::editor::pianoroll::parsing
{

    /**
     * Important difference with SARGAM:
     * Shadja/Sa 	S 	C
     * Shuddha Rishabha/Ri 	R1 	C#/Db
     * Chatusruthi Rishabha/Ri 	R2 	D
     * Shatsruthi Rishabha/Ri 	R3 	D#/Eb
     * Suddha Gaandhaara/Ga 	G1 	D
     * Saadharana Gaandhaara/Ga 	G2 	D#/Eb
     * Anthara Gaandhaara/Ga 	G3 	E
     * Shuddha Madhyama/Ma 	M1 	F
     * Prati Madhyama/Ma 	M2 	F#/Gb
     * Panchama/Pa 	P 	G
     * Suddha Dhaivata/Dha 	D1 	G#/Ab
     * Chatsruthi Dhaivata/Dha 	D2 	A
     * Shatsruthi Dhaivata/Dha 	D3 	A#/Bb
     * Shuddha Nishadha/Ni 	N1 	A
     * Kaisiki Nishadha/Ni 	N2 	A#/Bb
     * Kaakali Nishadha/Ni 	N3 	B
     */
    class CarnaticParser: public AbstractParser
    {
        private:
            // @todo Single repository of piano keys
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
        
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };
}

#endif // LMMS_PLUGINS_MELODYEDITOR_CARNATICPARSER_H