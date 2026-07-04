/**
 * NotationsFormatter.cpp - Extracts just the notations in easy-to-view columns.
 *                        - Comments are stripped out.
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 * 
 * Notes: Code enhanced with AI
 */

#include <algorithm> // for std::clamp

#include <QStringList>
#include <QRegularExpression>

#include "NotationsFormatter.h"

namespace lmms::melodyeditor
{
    namespace
    {
        struct Row
        {
            QStringList columns;
            bool isSpecialComment = false;
        };

        constexpr int kMinWidth = 3;
        constexpr int kMaxWidth = 8;
        constexpr int kPadding  = 2;
    }

    QString NotationsFormatter::format(QString notations, bool keepSpecialComments)
    {
        notations.remove('\r'); // Normalize line endings

        int maxColumnWidth = kMinWidth;
        QVector<Row> rows;

        const QStringList lines = notations.split('\n', Qt::SkipEmptyParts);
        rows.reserve(lines.size());

        // -------- First pass: parse + measure --------
        for (const QString &line : lines)
        {
            // Handle comments
            if (line.startsWith('#'))
            {
                if (keepSpecialComments && line.startsWith("#!"))
                {
                    rows.append(Row{{line}, true});
                }
                continue;
            }

            const QStringList columns = line.split(QRegularExpression("\\s+"),
                                                   Qt::SkipEmptyParts);

            QStringList processed;
            processed.reserve(columns.size());

            for (const QString &column : columns)
            {
                // Skip bar markers
                if (column == "|" || column == "/")
                    continue;

                maxColumnWidth = qMax(maxColumnWidth, column.size());
                processed.append(column);
            }

            if (!processed.isEmpty())
            {
                rows.append(Row{processed, false});
            }
        }

        const int finalWidth =
            std::clamp(maxColumnWidth + kPadding, kMinWidth, kMaxWidth);

        const QChar padChar = u' ';

        QStringList output;
        output.reserve(rows.size());

        // -------- Second pass: format --------
        for (const Row &row : rows)
        {
            if (row.isSpecialComment)
            {
                output.append(row.columns.first());
                continue;
            }

            QString line;
            line.reserve(row.columns.size() * finalWidth);

            const int last = row.columns.size();
            for (int i = 0; i < last; ++i)
            {
                QString col = row.columns[i];

                if (col.size() > finalWidth)
                    col.truncate(finalWidth);

                // Do not pad the last column
                if (i == last -1)
                {
                    line += col;
                }
                else
                {
                    line += col.leftJustified(finalWidth, padChar);
                }
            }

            output.append(line);
        }

        return output.join('\n');
    }
}