/**
 * Utilties.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_UTILITIES_H
#define LMMS_PLUGINS_MELODYEDITOR_UTILITIES_H

#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{
    class Utilities
    {
        public:
            /**
             * Used to access the embedded resources.
             * The plguin name when compiled.
             * @see CMakeLists.txt | MUST match, as internal identifier.
             */
            const QString identifier = "melodyeditor"; // DO NOT CHANGE!
            const QString DEFAULT_ICON = "english"; // failsafe default icon: hindustani | english
            
            Utilities();
            ~Utilities();

            QString fileContents(QString filePath);
            
            /**
             * To not to allow large file sizes
             */ 
            bool sizeCheck(QString filePath);
        
            /**
             * 20kb notations gives around 2000 to 5000 measures.
             * This is nearly 1 hour long play time, @160 bpm.
             * Processing too large text may cause crash or delay.
             */ 
            const int MAX_INPUT_LIMIT = 20480;

            /**
             * File name extension for dropping one.
             * 
             * Proposals:
             * .lrc, .txt, .melody
             */
            const QString MELODY_EXTENSION = "txt";

            /**
             * Zoom factors on notations editor
             */
            const int MIN_FONTSIZE = 10; // min: readable font size: 10
            const int MAX_FONTSIZE = 28; // max 36
            const int ZOOM_FACTOR = 90; // 80 to 120 | 120 default | font size zoom factor
    };
}

#endif // LMMS_PLUGINS_MELODYEDITOR_UTILITIES_H