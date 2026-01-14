/**
 * GermanParser.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_GERMANPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_GERMANPARSER_CPP

#include "../includes/DataStructures.h"
#include "AbstractParser.h"
#include "GermanParser.h"

#include <QList>

using lmms::gui::editor::pianoroll::parsing::NotationCell;

namespace lmms::gui::editor::pianoroll::parsing
{
    void GermanParser::setup()
    {
        this->_name = "Western / German";
        this->_identifier = "german";
    };
    
    QList<NotationCell *> GermanParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        // @todo HELP WANTED to complete this parser.

        QList<NotationCell *> cells = {};
        NotationCell *cell = new NotationCell();
        cells.append(cell);
        return cells;
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_GERMANPARSER_CPP