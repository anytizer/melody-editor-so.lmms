/**
 * ALDAParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ALDAPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_ALDAPARSER_H

#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    /**
     * ALDA Notations Parser
     */
    class ALDAParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_ALDAPARSER_H