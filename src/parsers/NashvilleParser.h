/**
 * NashvilleParser.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

#include <QList>
#include <QString>


namespace lmms::gui::editor::pianoroll::parsing
{

    class NashvilleParser: public AbstractParser
    {
        public:
            virtual void setup() override;
            QList<NotationCell *> parse(QString text) override;
    };

}