/**
 * MelodyEditorView.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 */
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDomDocument>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QLabel>
#include <QTextStream>
#include <QEvent>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QDebug>

#include "MelodyEditor.h"
#include "MelodyEditorView.h"
#include "MelodyEditorTextArea.h"
#include "src/includes/Utilities.h"

#include "ComboBox.h"
#include "FileDialog.h"
#include "Song.h"
#include "GuiApplication.h"
#include "PianoRoll.h"
#include "PatternStore.h"


using namespace lmms::melodyeditor;
using lmms::gui::melodyeditor::MelodyEditorTextArea;


namespace lmms::gui
{

	MelodyEditorView::MelodyEditorView(MelodyEditor* plugin)
		: ToolPluginView(plugin),
		m_plugin(plugin)
	{
		this->setAcceptDrops(true);
		this->setMinimumSize(450, 450);

		connect(getGUI()->pianoRoll(), &PianoRollWindow::currentMidiClipChanged, this, &MelodyEditorView::setClipFromPianoRoll);
		this->setClipFromPianoRoll();

		auto textArea(new MelodyEditorTextArea());
		textArea->setDocument(m_plugin->m_document);
		connect(textArea, &MelodyEditorTextArea::fileDropped, m_plugin, &MelodyEditor::loadFile);

		auto languageLabel = new QLabel("Notation system:", this);

		auto languageBox = new ComboBox(this, "Select Notation System");
		languageBox->setModel(m_plugin->m_parserModel);

		auto errorBox(new QPlainTextEdit(this));
		errorBox->setDocument(m_plugin->m_log);
		//m_errorBox->setDisabled(true);

		QPushButton* writeButton = new QPushButton("Live coding", this);
		writeButton->setToolTip("Write notes to Piano Roll");
		writeButton->setCheckable(true);
		writeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		writeButton->setStyleSheet("QPushButton:checked { background-color: #276a90; border-style: inset; }");
		connect(writeButton, &QPushButton::clicked, m_plugin, [this, plugin, writeButton]{
			bool now = writeButton->isChecked();
			m_plugin->flag(now);
			if(now) m_plugin->parse(); // also parse immediately
			qDebug() << "Live coding: " << writeButton->isChecked();
		});

		auto openButton = new QPushButton("Open", this);
		openButton->setToolTip("Open melody text file");
		openButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		connect(openButton, &QPushButton::clicked, this, &MelodyEditorView::openNotationsFileSelector);

		auto loadButton = new QPushButton("Import", this);
		loadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		loadButton->setToolTip("Import notes into Piano-Roll");
		//connect(loadButton, &QPushButton::clicked, m_plugin, &MelodyEditorView::setClipFromPianoRoll);
		connect(loadButton, &QPushButton::clicked, m_plugin, &MelodyEditor::importFromClip);

		auto mainLayout = new QVBoxLayout(this);
		auto splitter = new QSplitter(Qt::Vertical, this);
		auto languageLayout = new QHBoxLayout(this);
		auto buttonLayout = new QHBoxLayout(this);
		splitter->addWidget(textArea);
		splitter->addWidget(errorBox);
		splitter->setStretchFactor(0,10);
		splitter->setStretchFactor(1,1);
		languageLayout->addWidget(languageLabel);
		languageLayout->addWidget(languageBox);
		buttonLayout->addWidget(writeButton);
		buttonLayout->addWidget(openButton);
		buttonLayout->addWidget(loadButton);
		mainLayout->addWidget(splitter);
		mainLayout->addLayout(languageLayout);
		mainLayout->addLayout(buttonLayout);
		this->setLayout(mainLayout);
		
		//this->hide();
		QWidget* pw = parentWidget();
		if (pw!=nullptr)
		{
			pw->hide(); // default hidden
			//pw->layout()->setSizeConstraint(QLayout::SetFixedSize);

			Qt::WindowFlags flags = pw->windowFlags();
			//flags |= Qt::MSWindowsFixedSizeDialogHint;
			flags |= Qt::WindowMaximizeButtonHint;
			flags |= Qt::WindowStaysOnTopHint;
			pw->setWindowFlags(flags);

			pw->adjustSize();
		}
	}


	//! Hacky way to get a writable MidiClip pointer from PianoRollWindow
	void MelodyEditorView::setClipFromPianoRoll()
	{
		const MidiClip* currentMidiClip = getGUI()->pianoRoll()->currentMidiClip();

		const auto trackContainers = std::initializer_list<TrackContainer*>{
			Engine::getSong(),
			Engine::patternStore(),
		};

		for (auto* trackContainer: trackContainers)
		{
			for (auto* track: trackContainer->tracks())
			{
				if (track->type() != Track::Type::Instrument) { continue; }
				for (auto* clip: track->getClips())
				{
					if (static_cast<const MidiClip*>(clip) == currentMidiClip)
					{
						m_plugin->setMidiClip(static_cast<MidiClip*>(clip));
						return;
					}
				}
			}
		}
		m_plugin->setMidiClip(nullptr);
		m_plugin->m_log->clear();
	}


	void MelodyEditorView::openNotationsFileSelector()
	{
		QString dir = m_plugin->m_file.isEmpty() ? "" : QFileInfo(m_plugin->m_file).dir().path();
		FileDialog ofd( this, "Open melody notations", "", "Melodies (*.txt)");
		ofd.setFileMode(FileDialog::ExistingFiles);
		if( ofd.exec () == QDialog::Accepted && !ofd.selectedFiles().isEmpty() )
		{
			auto filename = ofd.selectedFiles()[0];

			if (!sizeCheck(filename))
			{
				QMessageBox::critical(this, "Error", "The file you are trying to load is too large");
				return;
			}

			m_plugin->loadFile(filename);
		}
	}


} // namespace lmms::gui
