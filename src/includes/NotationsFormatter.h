/**
 * NotationsFormatter.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

 #pragma once

 #include <QString>

namespace lmms::melodyeditor
{
    class NotationsFormatter
    {
        private:
            int width = 6; // re-calculated during phase #1 of formatting
        
        public:
            QString format(QString notations, bool keepSpecialComments);
    };
}