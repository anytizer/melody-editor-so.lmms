/**
 * MelodyEditor.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_MELODY_EDITOR_H
#define LMMS_MELODY_EDITOR_H

#include "MelodyEditorView.h"
#include "ToolPlugin.h"

using lmms::ToolPlugin;
using lmms::gui::PluginView;

namespace lmms
{

	class MelodyEditor : public ToolPlugin
	{
		public:
			MelodyEditor();

			QString nodeName() const override;
			void saveSettings(QDomDocument&, QDomElement&) override {}
			void loadSettings(const QDomElement&) override {}

			PluginView* instantiateView(QWidget*) override;
	};

} // namespace lmms

#endif // LMMS_MELODY_EDITOR_H