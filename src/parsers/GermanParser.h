/**
 * GermanParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_GERMANPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_GERMANPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{

    // https://en.wikipedia.org/wiki/Key_signature_names_and_translations
    class GermanParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_GERMANPARSER_H