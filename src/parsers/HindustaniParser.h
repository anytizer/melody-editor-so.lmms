/**
 * HindustaniParser.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_HINDUSTANITPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_HINDUSTANITPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{
    class HindustaniParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };
}

#endif // LMMS_PLUGINS_MELODYEDITOR_HINDUSTANITPARSER_H