/**
 * Utilities.cpp - file handle utilities
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include "Utilities.h"

#include <QString>
#include <QIODevice>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QMimeData>
#include <QFileInfo>

#include "MidiClip.h"


namespace lmms::PLUGIN_NAME
{
    /**
     * Return the melody notations text from external file.
     */
    QString fileContents(QString filePath)
    {
        QString notations = "";

        QFile file(filePath);
        //if(file.exists()) // @todo Check if this is applied already.
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            
            // Preferably ASCII Contents only.
            // However, Devangarari Scripts based Notations are also tested.
            notations = in.readAll();
            
            file.close();
        }

        return notations;
    }


    /**
     * Do NOT process too large files.
     */
    bool sizeCheck(QString filePath)
    {
        QFile file(filePath);
        return file.size() <= MAX_INPUT_LIMIT;
	}


    QString pathFromMimeData(const QMimeData* mimeData)
    {
        for (const QUrl& url : mimeData->urls())
        {
            if (!url.isLocalFile()) { continue; }
            QString path = url.toLocalFile();
            if (QFileInfo(path).suffix().toLower() == MELODY_EXTENSION)
            {
                return path;
            }
        }
        return "";
    }

	void writeNotesToMidiClip(const std::vector<Note>& notes, MidiClip* clip)
	{
		if (!clip) return;

		clip->setJournalling(false);
		clip->clear();
		for (const auto& note: notes)
		{
			clip->addNote(note, /*quantize*/ false);
		}
		clip->setJournalling(true);
	}
}