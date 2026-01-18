/**
 * MelodyEditorEventFilter.h
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_MELODY_EDITOR_EVENT_FILTER_H
#define LMMS_GUI_MELODY_EDITOR_EVENT_FILTER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>

namespace lmms::gui
{

	class MelodyEditorEventFilter: public QObject
	{
		private:
			Q_OBJECT

			QPlainTextEdit* m_editor;
			QPushButton* m_button;

		public:
			explicit MelodyEditorEventFilter(QObject* parent = nullptr);
			bool eventFilter(QObject* obj, QEvent* event) override;
	};

}

#endif // LMMS_GUI_MELODY_EDITOR_EVENT_FILTER_H