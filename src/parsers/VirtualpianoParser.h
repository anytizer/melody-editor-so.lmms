/**
 * VirtualpianoParser.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_VIRTUALPIANOPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_VIRTUALPIANOPARSER_H

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

#include <QList>
#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{
    class VirtualpianoParser: public AbstractParser
    {
        private:
            QList<FindAndReplace *> vkr = QList<FindAndReplace *>();

        public:
            void setup() override;            
            QList<NotationCell *> parse(QString text) override;
    };
}

#endif // LMMS_PLUGINS_MELODYEDITOR_VIRTUALPIANOPARSER_H