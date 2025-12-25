/**
 * CarnaticParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
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