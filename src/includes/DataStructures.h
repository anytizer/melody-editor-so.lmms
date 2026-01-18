/**
 * DataStructures.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_PLUGINS_MELODYEDITOR_DATASTRUCTURES_H
#define LMMS_PLUGINS_MELODYEDITOR_DATASTRUCTURES_H

#include <QString>

namespace lmms::gui::editor::pianoroll::parsing
{
	/**
	 * For individual <note/> node in the xml
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
	 * @todo Replace with QMap; for speedy operation?
	 * @see https://doc.qt.io/qt-6/qmap.html
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

#endif // LMMS_PLUGINS_MELODYEDITOR_DATASTRUCTURES_H