/**
 * MelodyEditorTextArea.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#pragma once

#include <QPlainTextEdit>

class QWheelEvent;

namespace lmms::gui::melodyeditor
{
	class MelodyEditorTextArea: public QPlainTextEdit
	{
		private:
			Q_OBJECT

		protected:
			void mouseDoubleClickEvent(QMouseEvent *event) override;
			void dragEnterEvent(QDragEnterEvent *event) override;
    		void dropEvent(QDropEvent *event) override;
			void wheelEvent(QWheelEvent *event) override;
			
		public:
			MelodyEditorTextArea();
			~MelodyEditorTextArea();

		signals:
			void fileDropped(QString);
			void doubleClicked();
	};
}