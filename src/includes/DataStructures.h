/**
 * DataStructures.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_EDITOR_PIANOROLL_PARSING_DATASTRUCTURES_H
#define LMMS_GUI_EDITOR_PIANOROLL_PARSING_DATASTRUCTURES_H

#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{
	/**
	 * For  individual <note/> node in the xml
	 */
	struct NotationCell
	{
		public:
			int position = 0; // pixels
			float length = 0.00f; // beat length in pixels?
			QString notation = "";
	};

	/**
	 * To build a dataset of find and replace strings.
	 */
	struct FindAndReplace
	{
		public:
			QString find = "";
			QString replaceWith = "";
			FindAndReplace(): find(""), replaceWith(""){}
			FindAndReplace(QString f, QString r): find(f), replaceWith(r)
			{
				this->find = f;
				this->replaceWith = r;
			}
	};
}

#endif // LMMS_GUI_EDITOR_PIANOROLL_PARSING_DATASTRUCTURES_H