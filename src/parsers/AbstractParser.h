/**
 * AbstractParser.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_ABSTRACTPARSER_H
#define LMMS_PLUGINS_MELODYEDITOR_ABSTRACTPARSER_H

#include <vector>
#include <QString>

namespace lmms
{
class MidiClip;
class Model;
}


namespace lmms::melodyeditor
{


class AbstractParser
{
    public:
        AbstractParser() = default;
        virtual ~AbstractParser() = default;

        //! Display name
        virtual QString name() const = 0;

		//! Icon name
        virtual std::string icon() const = 0;

		//! Documentation for the Help button
		virtual QString help() const = 0;

		//! Log messages from parsing and import
		virtual QString logMessages() const = 0;

        /*! \brief Parse a string
         *
         *  This will be called any time the user changes the text.
         *  It should evaluate the string and write warnings and messages to the log.
         *  It should not touch anything in the current project.
         *
         *  NOTE: may throw ParserError
         */
        virtual void parse(const QString& string) = 0;

		/*! \brief Returns true if write() will only change what's been previously changed
		 *
		 *  This will be called after parse() and before write() during live coding.
		 *  Since write() may change anything in the project, the caller must
		 *  keep track of which Models were previously changed, and turn off
		 *  live coding if more Models are about to be changed.
		 *
		 *  NOTE: clipInPianoRoll is nullptr if no clip is open
		 */
		virtual bool isSafeToWrite(const MidiClip* clipInPianoRoll,
			const std::vector<const Model*>& previouslyChanged) = 0;

		/*! \brief Write to the project
		 *
		 *  This is called as the last step during live coding. It may write to
		 *  the clip in PianoRoll or to any other clip in the project. It may create tracks,
		 *  change tempo, etc. It all depends on the parser and what the user entered.
		 *
		 *  It returns a list of Models (clips, tracks, knobs) that were changed. It may be
		 *  used to check isSafeToWrite() on subsequent updates.
		 *
		 *  NOTE: may throw ParserError
		 *  NOTE: clipInPianoRoll is nullptr if no clip is open
		 */
		virtual std::vector<const Model*> write(MidiClip* clipInPianoRoll) = 0;

        //! Generate a string representation of the given MidiClip
        virtual QString importFromClip(const MidiClip*) { return {}; }
		virtual bool supportsClipImport() { return false; }

        //! Generate a string representation of the whole Song
        virtual QString importFromSong() { return {}; }
		virtual bool supportsSongImport() { return false; }
    };

}

#endif // LMMS_PLUGINS_MELODYEDITOR_ABSTRACTPARSER_H
