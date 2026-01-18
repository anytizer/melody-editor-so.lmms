/**
 * MelodyEditorView.cpp
 *
 * Copyright (c) 2025 - 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QDomDocument>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QEvent>
#include <QDebug>

#include "MelodyEditor.h"
#include "MelodyEditorView.h"
#include "MelodyEditorEventFilter.h"

#include "src/includes/DataStructures.h"
#include "src/includes/Utilities.h"
#include "src/parsers/ParsersFactory.h"

#include "Engine.h"
#include "Song.h"
#include "GuiApplication.h"
#include "PianoRoll.h"
#include "MidiClip.h"
#include "MidiClipView.h"
#include "Editor.h"

using lmms::gui::editor::pianoroll::parsing::AbstractParser;
using lmms::gui::editor::pianoroll::parsing::NotationCell;
using lmms::gui::editor::pianoroll::parsing::Utilities;
using lmms::gui::PianoRollWindow;
using lmms::MidiClip;

namespace lmms::gui
{

	MelodyEditorView::MelodyEditorView(MelodyEditor* plugin)
		: ToolPluginView(plugin),
		m_plugin(plugin)
	{
		this->setAcceptDrops(true);

		this->setMinimumSize(300, 300);
		this->setMaximumSize(800, 600);
		this->resize(550, 300);

		const int ui_height = 28; // height for combo box and button

		// %1 = melodyeditor, %2 = sargam|etc.
		// @see icons.qrc, CMakeLists.txt
		const QString icon_at = ":/artwork/%1/ns-%2.svg";

		QComboBox *parsers_combobox = new QComboBox(this); // text | data
		parsers_combobox->setEditable(false);
		parsers_combobox->setMinimumHeight(ui_height);
		parsers_combobox->setCurrentIndex(-1);
		parsers_combobox->setPlaceholderText("Select a Notation System Parser");
		for(int i=0; i<pf->parsers.count(); ++i)
		{
			parsers_combobox->setIconSize(QSize(24, 24));
			
			// @todo %2.svg => IDE understands as format specifier for "%2.s"
			// When icon is not found, it will throw message in console.
			QString parser = pf->parsers[i]->name();
			QString identifier = pf->parsers[i]->identifier();

			QString icon = QString(icon_at).arg(u->identifier).arg(identifier);
			QFileInfo fileInfo(icon);
			// if (!fileInfo.exists() || !fileInfo.isFile())
			// {
			// 	// copy icon from first parser, hindustani?
			// 	// 0-th icon
			// 	icon = QString(icon_at).arg(u->identifier).arg(pf->parsers[0]->identifier());
				
			// 	// even here, if file not found, debug!
			// 	// qDebug() << "Event the alternative icon was not found."
			// }

			parsers_combobox->addItem(QIcon(icon), parser, identifier);
		}

		connect(parsers_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MelodyEditorView::onComboBoxIndexChanged);

		QPushButton *button = new QPushButton("Update MIDI Clip");
		button->setMinimumHeight(ui_height);
		connect(button, &QPushButton::clicked, this, &MelodyEditorView::updateMidiClip);

		QPushButton *writeNow = new QPushButton("Write Now");
		writeNow->setMinimumHeight(ui_height);
		writeNow->setObjectName("writeNow");
		connect(writeNow, &QPushButton::clicked, this, &MelodyEditorView::flagWriteNow);

		MelodyEditorEventFilter* filter = new MelodyEditorEventFilter(this);
		this->pte->installEventFilter(filter); // key press at PTE will live update
		writeNow->installEventFilter(filter); // button press will flag live writing

		this->error->setMinimumHeight(ui_height);
		this->error->setMaximumHeight(ui_height);
		this->error->setReadOnly(true);

		QVBoxLayout *layout0 = new QVBoxLayout(nullptr);
		layout0->addWidget(this->pte);
		layout0->addWidget(this->error);

		QHBoxLayout *layout1 = new QHBoxLayout(nullptr);
		layout1->addWidget(parsers_combobox);
		layout1->addWidget(button);
		layout1->addWidget(writeNow);

		QVBoxLayout *layout2 = new QVBoxLayout(nullptr);
		layout2->addLayout(layout0);
		layout2->addLayout(layout1);
		setLayout(layout2);

		this->hide();
		QWidget* pw = parentWidget();
		if (pw!=nullptr)
		{
			pw->hide(); // default hidden
			pw->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);

			Qt::WindowFlags flags = pw->windowFlags();
			//flags |= Qt::MSWindowsFixedSizeDialogHint; // let user to change it
			flags &= ~Qt::WindowMaximizeButtonHint;
			flags |= Qt::WindowStaysOnTopHint;
			pw->setWindowFlags(flags);
			pw->resize(550, 300);
			//pw->setFixedSize(this->sizeHint());
			//pw->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		}
	}

	void MelodyEditorView::onComboBoxIndexChanged(int index)
	{
		this->parser_id = index;
	}

	void MelodyEditorView::flagWriteNow()
	{
		// if live mode was disabled, enable it.
		// if keyboiard typing on write area,
		// update midi clip
		this->pte->flagWriteNow();
	}

	void MelodyEditorView::updateMidiClip()
	{
		QList<QString> messages = {};

		// Do not process too long melodies. Save CPU.
		// Prevent accidental freezing, or data loss.
		QString userNotations = this->pte->toPlainText();
		if(userNotations.length() > u->MAX_INPUT_LIMIT)
		{
			// may cause the last note to be weird, though, still safe.
			userNotations = userNotations.left(u->MAX_INPUT_LIMIT);
			messages.append("Too long text to process. Trimmed.");
		}

		/**
		 * try/catch now allows the multiple calls even when there was an error,
		 * without crashing the host.
		 */
		try
		{
			if(this->parser_id>=0)
			{
				AbstractParser *parser = this->pf->parsers[this->parser_id];
				QList<NotationCell *> cells = parser->parse(userNotations);

				QString xpt_xml = "";
				parser->cells_to_xml(cells, xpt_xml);

				QDomDocument *root = new QDomDocument();
				root->setContent(xpt_xml);
				const QDomElement midiclipnodes = root->documentElement(); // <midiclip/>
				if(midiclipnodes.hasChildNodes()) // <note/>[]
				{
					// Similar to MIDI Import Clip, using xpt
					// This casues a minimal change to patch original LMMS Codes.
					GuiApplication *application = getGUI();
					PianoRollWindow *m_pianoRoll = application->pianoRoll();
					MidiClip *m_midiClip = const_cast<MidiClip*>(m_pianoRoll->currentMidiClip());
					if(m_midiClip!=nullptr)
					{
						TimePos pos = m_midiClip->startPosition();
						m_midiClip->loadSettings(midiclipnodes);
						m_midiClip->updateLength();
						m_midiClip->movePosition(pos);
					}
					else
					{
						messages.append("Open a Piano-Roll Window first.");
					}
				}
				else
				{
					messages.append("Invalid or Empty Notations!");
				}
			}
			else
			{
				messages.append("Parser was NOT selected.");
			}
		}
		catch(const std::exception& e)
		{
			messages.append(e.what());
		}

		if(messages.count()>0)
		{
			this->error->setText(messages.join("; "));
		}
	}

	void MelodyEditorView::closeEvent(QCloseEvent*)
	{
		qDebug() << "MelodyEditor Closed!";
	}

} // namespace lmms::gui
