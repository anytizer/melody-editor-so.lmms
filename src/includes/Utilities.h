/**
 * Utilties.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#pragma once

#include <QString>

#include "Note.h"

class QMimeData;

namespace lmms
{
	class MidiClip;
}


namespace lmms::PLUGIN_NAME
{
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
     */
    const QString MELODY_EXTENSION = "txt";

    /**
     * Zoom factors on notations editor
     */
    const int MIN_FONTSIZE = 10; // min: readable font size: 10
    const int MAX_FONTSIZE = 28; // max 36
    const int ZOOM_FACTOR = 90; // 80 to 120 | 120 default

    QString pathFromMimeData(const QMimeData* mimeData);

    //! A semi-fast way to write notes to a MidiClip
    void writeNotesToMidiClip(const std::vector<Note>& notes, MidiClip* clip);
}
