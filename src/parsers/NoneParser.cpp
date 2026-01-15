/**
 * NoneParser.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_NONEPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_NONEPARSER_CPP

#include "AbstractParser.h"
#include "NoneParser.h"

#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{
    void NoneParser::setup()
    {
        this->_name = "Select a Parser";
        this->_identifier = "none";
    };

    QList<NotationCell *> NoneParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        QList<NotationCell *> cells = {};
        NotationCell *cell = new NotationCell();
        cells.append(cell);
        return cells;
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_NONEPARSER_CPP