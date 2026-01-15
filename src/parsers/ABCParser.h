/**
 * ABCParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ABCPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_ABCPARSER_H

#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    /**
     * ABC Notations Parser
     */
    class ABCParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_ABCPARSER_H