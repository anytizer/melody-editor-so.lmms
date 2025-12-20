/**
 * AbstractParser.cpp
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGIN_MELODYEDITOR_ABSTRACTPARSER_CPP
#define LMMS_PLUGIN_MELODYEDITOR_ABSTRACTPARSER_CPP

#include "../includes/DataStructures.h"
#include "AbstractParser.h"

#include <QString>
#include <QList>

namespace lmms::gui::editor::pianoroll::parsing
{
    AbstractParser::AbstractParser()
    {
        this->notes = {}; // start fresh
        this->notes.append("C");  // 0
        this->notes.append("C#"); // 1
        this->notes.append("D");  // 2
        this->notes.append("D#"); // 3
        this->notes.append("E");  // 4
        this->notes.append("F");  // 5
        this->notes.append("F#"); // 6
        this->notes.append("G");  // 7
        this->notes.append("G#"); // 8
        this->notes.append("A");  // 9
        this->notes.append("A#"); // 10
        this->notes.append("B");  // 11

        this->replacements = {};
        this->replacements.append(new FindAndReplace("™", "#"));
        // notes.replace("‘", replace = SpecialKeys.LOWER_OCTAVE_NOTATION }); // appears in front
        
        // common symbols
        // notes.replace("*", replace = sk.HIGHER_OCTAVE_NOTATION }); // not necessary | same destination
        
        // super oddities, rather used in octave identification
        // notes.replace("`", "#"); // sharp/flat
        // notes.replace("'", "#"); // sharp/flat
        // notes.replace("’", "#"); // sharp/flat ??
        
        QString higher_octavemarker = "*";
        this->replacements.append(new FindAndReplace("º", higher_octavemarker));
        this->replacements.append(new FindAndReplace("°", higher_octavemarker));
        this->replacements.append(new FindAndReplace("’", higher_octavemarker)); // octave change!!
        //notes.replace("'", higher_octavemarker); // octave change!!

        // continuation mark
        QString continuation = "-";
        this->replacements.append(new FindAndReplace("-", continuation)); // ??
        this->replacements.append(new FindAndReplace("-", continuation));
        this->replacements.append(new FindAndReplace("_", continuation));
        this->replacements.append(new FindAndReplace("—", continuation));
        this->replacements.append(new FindAndReplace("~", continuation));
        this->replacements.append(new FindAndReplace("ऽ", continuation)); // avagraha

        // division / bar separator
        QString division = "|";
        this->replacements.append(new FindAndReplace("/", division));
        this->replacements.append(new FindAndReplace("।", division));
        this->replacements.append(new FindAndReplace("|", division));
        this->replacements.append(new FindAndReplace("\\", division));

        this->replacements.append(new FindAndReplace("X", "x")); // slience marker
    }

    AbstractParser::~AbstractParser()
    {
        this->notes.clear();
        this->replacements.clear();
    }

    QString AbstractParser::replace(QString text)
    {
        foreach(FindAndReplace* far, this->replacements)
        {
            text = text.replace(far->find, far->replaceWith);
        }

        return text;
    }
    
    /**
     * @todo Reuse data from PianoRoll if possible
     * Usage purpose:
     *  - To validate a user input key.
     *  - To convert a key into MIDI key number
     *  - Depends on the build type
     */
    int AbstractParser::getPianoKey(QString note)
    {
        int octave = 4 + 1; // C4 is the standard root key
        octave += note.count("*");
        octave -= note.count(".");

        QString purenote = note;
        purenote.replace("*", "");
        purenote.replace(".", "");

        int pianokey = -1;
        for(int nn = 0; nn<this->notes.count(); ++nn)
        {
            if(purenote == this->notes[nn])
            {
                pianokey = 12 * octave + nn; // note number
                break;
            }
        }

        // -1 is an erraneous entry
        return pianokey;
    }

    /**
     * Output of this call MUST be similar to Export MIDI Clip.
     * 
     * As of 2025-11-08:
     * <lmms-project creator="LMMS" creatorplatform="linux" creatorplatformtype="ubuntu" creatorversion="1.3.0-alpha.1.916+f0cb32f" type="midiclip" version="31">
     * <head/>
     * <midiclip>
     *   <note />
     * </midiclip>
     * </lmms-project>
     */

    void AbstractParser::cells_to_xml(QList<NotationCell *> cells, QString &xml)
    {
        int steps = 16;

        /**
         * Fake information to simulate to the latest LMMS version data
         */
        
        // must be: integer, @see DataFile.cpp#2175
        QString version = "31";
        
        // This application name
        QString creator = "MelodyEditor";
        
        // @todo Determine it
        QString creator_version = "MelodyEditor v1.0";
        
        // @todo Use block number as well
        // "Melody Clip: %1";
        QString block_name = "Melody Clip";// " #%1").arg();
        block_name.replace("'", ""); // To not to break xml with quotes
        
        // @todo Use color rotator for each block
        QString color = "#7d039f"; // always prepend a # sign

        QString xmlbody = "";
        int globallength = 0;
        for(NotationCell *cell: cells) // cells | notes
        {
            globallength += cell->length;

            int pianoKey = getPianoKey(cell->notation);
            if(pianoKey == -1) continue; // skip invalid key

            xmlbody += QString("<note key='%1' len='%2' pan='0' pos='%3' type='0' vol='100' name='%4' />").arg(
                QString("%1").arg(pianoKey),
                QString("%1").arg(cell->length),
                QString("%1").arg(cell->position),
                cell->notation
            );
        }

        QString xmlhead = QString(
            "<midiclip steps='%1' len='%2' off='0' muted='0' name='%3' color='%4' pos='0' type='1' autoresize='1'>"
        ).arg(
            QString("%1").arg(steps),
            QString("%1").arg(globallength),
            block_name,
            color
        );

        QString xmlfoot = QString("</midiclip>");

        xml = xmlhead + xmlbody + xmlfoot;
        
        // double quoted values; as the xml is built with single quotes for ease | above
        xml.replace("'", "\"");

        // @todo save .xpt file optionally.
    }


    /**
     * @todo Save individual XMLs too, in notations-xx.xpt.
     * @todo For vk(), treat new line as spacer | Done in the source.
     */
    int AbstractParser::process_block(const QString block, QList<NotationCell *> &cells, int &position)
    {
        int errors = 0;

        QString _block = block;
        _block.replace("\r", "\n");

        QStringList lines = _block.split("\n");
        for(const QString& line: lines)
        {
            QString ln = line.trimmed();

            if(ln == "")
                continue;

            // comments be fitered out
            if(!ln.startsWith("#"))
            {
                errors += this->process_line(ln, cells, position);
            }
        }

        return errors;
    }


    int AbstractParser::process_line(const QString line, QList<NotationCell *> &cells, int &position)
    {
        int errors = 0;
        QString oneline = line;
        oneline.replace("/", "|");
        oneline.replace("|", " ");
        oneline.replace("\t", " ");
        
        QStringList columns = oneline.split(" ");
        for(const QString& column: columns)
        {
            QString col = column.trimmed();
            if(col!="")
            {
                errors += this->process_beatnotes(col, cells, position);
            }
        }

        return errors;
    }

	/**
	 * One beat of a measure!
	 * 
	 * Multiple notes may share the time using comma.
	 * eg. "C,D" - 50% time shared
	 * eg. "G,R,S" - 33% time shared
	 * eg. "R,S,D.,P." - 25% time shared
	 */
	int AbstractParser::process_beatnotes(const QString column, QList<NotationCell *> &cells, int &position)
	{
		int errors = 0;
        
        // 16 x 4? nominator x denominator? Not sure.
        // @see below function also
		int width = 48;

		if(column.contains(","))
		{
			float semilength = width / (column.count(",") + 1);
			for(const QString& tone: column.split(","))
			{
				errors += this->process_tone(tone, cells, semilength, position);
			}
		}
		else
		{
			errors += this->process_tone(column, cells, width, position);
		}

		return errors;
	}


	/**
	 * At this point, you receive a particularly pitched tone.
	 */
	int AbstractParser::process_tone(const QString tone, QList<NotationCell *> &cells, float length, int &position)
	{
		int errors = 0;
		
		if(tone == "x" || tone == "X")
		{
			// Silence marker. Just jump to another placement.
			position += length;
		}	
		else if(tone == "-")
		{
			if(this->chord_processing)
			{
				// elongate each note of the chords
				for(NotationCell *cell: this->chords)
				{
					cell->length += length;
				}
			}
			else if(cells.count() > 0)
			{
				cells.back()->length += length;
			}
			// else
			// {
			// 	// cannot start with a continuation mark
			// }
			
			position += length;
		}
		else if(tone == "[")
		{
			// chords start
			this->chord_processing = true;
			this->chord_start_position = position; // [ q w e r t y ]
		}
		else if(tone == "]")
		{
			this->chord_processing = false;
			for(NotationCell *chord: this->chords)
			{
				cells.append(chord);
			}
			
			this->chords = {}; // for next round
			position += length;
		}
		else if(this->getPianoKey(tone)!=-1)// @todo if tone_exists(tone)?
		// @todo eg. Filter E#, W#, ... kind of wrong notes.
		// @todo Currently they simply occupy place but do not appear in C4.
		// @todo Steps still counted.
		{
			// @todo Validate tone. Otherwise, they will appear in C4 position.
			NotationCell *cell = new NotationCell();

			if(this->chord_processing)
			{
				// @todo Make it posible to control length of a chord.
				cell->position = this->chord_start_position;
				cell->length = 48; // length; // @todo Fix hard coded value | or, share from above
				cell->notation = tone;
                chords.append(cell);
    		}
			else
			{
				cell->position = position;
				cell->length = length;
				cell->notation = tone;
				cells.append(cell);
			
				position += length;
			}
		}
		else
		{
			errors = 1;
		}

		return errors;
	}
}

#endif // LMMS_PLUGIN_MELODYEDITOR_ABSTRACTPARSER_CPP