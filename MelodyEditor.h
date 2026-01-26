/**
 * MelodyEditor.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#pragma once

#include <QPointer>

#include "MidiClip.h"
#include "ToolPlugin.h"

class QTextDocument;

namespace lmms
{
	class ComboBoxModel;
}

namespace lmms::gui
{
	class MelodyEditorView;
}

namespace lmms::melodyeditor
{
	class AbstractParser;
}

using lmms::ToolPlugin;
using lmms::gui::PluginView;
using lmms::melodyeditor::AbstractParser;

namespace lmms
{

	class MelodyEditor : public ToolPlugin
	{
		Q_OBJECT

		public:
			MelodyEditor();
			virtual ~MelodyEditor() override = default;

			QString nodeName() const override;
			void saveSettings(QDomDocument&, QDomElement&) override {}
			void loadSettings(const QDomElement&) override {}
			void loadFile(const QString& filename) override;

			void setMidiClip(MidiClip* midiClip) { m_midiClip = midiClip; }
			inline void flag(bool value) {this->m_liveCodingModel->setValue(value); }

			PluginView* instantiateView(QWidget*) override;

		public slots:
			void importFromClip();
			void parse();
			void parseNotations(QString notations);
			void writeToClip();

		private:
			AbstractParser* parser();

			const std::vector<AbstractParser*> m_parsers;
			ComboBoxModel* m_parserModel;
			BoolModel* m_liveCodingModel;

			QPointer<MidiClip> m_midiClip;
			std::map<QString, QPointer<Track>> m_partMap;

			QString m_file;
			QTextDocument* m_document;
			QTextDocument* m_log;

		friend class lmms::gui::MelodyEditorView;
	};

} // namespace lmms