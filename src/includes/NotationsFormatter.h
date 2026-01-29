/**
 * NotationsFormatter.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

 #pragma once

 #include <QString>

namespace lmms::PLUGIN_NAME
{
    class NotationsFormatter
    {
        private:
            // re-calculated during formatting
            int width = 6;
        
        public:
            QString format(QString notations);
    };
}