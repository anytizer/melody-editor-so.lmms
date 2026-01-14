/**
 * MissingParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_MISSINGPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_MISSINGPARSER_H

#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    /**
     * Purpose: To serve as a dummy parser where none of the real ones were registered.
     */
    class MissingParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_MISSINGPARSER_H