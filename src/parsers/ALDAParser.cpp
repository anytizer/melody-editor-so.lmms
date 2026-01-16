/**
 * ALDAParser.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ALDAPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_ALDAPARSER_CPP

#include "AbstractParser.h"
#include "ALDAParser.h"

#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{
    void ALDAParser::setup()
    {
        this->_name = "ALDA Parser";
        this->_identifier = "alda";
        this->_completed = true;
    };

    QList<NotationCell *> ALDAParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        // @todo HELP WANTED to complete this parser.
        // @see https://alda.io/tutorial/
        // @see https://github.com/alda-lang/alda/tree/master/doc/

        QList<NotationCell *> cells = {};
        NotationCell *cell = new NotationCell();
        cells.append(cell);
        return cells;
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_ALDAPARSER_CPP