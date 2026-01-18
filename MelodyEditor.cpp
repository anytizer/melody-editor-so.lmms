/**
 * MelodyEditor.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#include <QWidget>

#include "MelodyEditorView.h"
#include "MelodyEditor.h"

#include "AudioEngine.h"
#include "Engine.h"
#include "Song.h"
#include "embed.h"
#include "plugin_export.h"

using lmms::gui::MelodyEditorView;

namespace lmms
{

extern "C"
{
	Plugin::Descriptor PLUGIN_EXPORT melodyeditor_plugin_descriptor = {
		LMMS_STRINGIFY(PLUGIN_NAME),
		"Melody Editor",
		QT_TRANSLATE_NOOP("PluginBrowser", "Melody Editor"),
		"Bimal Poudel <anytizer@users.noreply.github.com>",
		0x0100,
		Plugin::Type::Tool,
		new PluginPixmapLoader("logo"),
		"txt", // nullptr,
		nullptr,
	};

	PLUGIN_EXPORT Plugin* lmms_plugin_main(Model*, void*)
	{
		return new MelodyEditor();
	}

} // extern "C"


MelodyEditor::MelodyEditor(): ToolPlugin(&melodyeditor_plugin_descriptor, nullptr)
{
}

QString MelodyEditor::nodeName() const
{
	return melodyeditor_plugin_descriptor.name;
}

PluginView* MelodyEditor::instantiateView(QWidget*)
{
	return new MelodyEditorView(this);
}

} // namespace lmms
