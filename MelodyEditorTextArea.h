/**
 * MelodyEditorTextArea.h
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#pragma once

#include <QPlainTextEdit>
#include <QString>

class QWheelEvent;

namespace lmms::gui::melodyeditor
{
	class MelodyEditorTextArea: public QPlainTextEdit
	{
		private:
			Q_OBJECT
			QWidget *lineNumberArea;
			const QString helpText =
				"# 1. Type or paste melody notations here.\n"
				"# 2. Double click this area to open a file.\n"
				"# 3. Ctrl+/ toggles the commented lines.\n"
				"# 4. Ctrl+UP/DOWN, Ctrl+F9/F10 controls player.\n"
				"# 5. Refer to specific parser for notations.\n"
				"# 6. Ctrl+P on selection == partial parse.\n"
				"# 7. F1 - for help [ @anytizer, @allejok96 ].\n"
			;

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