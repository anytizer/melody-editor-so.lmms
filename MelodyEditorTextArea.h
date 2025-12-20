/**
 * MelodyEditorTextArea.h
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_MELODY_EDITOR_TEXTAREA_H
#define LMMS_GUI_MELODY_EDITOR_TEXTAREA_H

#include <QPlainTextEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QObject>
#include <QFileSystemWatcher>

#include "src/includes/Utilities.h"

using lmms::gui::editor::pianoroll::parsing::Utilities;

namespace lmms::gui
{
	class MelodyEditorTextArea: public QPlainTextEdit
	{
		private:

			Q_OBJECT

			bool loadNotationsFile(QString filename);
			QFileSystemWatcher watcher;
			
			Utilities* u = new Utilities();
			
		protected:
			void mouseDoubleClickEvent(QMouseEvent *event) override;
			void dragEnterEvent(QDragEnterEvent *event) override;
    		void dropEvent(QDropEvent *event) override;
			void wheelEvent(QWheelEvent *event) override;			
			
		public:
			MelodyEditorTextArea();
			~MelodyEditorTextArea();

		public slots:
			void openNotationsFileSelector();
	};
}

#endif // LMMS_GUI_MELODY_EDITOR_TEXTAREA_H