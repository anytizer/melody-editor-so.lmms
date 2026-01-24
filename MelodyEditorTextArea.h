/**
 * MelodyEditorTextArea.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#ifndef LMMS_GUI_MELODY_EDITOR_TEXTAREA_H
#define LMMS_GUI_MELODY_EDITOR_TEXTAREA_H

#include <QPlainTextEdit>

class QWheelEvent;

namespace lmms::gui::melodyeditor
{
	class MelodyEditorTextArea: public QPlainTextEdit
	{
		private:
			Q_OBJECT

		protected:
			void dragEnterEvent(QDragEnterEvent *event) override;
    		void dropEvent(QDropEvent *event) override;
			void wheelEvent(QWheelEvent *event) override;			
			
		public:
			MelodyEditorTextArea();
			~MelodyEditorTextArea();

		signals:
			void fileDropped(QString);
	};
}

#endif // LMMS_GUI_MELODY_EDITOR_TEXTAREA_H
