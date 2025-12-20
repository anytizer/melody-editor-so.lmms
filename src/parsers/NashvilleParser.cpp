/**
 * NashvilleParser.cpp
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */
#ifndef LMMS_PLUGIN_MELODYEDITOR_NASHVILLEPARSER_CPP
#define LMMS_PLUGIN_MELODYEDITOR_NASHVILLEPARSER_CPP

#include "../includes/DataStructures.h"
#include "NashvilleParser.h"

#include <QList>

using lmms::gui::editor::pianoroll::parsing::NotationCell;

namespace lmms::gui::editor::pianoroll::parsing
{
    void NashvilleParser::setup()
    {
        this->_name = "Numeric System";
        this->_identifier = "nashville";
    };

    QList<NotationCell *> NashvilleParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        QList<NotationCell *> cells = {};
        // NotationCell *cell = new NotationCell();
        // cells.append(cell);
        return cells;
    }
}

#endif // LMMS_PLUGIN_MELODYEDITOR_NASHVILLEPARSER_CPP
