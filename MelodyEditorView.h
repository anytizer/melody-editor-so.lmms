/**
 * MelodyEditorView.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#pragma once

#include "ToolPluginView.h"

#include "MelodyEditorTextArea.h"

namespace lmms
{
	class MelodyEditor;
}


namespace lmms::gui
{
	using lmms::gui::melodyeditor::MelodyEditorTextArea;

	class MelodyEditorView : public ToolPluginView
	{
		private:
			Q_OBJECT

			MelodyEditor* m_plugin;
			MelodyEditorTextArea* textArea;

		public:
			MelodyEditorView(MelodyEditor* plugin);
			
		public slots:
			void openNotationsFileSelector();
			void setClipFromPianoRoll();
			void formatNotes();
			QString getSelectedNotations();
		
		protected:
			void keyPressEvent(QKeyEvent* event) override;
	};

} // namespace lmms::gui
