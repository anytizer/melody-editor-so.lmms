/**
 * MelodyEditor.cpp
 *
 * Copyright (c) 2026 Bimal Poudel <anytizer@users.noreply.github.com>
 * Copyright (c) 2026 Alex <allejok96@users.noreply.github.com>
 */

#include <QWidget>

#include "MelodyEditorView.h"
#include "MelodyEditor.h"

#include <QPlainTextDocumentLayout>
#include <QTextDocument>

#include <QDebug>

#include "ComboBoxModel.h"
#include "embed.h"
#include "plugin_export.h"

#include "src/includes/Utilities.h"
#include "src/parsers/AbstractParser.h"
#include "src/parsers/AldaParser.h"
#include "src/parsers/SimpleParser.h"

using lmms::gui::MelodyEditorView;
using namespace lmms::melodyeditor;

namespace lmms
{

extern "C"
{

	Plugin::Descriptor PLUGIN_EXPORT melodyeditor_plugin_descriptor = {
		LMMS_STRINGIFY(PLUGIN_NAME),
		"&Melody Editor",
		QT_TRANSLATE_NOOP("PluginBrowser", "Melody Editor"),
		"Bimal Poudel <anytizer@users.noreply.github.com>",
		0x0100,
		Plugin::Type::Tool,
		new PluginPixmapLoader("logo"),
		"txt", // nullptr,
		nullptr,
	};

	PLUGIN_EXPORT Plugin* lmms_plugin_main(Model*, void*)
	{
		return new MelodyEditor();
	}

} // extern "C"


MelodyEditor::MelodyEditor()
	: ToolPlugin(&melodyeditor_plugin_descriptor, nullptr)
	, m_parsers{
		new AldaParser(),
		new SimpleParser(ENGLISH_DIALECT),
		new SimpleParser(HINDUSTANI_DIALECT),
		//new SimpleParser(CARNATIC_DIALECT)
	}
	, m_parserModel(new ComboBoxModel(this, "Parser"))
	, m_liveCodingModel(new BoolModel(true, this, "Live coding"))
	, m_document(new QTextDocument(this))
	, m_log(new QTextDocument(this))
{
	m_document->setDocumentLayout(new QPlainTextDocumentLayout(m_document));
	m_log->setDocumentLayout(new QPlainTextDocumentLayout(m_log));

	for(auto parser : m_parsers)
	{
		auto icon = std::make_unique<PluginPixmapLoader>(parser->icon());
		m_parserModel->addItem(parser->name(), std::move(icon));
	}

	connect(m_parserModel, &Model::dataChanged, this, &MelodyEditor::parse);
    connect(m_document, &QTextDocument::contentsChanged, this, &MelodyEditor::parse);
}

QString MelodyEditor::nodeName() const
{
	return melodyeditor_plugin_descriptor.name;
}

PluginView* MelodyEditor::instantiateView(QWidget*)
{
	return new MelodyEditorView(this);
}




void MelodyEditor::loadFile(const QString& filename)
{
	m_document->setPlainText(fileContents(filename));
	m_file = filename;
}




void MelodyEditor::importFromClip()
{
	if (!m_midiClip) return m_log->setPlainText("Open a clip in Piano Roll first");
	parser()->importFromClip(m_midiClip);

	return m_log->setPlainText(parser()->logMessages());
}


void MelodyEditor::parseNotations(QString notations)
{
	notations.replace('\r', '\n');
	
	// temporarily strip non-ascii
	/**
	// strip out unicodes and keep ascii only
	QString output;
    for (const QChar &qc : notations) {
        // ASCII in the range 0 to 127
        if (qc.unicode() >= 0 && qc.unicode() <= 127)
		{
            output += qc;
        }
    }
	notations = output;
	*/
	notations = notations.replace(QRegularExpression(QString("[^\\x00-\\x7F]")), "");

	try
	{
		parser()->parse(notations);
		
		// @todo Complete safety
		//if (parser()->isSafeToWrite(nullptr, {}))
		{
			parser()->write(m_midiClip);
		}
		m_log->setPlainText(parser()->logMessages());
	}
	catch (const ParserError& e)
	{
		m_log->setPlainText(e.what());
	}
}




void MelodyEditor::parse()
{
	if (!m_liveCodingModel->value()) return;

	// QString notations = m_document->toPlainText();
	// this->parseNotations(notations);
	this->parseNotations(m_document->toPlainText());
}




AbstractParser* MelodyEditor::parser()
{
	return m_parsers.at(m_parserModel->value());
}

} // namespace lmms
