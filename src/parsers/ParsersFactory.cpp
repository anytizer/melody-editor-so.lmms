/**
 * ParserFactory.cpp- List of registered melody parsers
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_PARSERFACTORY_CPP
#define LMMS_PLUGINS_MELODYEDITOR_PARSERFACTORY_CPP

#include "ParsersFactory.h"

#include "ABCParser.h"
#include "ALDAParser.h"
#include "CarnaticParser.h"
#include "EnglishParser.h"
#include "GermanParser.h"
#include "HindustaniParser.h"
#include "NashvilleParser.h"
#include "VirtualpianoParser.h"

namespace lmms::gui::editor::pianoroll::parsing
{
    ParsersFactory::ParsersFactory()
    {
        // If parser is not completed: Do not expose it.
        // Parsers will appear in the same order as registered.
        this->parsers = {};

        /**
         * DO NOT change the order for this parser.
         * NONE-Parser should be on TOP, at the 0-th position.
         * 
         * It is a placeholder.
         */
        //this->parsers.append(new NoneParser());

        /**
         * ABC Notation parser
         * 
         * @see https://abcnotation.com/
         */
        this->parsers.append(new ABCParser());

        /**
         * ALDA parser
         * 
         * @see https://alda.io/
         * @see https://alda.io/cheat-sheet/
         */
        this->parsers.append(new ALDAParser());

        /**
         * SARGAM Parser in Romanized letters.
         * SARGAM Parser in Devangari scripts.
         */
        this->parsers.append(new HindustaniParser());
        
        /**
         * Mostly similar to regular SARGAMs
         * @todo incomplete | partially functioning
         * @see https://www.melakarta.com/education.html
         * @see https://www.shivkumar.org/music/
         */
        // this->parsers.append(new CarnaticParser());
        
        /**
         * English keyboard based
         */
        this->parsers.append(new EnglishParser());

        /**
         * German notes
         * Also see:
         * @see https://en.wikipedia.org/wiki/Helmholtz_pitch_notation
         */
        // this->parsers.append(new GermanParser());

        /**
         * Nashville Number system
         * 
         * @see https://www.grossepointemusicacademy.com/nashville-number-system/
         * @see https://loopcommunity.com/blog/2021/09/the-nashville-number-system/
         */
        // this->parsers.append(new NashvilleParser());

        /**
         * Various virtual piano - using QWERTY keyboard data entry
         */
        // this->parsers.append(new VirtualpianoParser());
        
        // @todo Keyboard settings differences!!
        // LMMS Mac Build keyboard layout
        // LMMS Windows Build keyboard layout
        // LMMS Linux Build keyboard layout
        
        /**
         * If we missed a parser above...
         * DO NOT queue it as a general purpose parser in abovez code.
         */
        // if(!this->parsers.count())
        // {
        //     this->parsers.append(new MissingParser());
        // }

        /**
         * Each parser has a setup() method.
         * Invoke them.
         */
        this->setup_all_registered_parsers();
    }

    ParsersFactory::~ParsersFactory()
    {
        this->parsers.clear();
    }

    void ParsersFactory::setup_all_registered_parsers()
    {
        int len = this->parsers.length();
        for(int i=0; i<len; ++i)
        {
            this->parsers[i]->setup();
        }
    }
}

#endif // LMMS_PLUGINS_MELODYEDITOR_PARSERFACTORY_CPP