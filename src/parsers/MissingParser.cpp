/**
 * MissingParser.cpp
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_MISSINGPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_MISSINGPARSER_CPP

#include "AbstractParser.h"
#include "MissingParser.h"

#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{
    void MissingParser::setup()
    {
        this->_name = "Missing Parser";
        this->_identifier = "missing";
    };

    QList<NotationCell *> MissingParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        QList<NotationCell *> cells = {};
        NotationCell *cell = new NotationCell();
        cells.append(cell);
        return cells;
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_MISSINGPARSER_CPP