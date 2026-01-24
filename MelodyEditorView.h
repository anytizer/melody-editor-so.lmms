/**
 * MelodyEditorView.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_MELODY_EDITOR_VIEW_H
#define LMMS_GUI_MELODY_EDITOR_VIEW_H

#include "ToolPluginView.h"


namespace lmms
{
class MelodyEditor;
}


namespace lmms::gui
{

	class MelodyEditorView : public ToolPluginView
	{
		private:
			Q_OBJECT

			MelodyEditor* m_plugin;

		public:
			MelodyEditorView(MelodyEditor* plugin);

		public slots:
			void openNotationsFileSelector();
			void setClipFromPianoRoll();

	};

} // namespace lmms::gui

#endif // LMMS_GUI_MELODY_EDITOR_VIEW_H

