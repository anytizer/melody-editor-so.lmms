/**
 * VirtualpianoParser.cpp - Experimental virtual piano parser
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_VIRTUALPIANOPARSER_CPP
#define LMMS_PLUGINS_MELODYEDITOR_VIRTUALPIANOPARSER_CPP

#include "../includes/DataStructures.h"
#include "AbstractParser.h"
#include "EnglishParser.h"
#include "VirtualpianoParser.h"

#include <QList>
#include <QMessageBox>

using lmms::gui::editor::pianoroll::parsing::NotationCell;

namespace lmms::gui::editor::pianoroll::parsing
{
    void VirtualpianoParser::setup()
    {
        this->_name = "Virtual Piano";
        this->_identifier = "virtual";

        this->vkr = {};
        this->vkr.append(new FindAndReplace("1", "C2"));
        this->vkr.append(new FindAndReplace("2", "D2"));
        this->vkr.append(new FindAndReplace("3", "E2"));
        this->vkr.append(new FindAndReplace("4", "F2"));
        this->vkr.append(new FindAndReplace("5", "G2"));
        this->vkr.append(new FindAndReplace("6", "A2"));
        this->vkr.append(new FindAndReplace("7", "B2"));
        this->vkr.append(new FindAndReplace("8", "C3"));
        this->vkr.append(new FindAndReplace("9", "D3"));
        this->vkr.append(new FindAndReplace("0", "E3"));
        this->vkr.append(new FindAndReplace("q", "F3"));
        this->vkr.append(new FindAndReplace("w", "G3"));
        this->vkr.append(new FindAndReplace("e", "A3"));
        this->vkr.append(new FindAndReplace("r", "B3"));
        this->vkr.append(new FindAndReplace("t", "C4"));
        this->vkr.append(new FindAndReplace("y", "D4"));
        this->vkr.append(new FindAndReplace("u", "E4"));
        this->vkr.append(new FindAndReplace("i", "F4"));
        this->vkr.append(new FindAndReplace("o", "G4"));
        this->vkr.append(new FindAndReplace("p", "A4"));
        this->vkr.append(new FindAndReplace("a", "B4"));
        this->vkr.append(new FindAndReplace("s", "C5"));
        this->vkr.append(new FindAndReplace("d", "D5"));
        this->vkr.append(new FindAndReplace("f", "E5"));
        this->vkr.append(new FindAndReplace("g", "F5"));
        this->vkr.append(new FindAndReplace("h", "G5"));
        this->vkr.append(new FindAndReplace("j", "A5"));
        this->vkr.append(new FindAndReplace("k", "B5"));
        this->vkr.append(new FindAndReplace("l", "C6"));
        this->vkr.append(new FindAndReplace("z", "D6"));
        this->vkr.append(new FindAndReplace("x", "E6"));
        this->vkr.append(new FindAndReplace("c", "F6"));
        this->vkr.append(new FindAndReplace("v", "G6"));
        this->vkr.append(new FindAndReplace("b", "A6"));
        this->vkr.append(new FindAndReplace("n", "B6"));
        this->vkr.append(new FindAndReplace("m", "C7"));
        this->vkr.append(new FindAndReplace("!", "C#2"));
        this->vkr.append(new FindAndReplace("@", "D#2"));
        this->vkr.append(new FindAndReplace("#", "F2"));
        this->vkr.append(new FindAndReplace("$", "F#2"));
        this->vkr.append(new FindAndReplace("%", "G#2"));
        this->vkr.append(new FindAndReplace("^", "A#2"));
        this->vkr.append(new FindAndReplace("&", "C3"));
        this->vkr.append(new FindAndReplace("*", "C#3"));
        this->vkr.append(new FindAndReplace("(", "D#3"));
        this->vkr.append(new FindAndReplace(")", "F3"));
        this->vkr.append(new FindAndReplace("Q", "F#3"));
        this->vkr.append(new FindAndReplace("W", "G#3"));
        this->vkr.append(new FindAndReplace("E", "A#3"));
        this->vkr.append(new FindAndReplace("R", "C4"));
        this->vkr.append(new FindAndReplace("T", "C#4"));
        this->vkr.append(new FindAndReplace("Y", "D#4"));
        this->vkr.append(new FindAndReplace("U", "F4"));
        this->vkr.append(new FindAndReplace("I", "F#4"));
        this->vkr.append(new FindAndReplace("O", "G#4"));
        this->vkr.append(new FindAndReplace("P", "A#4"));
        this->vkr.append(new FindAndReplace("A", "C5"));
        this->vkr.append(new FindAndReplace("S", "C#5"));
        this->vkr.append(new FindAndReplace("D", "D#5"));
        this->vkr.append(new FindAndReplace("F", "F5"));
        this->vkr.append(new FindAndReplace("G", "F#5"));
        this->vkr.append(new FindAndReplace("H", "G#5"));
        this->vkr.append(new FindAndReplace("J", "A#5"));
        this->vkr.append(new FindAndReplace("K", "C6"));
        this->vkr.append(new FindAndReplace("L", "C#6"));
        this->vkr.append(new FindAndReplace("Z", "D#6"));
        this->vkr.append(new FindAndReplace("X", "F6"));
        this->vkr.append(new FindAndReplace("C", "F#6"));
        this->vkr.append(new FindAndReplace("V", "G#6"));
        this->vkr.append(new FindAndReplace("B", "A#6"));
        this->vkr.append(new FindAndReplace("N", "C7"));
        this->vkr.append(new FindAndReplace("M", "C#7"));
    };

    /**
	* There are many virtual keyboards possible.
    * Hence, this parser is experimental.
	* 
	* @todo Pick one virtual piano keyboard.
	* eg.: LMMS internal virual keyboard?
	* 
	* eg. From pianoletternotes:
	* The numbers in front of each line are the octave, each octave has an unique color so you can easily follow them.
	* Lowercase (a b c d e f g) letters are natural notes (white keys, a.k.a A B C D E F G ).
	* Uppercase (A C D F G) letters are the sharp notes (black keys a.k.a. A# C# D# F# G#)
	* The lines / dashes (-) between letters indicates timing to play the notes.
	* (usually 5-6 dashes is about 1 second)
	* RH / LH means Right Hand / Left Hand and it's mostly for people who play the piano, it tells them with what hand to play the lines.
	* Also, if you want to play a easy version of the song, playing only the RH lines does exactly that,
	* because on most songs RH notes are for melody and LH notes are for bass.
	* 
	* eg. from VirtualPiano.net:
	* ORDER OF LETTERS | HOW THEY’RE PLAYED
	* -----------------|-------------------------------------------------
	* [asdf]           | Play notes together simultaneously
	* [a s d f]        | Play the sequence at fastest possible speed
	* asdf             | Play notes one after the other quickly
	* a s d f          | Play each note after a short pause
	* [as] [df]        | Play “as” together, short pause, then “df” together
	* as|df            | Pause for “|”
	* as| df           | Long pause for “|” with one extra space
	* as | df          | Longer pause for “|” with 2 extra spaces
	* as| |df          | Longest pause for 2 “|” with an extra space
	* Paragraph Break  | Extended pause
	* 
	* Inferences
	*   - space is a pause denotation
	*   - pipe is a long pause
	*   - braces are like chords
	*
	* @todo vk() needs to know the long pressed keys | done for notes but the chords.
	* @todo Writing comments for vk() notations is currently impossible, due to # in use.
	* @todo Move *fr variable outside of the loop | reuse from parent.
	*/
    QList<NotationCell *> VirtualpianoParser::parse(QString text)
    {
        if(text.length()<=0) return {};

        QString un = this->replace(text);

        un.replace("\r", "\n");
        un.replace("\n\n", "\n");
        QStringList input = un.split(""); // , Qt::KeepEmptyParts);

        QStringList western = {};
        for (const QString& column : input)
        {
            if(column.contains("["))
            {
                western.append("[");
            }
            else if(column.contains("]"))
            {
                // @todo Attached symbols won't load. eg. --]
                western.append("]");
            }
            else if(column == " " || column == "\n")
            {
                western.append("x");
            }
            else if (column == "|")
            {
                // longer silence
                western.append("x");
                western.append("x");
            }
            else if(column == "-")
            {
                // to elongate chords or notes
                western.append("-");
            }
            else
            {
                // replace keyboard mappings
                for(int i=0; i<this->vkr.size(); ++i)
                {
                    if(this->vkr.at(i)->find == column)
                    {
                        western.append(this->vkr.at(i)->replaceWith);
                        break;
                    }
                }
            }
        } // for

        // Now, an English parser should be enough here.
        EnglishParser *english = new EnglishParser();
        return english->parse(western.join(" "));
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_VIRTUALPIANOPARSER_CPP