/**
 * NoneParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_NONEPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_NONEPARSER_H

#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    /**
     * Purpose: To serve as a dummy parser at 0-th position
     * Status: Always un-selected.
     * Lets the user choose other valid parsers.
     */
    class NoneParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_NONEPARSER_H