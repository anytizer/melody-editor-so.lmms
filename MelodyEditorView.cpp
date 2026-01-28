/**
 * MelodyEditorView.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
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
#include <QColor>
#include <QSlider>

#include <QDebug>

#include "MelodyEditor.h"
#include "MelodyEditorView.h"
#include "MelodyEditorTextArea.h"
#include "src/includes/NotationsFormatter.h"
#include "src/includes/Utilities.h"

#include "ComboBox.h"
#include "FileDialog.h"
#include "Song.h"
#include "GuiApplication.h"
#include "PianoRoll.h"
#include "PatternStore.h"


using lmms::gui::melodyeditor::MelodyEditorTextArea;
using namespace lmms::melodyeditor;


namespace lmms::gui
{

	MelodyEditorView::MelodyEditorView(MelodyEditor* plugin)
		: ToolPluginView(plugin),
		m_plugin(plugin)
	{
		int buttonHeight = 32;

		this->setAcceptDrops(true);
		this->setMinimumSize(600, 400);

		connect(getGUI()->pianoRoll(), &PianoRollWindow::currentMidiClipChanged, this, &MelodyEditorView::setClipFromPianoRoll);
		this->setClipFromPianoRoll();

		auto textArea(new MelodyEditorTextArea());
		textArea->setDocument(m_plugin->m_document);
		connect(textArea, &MelodyEditorTextArea::fileDropped, m_plugin, &MelodyEditor::loadFile);
		connect(textArea, &MelodyEditorTextArea::doubleClicked, this, &MelodyEditorView::openNotationsFileSelector);

		// font size scaling by a slider
		QSlider* zoomSlider = new QSlider(Qt::Horizontal);
		zoomSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		//zoomSlider->setMinimumWidth(500);
		//zoomSlider->setMaximumWidth(400);
		zoomSlider->setFixedHeight(24);
		zoomSlider->setRange(MIN_FONTSIZE, MAX_FONTSIZE);
		zoomSlider->setValue(14);
		zoomSlider->setTickInterval(3);
		//zoomSlider->setTickPosition(QSlider::TicksBelow);
		connect(zoomSlider, &QSlider::valueChanged, this, [this, textArea](int value){
			// zoom is also controlled with Ctrl+Wheel
			QFont font = textArea->font();
			font.setPointSize(std::clamp(value, MIN_FONTSIZE, MAX_FONTSIZE));
			textArea->setFont(font);
			textArea->update();
		});

		auto languageLabel = new QLabel("Notation system:", this);
		auto languageBox = new ComboBox(this, "Select Notation System");
		languageBox->setModel(m_plugin->m_parserModel);

		auto errorBox(new QPlainTextEdit(this));
		errorBox->setDocument(m_plugin->m_log);
		errorBox->setReadOnly(true);
		// errorBox->setDisabled(true);
		QPalette p = errorBox->palette();
		//p.setColor(QPalette::Base, Qt::black);
		p.setColor(QPalette::Text, QColor("#e895c1"));
		errorBox->setPalette(p);

		QPushButton* writeButton = new QPushButton("Live Coding", this);
		writeButton->setToolTip("Write notes to Piano Roll");
		writeButton->setFixedHeight(buttonHeight);
		writeButton->setCheckable(true);
		writeButton->setChecked(m_plugin->m_liveCodingModel->value());
		writeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		writeButton->setStyleSheet("QPushButton:checked { background-color: #276a90; border-style: inset; }");
		connect(writeButton, &QPushButton::clicked, m_plugin, [this, plugin, writeButton]{
			bool writingNow = writeButton->isChecked();
			m_plugin->flag(writingNow);
			if(writingNow)
			{
				// also parse immediately, once; when checked status changed
				// @todo pass writingNow as parameter to parse().
				m_plugin->parse();
			}
		});

		auto openButton = new QPushButton("Open", this);
		openButton->setToolTip("Open melody text file");
		openButton->setFixedHeight(buttonHeight);
		openButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		connect(openButton, &QPushButton::clicked, this, &MelodyEditorView::openNotationsFileSelector);

		auto formatButton = new QPushButton("Format", this);
		formatButton->setToolTip("Refformatting to become printer friendly");
		formatButton->setFixedHeight(buttonHeight);
		formatButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		connect(formatButton, &QPushButton::clicked, this, &MelodyEditorView::formatNotes);

		// auto loadButton = new QPushButton("Import", this);
		// loadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		// loadButton->setToolTip("Import notes into Piano-Roll");
		// // connect(loadButton, &QPushButton::clicked, m_plugin, &MelodyEditorView::setClipFromPianoRoll);
		// //connect(loadButton, &QPushButton::clicked, m_plugin, &MelodyEditor::importFromClip);
		// connect(loadButton, &QPushButton::clicked, m_plugin, &MelodyEditor::parse);
		
		auto updateButton = new QPushButton("Update", this);		
		updateButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		updateButton->setToolTip("Update Piano-Roll");
		updateButton->setFixedHeight(buttonHeight);
		connect(updateButton, &QPushButton::clicked, m_plugin, [this, textArea]{
			QString notations = "";
			QTextCursor cursor = textArea->textCursor();

			if(cursor.hasSelection())
			{
				notations = cursor.selectedText();
			}
			else
			{
				notations = textArea->toPlainText();
			}

			m_plugin->parseNotations(notations);
		});

		auto mainLayout = new QVBoxLayout(this);
		auto sliderLayout = new QHBoxLayout(nullptr);
		auto languageLayout = new QHBoxLayout(nullptr);
		auto splitter = new QSplitter(Qt::Vertical, this);
		auto buttonLayout = new QHBoxLayout(nullptr);
		splitter->addWidget(textArea);
		splitter->addWidget(errorBox);
		splitter->setStretchFactor(0,20);
		splitter->setStretchFactor(1,1);
		sliderLayout->addWidget(zoomSlider);
		languageLayout->addWidget(languageLabel);
		languageLayout->addWidget(languageBox);
		buttonLayout->addWidget(writeButton);
		buttonLayout->addWidget(openButton);
		buttonLayout->addWidget(formatButton);
		//buttonLayout->addWidget(loadButton);
		buttonLayout->addWidget(updateButton);
		mainLayout->addLayout(sliderLayout);
		mainLayout->addLayout(languageLayout);
		mainLayout->addWidget(splitter);
		mainLayout->addLayout(buttonLayout);
		this->setLayout(mainLayout);
		
		//this->hide();
		QWidget* pw = parentWidget();
		if (pw!=nullptr)
		{
			pw->hide(); // default hidden

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
			QString basename = QFileInfo(filename).fileName();

			if (!sizeCheck(filename))
			{
				m_plugin->m_log->setPlainText(QString("%1: %2").arg("File is too large to load").arg(basename));
				return;
			}

			m_plugin->loadFile(filename);
		}
	}


	void MelodyEditorView::formatNotes()
	{
		QString notations = m_plugin->m_document->toPlainText();
		NotationsFormatter* nf = new NotationsFormatter();
		notations = nf->format(notations);

		m_plugin->m_document->setPlainText(notations);
	}

} // namespace lmms::gui
