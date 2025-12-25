/**
 * MelodyEditorView.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_MELODY_EDITOR_VIEW_H
#define LMMS_GUI_MELODY_EDITOR_VIEW_H

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

			/**
			 * Defaults to first registered parser into the factory.
			 * Despite of -1 as placeholder.
			 * Often, might register to MissingParser
			 */
			int parser_id = 0;
			
			Utilities* u = new Utilities();

		public:
			MelodyEditorView(MelodyEditor* plugin);
			void updateMidiClip();
			
			MelodyEditorTextArea *pte = new MelodyEditorTextArea();
			const ParsersFactory *pf = new ParsersFactory();

			void openNotationsFileSelector();

		protected:
			void closeEvent(QCloseEvent*) override;
	};

} // namespace lmms::gui

#endif // LMMS_GUI_MELODY_EDITOR_VIEW_H

