/**
 * ABCParser.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ABCPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_ABCPARSER_CPP

#include "AbstractParser.h"
#include "ABCParser.h"

#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{
    void ABCParser::setup()
    {
        this->_name = "ABC Notation Parser";
        this->_identifier = "abc";
    };

    QList<NotationCell *> ABCParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        // @todo HELP WANTED to complete this parser.

        QList<NotationCell *> cells = {};
        NotationCell *cell = new NotationCell();
        cells.append(cell);
        return cells;
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_ABCPARSER_CPP