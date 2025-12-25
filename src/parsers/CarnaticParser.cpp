/**
 * CarnaticParser.cpp
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_CARNATICPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_CARNATICPARSER_CPP

#include "../includes/DataStructures.h"
#include "AbstractParser.h"
#include "CarnaticParser.h"

#include <QList>
#include <QMap>
#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{
    void CarnaticParser::setup()
    {
        this->_name = "SARGAM / Carnatic";
        this->_identifier = "carnatic";

        this->replacements.append(new FindAndReplace());

        this->replacements.append(new FindAndReplace("₁", "1"));
        this->replacements.append(new FindAndReplace("₂", "2"));
        this->replacements.append(new FindAndReplace("₃", "3"));
        this->replacements.append(new FindAndReplace("₁", "1")); // different set of sub-numbering?
        this->replacements.append(new FindAndReplace("₂", "2")); // different set of sub-numbering?
        this->replacements.append(new FindAndReplace("₃", "3")); // different set of sub-numbering?
        this->replacements.append(new FindAndReplace("SA", "S"));
        this->replacements.append(new FindAndReplace("RE", "R"));
        this->replacements.append(new FindAndReplace("RI", "R"));
        this->replacements.append(new FindAndReplace("GA", "G"));
        this->replacements.append(new FindAndReplace("MA", "M"));
        this->replacements.append(new FindAndReplace("PA", "P"));
        this->replacements.append(new FindAndReplace("DHA", "D"));
        this->replacements.append(new FindAndReplace("DA", "D"));
        this->replacements.append(new FindAndReplace("NI", "N"));
        this->replacements.append(new FindAndReplace("Ṡ", "S*"));
        this->replacements.append(new FindAndReplace("Ṡ", "S*"));
        this->replacements.append(new FindAndReplace("’", "*"));

    };
    
    QList<NotationCell *> CarnaticParser::parse(QString text)
    {
        if(text.length()<=0) return {};
        
        QString notes = this->replace(text); // .toUpper();

        // for (const QString& key : mapping.keys()) {
        //     QString value = mapping[key];
        //     notes.replace(key, value);
        // }

        // S R₁ G₁ M₁ P D₁ N₁ Ṡ
        // S R₂ G₂ M₁ P D₁ N₃ Ṡ
        // S R₃ G₃ M₁ P D₃ N₃ Ṡ
        // notes.replace("s", "C");
        // notes.replace("Ṡ", "C*");
        // notes.replace("P", "G");
        // notes.replace("P", "G");

        QStringList blocks = notes.split("#//");
        
        int errors = 0;
        int position = 0;        
        QList<NotationCell *> cells = {};
        for (const QString& block : blocks)
        {
            errors += process_block(block, cells, position);
        }
        
        return cells;
    }
}


#endif // LMMS_PLUGINS_MELODYEDITOR_CARNATICPARSER_CPP