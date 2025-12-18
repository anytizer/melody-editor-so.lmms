/**
 * MelodyEditorView.cpp
 *
 * Copyright (c) 2025 - 2025 Bimal Poudel <anytizer@users.noreply.github.com>
 */

#include "MelodyEditor.h"
#include "MelodyEditorView.h"

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
#include <QPlainTextEdit>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QEvent>
#include <QDebug>

#include "src/includes/DataStructures.h"
#include "src/includes/Utilities.h"
#include "src/parsers/ParsersFactory.h"
#include "src/parsers/MissingParser.h"
#include "src/parsers/HindustaniParser.h"
#include "src/parsers/CarnaticParser.h"
#include "src/parsers/EnglishParser.h"
#include "src/parsers/GermanParser.h"
#include "src/parsers/NashvilleParser.h"
#include "src/parsers/VirtualpianoParser.h"

#include "Engine.h"
#include "Song.h"
#include "GuiApplication.h"
#include "PianoRoll.h"
#include "MidiClip.h"
#include "Clip.h"
#include "MidiClipView.h"
#include "Editor.h"

using lmms::gui::editor::pianoroll::parsing::Utilities;
using lmms::gui::editor::pianoroll::parsing::AbstractParser;
using lmms::gui::PianoRollWindow;
using lmms::MidiClip;

namespace lmms::gui
{

	MelodyEditorView::MelodyEditorView(MelodyEditor* plugin)
		: ToolPluginView(plugin),
		m_plugin(plugin)
	{
		this->setAcceptDrops(true);

		this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		this->setFixedSize(520, 400); // Original default: 500 x 400;
		// later calculated to be in the center screen.
		// @todo: Make it of height as initial Piano Roll
		// INITIAL_PIANO_ROLL_HEIGHT

		int ui_height = 32; // height for combo box and button

		Utilities* u = new Utilities();

		QComboBox *parsers_combobox = new QComboBox(this); // text | data
		parsers_combobox->setEditable(false);
		parsers_combobox->setMinimumHeight(ui_height);
		parsers_combobox->setPlaceholderText("Select Notation System");
		parsers_combobox->setCurrentIndex(-1);
		for(int i=0; i<pf->parsers.count(); ++i)
		{
			parsers_combobox->setIconSize(QSize(24, 24));
			
			// @todo %2.svg => IDE understands as format specifier for "%2.s"
			// When icon is not found, it will throw message in console.
			QString parser = pf->parsers[i]->name();
			QString identifier = pf->parsers[i]->identifier();
			
			// %1 = melodyeditor, %2 = sargam|etc.
			// @see icons.qrc, CMakeLists.txt
			QString icon = QString(":/artwork/%1/ns-%2").arg(u->identifier).arg(identifier);
			parsers_combobox->addItem(QIcon(icon), parser, identifier);
		}

		QObject::connect(parsers_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
		[this, parsers_combobox](int index) {
			this->parser_id = index;
		});

		QPushButton *button = new QPushButton("Update MIDI Clip");
		button->setMinimumHeight(ui_height);
		connect(button, &QPushButton::clicked, this, &MelodyEditorView::updateMidiClip);

		QHBoxLayout *layout0 = new QHBoxLayout(nullptr);
		layout0->addWidget(pte);

		QHBoxLayout *layout2 = new QHBoxLayout(nullptr);
		layout2->addWidget(parsers_combobox);
		layout2->addWidget(button);

		QVBoxLayout *layout = new QVBoxLayout(nullptr);
		layout->addLayout(layout0);
		layout->addLayout(layout2);
		this->setLayout(layout);

		QWidget* pw = parentWidget();
		if (pw!=nullptr)
		{
			pw->hide(); // default hidden
			pw->layout()->setSizeConstraint(QLayout::SetFixedSize);

			Qt::WindowFlags flags = pw->windowFlags();
			flags |= Qt::MSWindowsFixedSizeDialogHint;
			flags &= ~Qt::WindowMaximizeButtonHint;
			flags |= Qt::WindowStaysOnTopHint;
			pw->setWindowFlags(flags);
		}
	}

	void MelodyEditorView::updateMidiClip()
	{
		QList<QString> messages = {};

		Utilities* u = new Utilities();

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
			if(this->parser_id != -1)
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
					MidiClip *m_midiClip = m_pianoRoll->currentMidiClip();
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
			QMessageBox::information(this, "Error / Output", messages.join("\n"));
		}
	}

	void MelodyEditorView::closeEvent(QCloseEvent*)
	{
		qDebug() << "MelodyEditor Closed!";
	}

} // namespace lmms::gui
