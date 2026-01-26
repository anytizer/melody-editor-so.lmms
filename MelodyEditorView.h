/**
 * MelodyEditorView.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#pragma once

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
			//void handleSelectionChanged(QString text);

	};

} // namespace lmms::gui
