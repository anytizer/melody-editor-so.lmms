/**
 * NotationsFormatter.cpp - Extracts just notations in easy-to-view columns
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include <QRegularExpression>

#include "NotationsFormatter.h"

namespace lmms::PLUGIN_NAME
{
    QString NotationsFormatter::format(QString notations)
    {
        // let us NOT support these characters
        notations.replace("\r", "");

        // trial status "true"
        bool keepComments = false;

        // best range: 1 to 6.
        // 4/4 time signature * width =
        // 4 x 4 x 6 = 96 characters wide per line | can fit well on maximized window
        int max_width = 3;
        std::vector<QStringList>* matrix = new std::vector<QStringList>{};

        // first part splits, prepares matrix and analyses the column's width required

        QStringList lines = notations.split(QRegularExpression("[\\n]"), Qt::KeepEmptyParts);
        int totalLines = lines.count();
        for (const QString &line : lines)
        {
            QStringList columns = {};
            if(!line.startsWith("#"))
            {
                columns = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                //for (const QString &column : columns)
                int totalColumns = columns.count();
                for(int c=0; c<totalColumns; ++c)
                {
                    QString column = columns[c];

                    // do not show bar lines
                    if(column=="|" || column=="/")
                    {
                        columns.removeAt(c);
                        continue;
                    }

                    if(column.length()>max_width)
                    {
                        max_width = column.length();
                    }
                }
            }
            else
            {
                // Use a flag to keep comments.
                // untouched commented lines
                if(keepComments) columns.push_back(line);
            }

            matrix->push_back(columns);
        }
        
        max_width += 2; //  cusioning space: +2
        int width = std::clamp(max_width, 3, 8);
        char padWith = 0x20; // [SPACE] character
        
        // second part joins the notations in a text block

        int currentLine = 0;
        QString formattedNotations = "";
        for (const QStringList &row : *matrix)
        {
            for (const QString &col : row)
            {
                formattedNotations += col.left(width).leftJustified(width, padWith);
            }

            // @todo do not apply the \n at the end of file
            // remove the ending spaces in the last column (shrinks)
            formattedNotations = formattedNotations.trimmed();
            if(++currentLine<totalLines)
            {
                formattedNotations += "\n";
            }
        }

        return formattedNotations;
    }
}