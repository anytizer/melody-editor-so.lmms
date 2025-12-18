/**
 * ParserFactory.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMG_GUI_EDITOR_PIANOROLL_PARSING_PARSERFACTORY_H
#define LMMG_GUI_EDITOR_PIANOROLL_PARSING_PARSERFACTORY_H


#include "./AbstractParser.h"

#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{

    class ParsersFactory
    {
        public:
            ParsersFactory();
            ~ParsersFactory();

            QList<AbstractParser *> parsers = QList<AbstractParser *>(); // {};
    };

}

#endif // LMMG_GUI_EDITOR_PIANOROLL_PARSING_PARSERFACTORY_H