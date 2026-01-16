/**
 * HindustaniParser.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_HINDUSTANIPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_HINDUSTANIPARSER_CPP

#include "../includes/DataStructures.h"
#include "AbstractParser.h"
#include "HindustaniParser.h"

#include <QStringList>

namespace lmms::gui::editor::pianoroll::parsing
{

    void HindustaniParser::setup()
    {
        this->_name = "SARGAM / Hindustani";
        this->_identifier = "hindustani";
        this->_completed = true;

        // Replace characters not used in SARGAM
        this->replacements.append(new FindAndReplace("a", ""));
        this->replacements.append(new FindAndReplace("A", ""));
        this->replacements.append(new FindAndReplace("e", "")); // NOT a key name
        this->replacements.append(new FindAndReplace("E", "")); // Not a key name
        this->replacements.append(new FindAndReplace("i", ""));
        this->replacements.append(new FindAndReplace("I", ""));
        //notes.replace("h", ""); // Dha => h part
        //notes.replace("ha", ""); // dha
        //notes.replace("HA", ""); // DHA
        this->replacements.append(new FindAndReplace("h", "")); // dha
        this->replacements.append(new FindAndReplace("H", "")); // dha
        this->replacements.append(new FindAndReplace("u", "")); // ru

        this->replacements.append(new FindAndReplace("(K)", "(k)")); // komal | flat
        // r(k) => R(K) => r

        // to sargam standards - multi letters
        this->replacements.append(new FindAndReplace("s", "S"));
        this->replacements.append(new FindAndReplace("सां", "S*"));
        this->replacements.append(new FindAndReplace("Ṡ", "S*"));
        this->replacements.append(new FindAndReplace("सा", "S"));
        this->replacements.append(new FindAndReplace("स", "S"));
        // some weird notations
        // notes.replace("ष", "S");
        // notes.replace("षा", "S");
        // notes.replace("शा", "S");
        // notes.replace("श", "S");
        ////////////////////////
        this->replacements.append(new FindAndReplace("S", "S+"));

        this->replacements.append(new FindAndReplace("रें", "R*"));
        this->replacements.append(new FindAndReplace("रे॒", "r"));
        this->replacements.append(new FindAndReplace("री", "R"));
        this->replacements.append(new FindAndReplace("रि", "R"));
        this->replacements.append(new FindAndReplace("र॓", "R"));
        this->replacements.append(new FindAndReplace("रे", "R"));
        this->replacements.append(new FindAndReplace("र", "R"));
        this->replacements.append(new FindAndReplace("ऋ", "R"));
        this->replacements.append(new FindAndReplace("ॠ", "R"));
        this->replacements.append(new FindAndReplace("R(k)", "r"));
        this->replacements.append(new FindAndReplace("r(k)", "r"));
        this->replacements.append(new FindAndReplace("Ṟ", "r"));
        this->replacements.append(new FindAndReplace("Ṛ", "R."));
        this->replacements.append(new FindAndReplace("Ṟ", "r"));
        this->replacements.append(new FindAndReplace("ṝ", "r"));
        ////////////////////////
        this->replacements.append(new FindAndReplace("r", "r+"));
        this->replacements.append(new FindAndReplace("R", "R+"));
        
        this->replacements.append(new FindAndReplace("G̱", "g"));
        this->replacements.append(new FindAndReplace("ग॒", "g"));
        this->replacements.append(new FindAndReplace("G", "G"));
        this->replacements.append(new FindAndReplace("गा", "G"));
        this->replacements.append(new FindAndReplace("ग", "G"));
        this->replacements.append(new FindAndReplace("गं", "G*"));
        this->replacements.append(new FindAndReplace("G(k)", "g"));
        this->replacements.append(new FindAndReplace("g(k)", "g"));
        ////////////////////////
        this->replacements.append(new FindAndReplace("g", "g+"));
        this->replacements.append(new FindAndReplace("G", "G+"));

        this->replacements.append(new FindAndReplace("म'", "M"));
        this->replacements.append(new FindAndReplace("मे", "M"));
        this->replacements.append(new FindAndReplace("म॑", "M")); // with long vertical bar
        this->replacements.append(new FindAndReplace("म", "m"));
        this->replacements.append(new FindAndReplace("Ḿ", "M"));
        this->replacements.append(new FindAndReplace("M̄", "M"));
        this->replacements.append(new FindAndReplace("m'", "M"));
        this->replacements.append(new FindAndReplace("M'", "M"));
        ////////////////////////
        this->replacements.append(new FindAndReplace("m", "m+"));
        this->replacements.append(new FindAndReplace("M", "M+"));

        this->replacements.append(new FindAndReplace("p", "P"));
        this->replacements.append(new FindAndReplace("प़", "P."));
        this->replacements.append(new FindAndReplace("प", "P"));
        this->replacements.append(new FindAndReplace("पा", "P"));
        ////////////////////////
        this->replacements.append(new FindAndReplace("P", "P+"));

        this->replacements.append(new FindAndReplace("dha", "d"));
        this->replacements.append(new FindAndReplace("Dha", "D"));
        this->replacements.append(new FindAndReplace("धै", "D"));
        this->replacements.append(new FindAndReplace("ध॒", "d")); // colliding with D?
        this->replacements.append(new FindAndReplace("ध़", "D.")); // lower octave? or komal?
        this->replacements.append(new FindAndReplace("ध़", "D.")); // lower octave? or komal?
        this->replacements.append(new FindAndReplace("ध", "D")); // colliding D?
        this->replacements.append(new FindAndReplace("d(k)", "d"));
        this->replacements.append(new FindAndReplace("d(k)", "d"));
        ////////////////////////
        this->replacements.append(new FindAndReplace("d", "d+"));
        this->replacements.append(new FindAndReplace("D", "D+"));

        this->replacements.append(new FindAndReplace("Ṉ", "n"));
        this->replacements.append(new FindAndReplace("नि॒", "n"));
        this->replacements.append(new FindAndReplace("नि", "N"));
        this->replacements.append(new FindAndReplace("ऩि", "N."));
        this->replacements.append(new FindAndReplace("ऩ", "n")); // @todo: NI, nee or ni komal?
        this->replacements.append(new FindAndReplace("नी", "N"));
        this->replacements.append(new FindAndReplace("N(k)", "n"));
        this->replacements.append(new FindAndReplace("n(k)", "n"));
        ////////////////////////
        this->replacements.append(new FindAndReplace("n", "n+"));
        this->replacements.append(new FindAndReplace("N", "N+"));

        // convert to western notes
        // + sign is appended to lock replacing other notes containing same letters. eg. RE => D4 => DHA? => A
        this->replacements.append(new FindAndReplace("S+", "C"));
        this->replacements.append(new FindAndReplace("r+", "C#"));
        this->replacements.append(new FindAndReplace("R+", "D"));
        this->replacements.append(new FindAndReplace("g+", "D#"));
        this->replacements.append(new FindAndReplace("G+", "E"));
        this->replacements.append(new FindAndReplace("m+", "F"));
        this->replacements.append(new FindAndReplace("M+", "F#"));
        this->replacements.append(new FindAndReplace("P+", "G"));
        this->replacements.append(new FindAndReplace("d+", "G#"));
        this->replacements.append(new FindAndReplace("D+", "A"));
        this->replacements.append(new FindAndReplace("n+", "A#"));
        this->replacements.append(new FindAndReplace("N+", "B"));
        this->replacements.append(new FindAndReplace("##", "#")); // MA' => F## => F#        
    };

    /**
     * @todo Reuse data from PianoRoll
     * @todo Use find-replace dataset
     * Usage purpose:
     *  - To validate a user input key.
     *  - To convert a key into MIDI key number
     * 
     * Expects that user types notations in Classical SARGAMS based on Roman letters.
     * 
     * Allowed letters: S, r, R, g, G, m, M, P, d, D, n, N
     * Allowed symbols: x, -, |, *, . (period), , (comma)
     * 
     * @todo Find an alternative way to replace strings
     * @see https://stackoverflow.com/questions/12048256/
     */
    QList<NotationCell *> HindustaniParser::parse(QString text)
    {
        // version: SA RE GA MA
        // version: Sa Re Ga(k) Ma
        // version: स र ग म
        // version: सा रे ग म॑
        // version: s r g m
        // version: S r G m' ... (selected)
        if(text.length()<=0) return {};
        
        // QString notes = this->replace_symbols(text);
        QString notes = this->replace(text);

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

#endif // LMMS_PLUGINS_MELODYEDITOR_HINDUSTANIPARSER_CPP