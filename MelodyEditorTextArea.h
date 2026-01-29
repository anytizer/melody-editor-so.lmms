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
			QWidget *lineNumberArea;

			void toggleComments();

		protected:
			void mouseDoubleClickEvent(QMouseEvent *event) override;
			void dragEnterEvent(QDragEnterEvent *event) override;
    		void dropEvent(QDropEvent *event) override;
			void wheelEvent(QWheelEvent *event) override;
			void resizeEvent(QResizeEvent *event) override;
			void keyPressEvent(QKeyEvent *e);
			
		public:
			MelodyEditorTextArea();
			~MelodyEditorTextArea();

			// to implement line numbers
			void lineNumberAreaPaintEvent(QPaintEvent *event);
    		int lineNumberAreaWidth();

		signals:
			void fileDropped(QString path);
			void doubleClicked();
			void keyPressed();
		
		private slots:
			void updateLineNumberAreaWidth(int newBlockCount);
			void highlightCurrentLine();
			void updateLineNumberArea(const QRect &rect, int dy);
	};



	class LineNumberArea : public QWidget {
		public:
			LineNumberArea(MelodyEditorTextArea *editor) : QWidget(editor), editor(editor) {}

			QSize sizeHint() const override {
				return QSize(editor->lineNumberAreaWidth(), 0);
			}

		protected:
			void paintEvent(QPaintEvent *event) override {
				editor->lineNumberAreaPaintEvent(event);
			}

		private:
			MelodyEditorTextArea *editor;
	};
}