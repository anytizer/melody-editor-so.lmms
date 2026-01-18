/**
 * ParserFactory.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_PARSERFACTORY_H
#define LMMS_PLUGINS_MELODYEDITOR_PARSERFACTORY_H

#include "./AbstractParser.h"

#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{

    class ParsersFactory
    {
        private:
            void setup_all_registered_parsers();

        public:
            ParsersFactory();
            ~ParsersFactory();

            QList<AbstractParser *> parsers = QList<AbstractParser *>(); // {};
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_PARSERFACTORY_H