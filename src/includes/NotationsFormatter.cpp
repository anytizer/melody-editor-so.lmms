/**
 * NotationsFormatter.cpp - Extracts just notations in easy-to-view columns.
 *                        - Comments are stripped out.
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

        // best range: 1 to 6.
        // 4/4 time signature * width = 4 x 4 x 6 = 96 characters wide per line
        // can fit well on maximized window
        int max_width = 3;

        QVector<QStringList> matrix;

        // first part of the loop splits, prepares matrix and analyses the column's width required

        QStringList lines = notations.split(QRegularExpression("[\\n]"), Qt::SkipEmptyParts);
        for (const QString &line : lines)
        {
            QStringList processedColumns;
            if(!line.startsWith("#"))
            {
                QStringList columns = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                int totalColumns = columns.count();
                for(int c=0; c<totalColumns; ++c)
                {
                    QString column = columns[c];

                    // do not show bar lines
                    if(column=="|" || column=="/")
                        continue;

                    if(column.length()>max_width)
                    {
                        max_width = column.length();
                    }
                    
                    processedColumns.append(column);
                }
                
                matrix.append(processedColumns);
            }
        }
        
        max_width += 2; //  cusioning space: +2
        int width = std::clamp(max_width, 3, 8);
        char padWith = 0x20; // [SPACE] character
        
        // second part of the similar loop joins the notations in a text block
        // without trailing \n at the end
        // and without padwidth in the last column

        QStringList outputBuffer;
        for (const QStringList &row : matrix)
        {
            QStringList formattedColumns;
            for (const QString &col : row)
            {
                formattedColumns.append(col.left(width).leftJustified(width, padWith));
            }

            // remove the ending spaces in the last column
            outputBuffer.append(formattedColumns.join("").trimmed());
        }
        
        return outputBuffer.join("\n");
    }
}