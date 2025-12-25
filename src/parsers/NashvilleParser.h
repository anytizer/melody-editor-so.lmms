/**
 * NashvilleParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_NASHVILLEPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_NASHVILLEPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

#include <QList>
#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{

    class NashvilleParser: public AbstractParser
    {
        public:
            void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_NASHVILLEPARSER_H