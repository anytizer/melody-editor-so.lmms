/**
 * EnglishParser.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ENGLISHPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_ENGLISHPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    class EnglishParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_ENGLISHPARSER_H