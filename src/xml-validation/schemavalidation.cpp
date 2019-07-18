#include "schemavalidation.h"

#include <QApplication>
#include <QTextEdit>
#include <QDebug>
#include <QXmlSchemaValidator>
#include <QFile>
#include <QTemporaryFile>
#include <QRegularExpression>

#include <QThread>

#include "src/mainwindow/mainwindow.h"
#include "src/editor/codeeditor.h"

#include <src/app/applicationconfig.h>

SchemaValidation::SchemaValidation(QFile instanceFile)
{
	instanceFile.open(QIODevice::ReadOnly);
	this->instanceFile = instanceFile.readAll();
	instanceFile.close();
}

SchemaValidation::SchemaValidation(QString fileContent)
{
	this->instanceFile = fileContent.toUtf8();
}


bool SchemaValidation::loadSchema()
{

	ApplicationConfig *appconf = new ApplicationConfig();
	QByteArray data;
	data = modifySchema(new QFile(appconf->schemaFile()));

	schema.setMessageHandler(&messageHandler);
	schema.load(data);

	if ( schema.isValid() ) {
		return true;
	} else {
		return false;
	}
}

void SchemaValidation::validate()
{
	bool errorOccured = false;
	//show wait cursor while validating because it can take some time
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if ( !loadSchema() ) {
		displaySchemaError();
	} else {
		QXmlSchemaValidator valildator(schema);
		if ( !valildator.validate(instanceFile)){
			moveCursor(messageHandler.line(), messageHandler.column());
			errorOccured = true;
		}
		displayValidationMessage(errorOccured);
	}

	QApplication::restoreOverrideCursor();
}

void SchemaValidation::displaySchemaError()
{
	QColor color;
	QTextEdit *messageLog = mainwindow->messageLog();
	QString errorMsg = messageHandler.statusMessage().remove(QRegExp("<[^>]*>"));
	QString line = QString::number(messageHandler.line());
	QString column = QString::number(messageHandler.column());
	errorMsg.append("  At Line: "+line+", Column: "+column);

	color.setNamedColor("#f66464");
	messageLog->setTextColor(color);
	messageLog->setText(errorMsg);
}

void SchemaValidation::displayValidationMessage(bool errorOccured)
{
	QColor color;

	QTextEdit *messageLog = mainwindow->messageLog();
	//make the error message readable
	QString errorMsg = messageHandler.statusMessage().remove(QRegExp("<[^>]*>"));

	if ( errorOccured ){
		color.setNamedColor("#f66464");
		messageLog->setTextColor(color);
		messageLog->setText(errorMsg);
	} else {
		color.setNamedColor("#5f8700");
		messageLog->setTextColor(color);
		messageLog->setPlainText("validation successful");
	}
}

void SchemaValidation::moveCursor(int line, int column)
{
	CodeEditor *ce = mainwindow->editor();
	ce->moveCursor(QTextCursor::Start);
	for (auto i = 1; i < line; ++i) {
		ce->moveCursor(QTextCursor::Down);
	}

	for (auto i = 1; i < column; ++i) {
		ce->moveCursor(QTextCursor::Right);
	}

	QList<QTextEdit::ExtraSelection> extraSelections;
	QTextEdit::ExtraSelection selection;

	const QColor lineColor = QColor(Qt::red).lighter(80);
	selection.format.setBackground(lineColor);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	selection.cursor = ce->textCursor();
	selection.cursor.clearSelection();
	extraSelections.append(selection);

	ce->setExtraSelections(extraSelections);

	ce->setFocus();
}


QByteArray SchemaValidation::modifySchema(QFile *file)
{
	if ( !file->open(QIODevice::ReadOnly)) {
		qDebug() << "Could not open file";
	}

	QByteArray fileData = file->readAll();
	file->close();
	QString text(fileData);

	text.replace(QString("<all>"), QString("<sequence>"));
	text.replace(QString("</all>"), QString("</sequence>"));

	QFile snippet(":/schemaSnippet.xml");
	snippet.open(QIODevice::ReadOnly);
	QByteArray snippetData = snippet.readAll();
	snippet.close();

	QRegularExpression rx;
	rx.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
	//matches the part in the schema which is causing an error during validation, so it can be replaced with a fix
	rx.setPattern("<element name=\"partition\" type=\"rfs:partition\" minOccurs=\"0\" maxOccurs=\"4\">.+?<\/element>.+?(?=<\/sequence>)");
	QRegularExpressionMatch m = rx.match(text, 0, QRegularExpression::NormalMatch, QRegularExpression::NoMatchOption);

	text.replace(rx, QString::fromUtf8(snippetData));

	return text.toUtf8();
}


