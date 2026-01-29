/**
 * MelodyEditorTextArea.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#include <QMimeData>
#include <QTextStream>
#include <QString>
#include <QFont>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QPainter>
#include <QTextBlock>

#include <QDebug>

#include "MelodyEditorTextArea.h"
#include "src/includes/Utilities.h"


using namespace lmms::melodyeditor;

namespace lmms::gui::melodyeditor
{

	MelodyEditorTextArea::MelodyEditorTextArea()
	{
		lineNumberArea = new LineNumberArea(this);
		connect(this, &MelodyEditorTextArea::blockCountChanged, this, &MelodyEditorTextArea::updateLineNumberAreaWidth);
		connect(this, &MelodyEditorTextArea::updateRequest, this, &MelodyEditorTextArea::updateLineNumberArea);
		connect(this, &MelodyEditorTextArea::cursorPositionChanged, this, &MelodyEditorTextArea::highlightCurrentLine);
		updateLineNumberAreaWidth(0);
    	highlightCurrentLine();

		this->setAcceptDrops(true);

		// Try to be OS neutral.
		QFont font("Consolas", 14); // Consolas | sans-serif @ 14 points
		this->setFont(font);
		this->setStyleSheet(
			"font-family: Menlo, Consolas, 'Ubuntu Mono', 'Roboto Mono', 'DejaVu Sans Mono', monospace;"
			//"selection-background-color: #c02b2b;"
			"selection-color: #d908f0;"
		);

		//this->setPlainText("# Double-Click to load a file.");
		
		// @todo Placeholder Text is not experienced.
		this->setPlaceholderText("# 1. Type or paste melody notations.\n# 2. Or, Double-Click to load a file.");
		
		// Disable right click menus.
		// But still, keep the shortcuts enabled eg. ctrl+C, ctrl+v
		// this->setContextMenuPolicy(Qt::NoContextMenu);
		
		// Enforce
		this->setLineWrapMode(QPlainTextEdit::NoWrap);
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		this->setCursor(Qt::IBeamCursor);
		this->ensureCursorVisible();
	}

	MelodyEditorTextArea::~MelodyEditorTextArea()
	{
	}

	void MelodyEditorTextArea::mouseDoubleClickEvent(QMouseEvent *event)
	{
		if (event->button() == Qt::LeftButton) {
			emit doubleClicked();
			event->accept();
		} else {
			QPlainTextEdit::mouseDoubleClickEvent(event);
		}
	}

	/**
	 * Custom zoom logic on ctrl+wheel
	 */
	void MelodyEditorTextArea::wheelEvent(QWheelEvent *event)
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			QFont font = this->font();
			int pointsize = font.pointSize() + (event->angleDelta().y() / ZOOM_FACTOR);
			font.setPointSize(std::clamp(pointsize, MIN_FONTSIZE, MAX_FONTSIZE));
			this->setFont(font);
		}

		// @see https://doc.qt.io/qt-6/qml-qtquick-controls-scrollview.html
		event->accept();
		QPlainTextEdit::wheelEvent(event); // make scrollable with wheel
	}


	void MelodyEditorTextArea::dragEnterEvent(QDragEnterEvent *event)
	{
		if (!pathFromMimeData(event->mimeData()).isEmpty())
		{
			event->acceptProposedAction();
		}
	}

	void MelodyEditorTextArea::dropEvent(QDropEvent *event)
	{
		auto path = pathFromMimeData(event->mimeData());
		if (!path.isEmpty())
		{
			emit fileDropped(path);
			event->acceptProposedAction();
		}
	}




	int MelodyEditorTextArea::lineNumberAreaWidth() {
		int digits = 1;
		int max = qMax(1, blockCount());
		while (max >= 10) {
			max /= 10;
			digits++;
		}

		// minimum size: 2 digits
		digits = qMax(2, digits);

		int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
		return space;
	}

	void MelodyEditorTextArea::updateLineNumberAreaWidth(int newBlockCount) {
		setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
	}

	void MelodyEditorTextArea::updateLineNumberArea(const QRect &rect, int dy) {
		if (dy)
			lineNumberArea->scroll(0, dy);
		else
			lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

		if (rect.contains(viewport()->rect()))
			updateLineNumberAreaWidth(0);
	}

	void MelodyEditorTextArea::resizeEvent(QResizeEvent *e) {
		QPlainTextEdit::resizeEvent(e);
		QRect cr = contentsRect();
		lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
	}

	void MelodyEditorTextArea::lineNumberAreaPaintEvent(QPaintEvent *event) {
		QPainter painter(lineNumberArea);
		//painter.fillRect(event->rect(), Qt::lightGray);
		painter.fillRect(event->rect(), QColor("#000000"));

		QTextBlock block = firstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
		int bottom = top + qRound(blockBoundingRect(block).height());

		while (block.isValid() && top <= event->rect().bottom()) {
			if (block.isVisible() && bottom >= event->rect().top()) {
				QString number = QString::number(blockNumber + 1);
				painter.setPen(QColor("#fafa06"));
				painter.drawText(0, top, lineNumberArea->width() - 2, fontMetrics().height(),
								Qt::AlignRight, number);
			}

			block = block.next();
			top = bottom;
			bottom = top + qRound(blockBoundingRect(block).height());
			++blockNumber;
		}
	}

	void MelodyEditorTextArea::highlightCurrentLine() {
		QList<QTextEdit::ExtraSelection> extraSelections;
		if (!isReadOnly()) {
			QTextEdit::ExtraSelection selection;
			//QColor lineColor = QColor(Qt::yellow).lighter(160);
			QColor lineColor = QColor("#8e8810");
			lineColor.setAlpha(40);
			selection.format.setBackground(lineColor);
			selection.format.setProperty(QTextFormat::FullWidthSelection, true);
			selection.cursor = textCursor();
			selection.cursor.clearSelection();
			extraSelections.append(selection);
		}
		setExtraSelections(extraSelections);
	}











	void MelodyEditorTextArea::keyPressEvent(QKeyEvent *e) {
		QString completed = "";
		QString pressed = e->text();
		
		if(pressed == "[") completed = "[]";
		if(pressed == "{") completed = "{}";
		if(pressed == "(") completed = "()";
		
		// Single quotes are part of notations itself.
		// Double quotes are often chords
		if(pressed == "\"") completed = "\"\"";
		
		if(completed!="")
		{
			this->insertPlainText(completed);
			QTextCursor c = this->textCursor();
			c.movePosition(QTextCursor::Left);
			this->setTextCursor(c);
		} else {
			QPlainTextEdit::keyPressEvent(e);
		}
	}

} // lmms::gui