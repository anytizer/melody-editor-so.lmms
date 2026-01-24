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

#include "MelodyEditorTextArea.h"
#include "src/includes/Utilities.h"


using namespace lmms::melodyeditor;

namespace lmms::gui::melodyeditor
{

	MelodyEditorTextArea::MelodyEditorTextArea()
	{
		this->setAcceptDrops(true);

		// Try to be OS neutral.
		QFont font("Consolas", MIN_FONTSIZE+4); // Consolas | sans-serif @ 14 points
		this->setFont(font);
		this->setStyleSheet("font-family: Menlo, Consolas, 'Ubuntu Mono', 'Roboto Mono', 'DejaVu Sans Mono', monospace;");

		
		//this->setPlainText("# Double-Click to load a file.");
		
		// @todo Placeholder Text is not experienced.
		this->setPlaceholderText("# Type or paste melody notations...");
		
		// Disable right click menus.
		// But still, keep the shortcuts enabled eg. ctrl+C, ctrl+v
		//this->setContextMenuPolicy(Qt::NoContextMenu);
		
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

} // lmms::gui