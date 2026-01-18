/**
 * AbstractParser.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ABSTRACTPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_ABSTRACTPARSER_H

#include "../includes/DataStructures.h"

#include <QList>
#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{

/**
 * Info: This is NOT an abstract class!
 * However, the objects should not be created directly.
 */
class AbstractParser
{
    protected:
        QString _name = "";
        QString _identifier = ""; // lower cased, one-word

        QList<FindAndReplace *> replacements = QList<FindAndReplace *>();
        //QString replace_symbols(QString text);

        bool chord_processing = false;
        int chord_start_position = 0;
        //QList<FindAndReplace *> *replaces = new QList<FindAndReplace *>();
        QList<QString> notes = {}; // pure Western/English names; only with sharps | no flats.
        QList<NotationCell *> chords = {};

        /**
         * Processing return number of errors.
         */
        int process_block(const QString block, QList<NotationCell *> &cells, int &position);
        int process_line(const QString line, QList<NotationCell *> &cells, int &position);
        int process_beatnotes(const QString column, QList<NotationCell *> &cells, int &position);
        int process_tone(const QString tone, QList<NotationCell *> &cells, float length, int &position);

        QString replace(QString text);
    
    public:
        AbstractParser();
        ~AbstractParser();
        virtual void setup() = 0;
        virtual QList<NotationCell *> parse(QString text) = 0;

        const QString name() const
        {
            return this->_name;
        }

        const QString identifier() const
        {
            return this->_identifier;
        }

        const QString toString() const
        {
            return _identifier;
        }

        int getPianoKey(QString note);
        void cells_to_xml(QList<NotationCell *> cells, QString &xml);
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_ABSTRACTPARSER_H