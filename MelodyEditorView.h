/**
 * MelodyEditorView.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_MELODY_EDITOR_VIEW_H
#define LMMS_GUI_MELODY_EDITOR_VIEW_H

#include <QLineEdit>

#include "ToolPluginView.h"

#include "MelodyEditor.h"
#include "MelodyEditorTextArea.h"
#include "src/parsers/ParsersFactory.h"
#include "src/includes/Utilities.h"

namespace lmms
{
	class MelodyEditor;
}

using lmms::gui::editor::pianoroll::parsing::Utilities;
using lmms::gui::editor::pianoroll::parsing::ParsersFactory;

namespace lmms::gui
{

	class MelodyEditorView : public ToolPluginView
	{
		private:
			Q_OBJECT

			MelodyEditor* m_plugin;
			bool m_liveWritingEnabled = false;

			/**
			 * Used inspite of -1 as placeholder.
			 * Defaults to first registered parser into the factory.
			 * Often, might register to MissingParser or NoneParser.
			 */
			int parser_id = 0;
			
			Utilities* u = new Utilities();

		public:
			MelodyEditorView(MelodyEditor* plugin);
			void updateMidiClip();
			void flagWriteNow();
			
			MelodyEditorTextArea* pte = new MelodyEditorTextArea();
			QLineEdit* error = new QLineEdit();
			const ParsersFactory *pf = new ParsersFactory();

			void openNotationsFileSelector();

		protected:
			void onComboBoxIndexChanged(int index);
			void closeEvent(QCloseEvent*) override;
	};

} // namespace lmms::gui

#endif // LMMS_GUI_MELODY_EDITOR_VIEW_H

