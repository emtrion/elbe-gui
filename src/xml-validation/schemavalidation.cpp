#include "schemavalidation.h"

#include <QApplication>
#include <QTextEdit>
#include <QDebug>
#include <QXmlSchemaValidator>

#include "src/mainwindow/mainwindow.h"
#include "src/editor/codeeditor.h"

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

//	QFile file(":/schema/schema.xsd");

	//"msdoshd" section is commented out due to errors caused by this section during the validation process
	//QSchemaValidator says schema is invalid because it has non-deterministic content at this section
	QFile file(":/schemaModified.xsd");
	file.open(QIODevice::ReadOnly);

	schema.setMessageHandler(&messageHandler);
	schema.load(&file, QUrl::fromLocalFile(file.fileName()));

	if ( schema.isValid() ) {
		file.close();
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
		errorOccured = true;
	} else {
		QXmlSchemaValidator valildator(schema);
		if ( !valildator.validate(instanceFile)){
			errorOccured = true;
		}
	}
	QApplication::restoreOverrideCursor();
	if ( errorOccured ) {
		moveCursor(messageHandler.line(), messageHandler.column());
	} else {
		//file is valid
	}
	displayValidationMessage(errorOccured);
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
