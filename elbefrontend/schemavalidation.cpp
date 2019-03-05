#include "schemavalidation.h"

#include <QApplication>
#include <QFile>
#include <QXmlSchema>
#include <QDebug>
#include <QUrl>
#include <QString>
#include <QXmlSchemaValidator>
#include <QSourceLocation>
#include <QAbstractMessageHandler>
#include <QTime>
#include "helpers.h"

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

//	QFile file(":/schema.xsd");
	QFile file(":/schemaModified.xsd");
	file.open(QIODevice::ReadOnly);

	schema.setMessageHandler(&messageHandler);

	schema.load(&file, QUrl::fromLocalFile(file.fileName()));

	if ( schema.isValid() ) {
		qDebug() << "schema is valid";
		file.close();
		return true;
	} else {
		qDebug() << "schema is invalid";
		//TODO: Insert errordialog
		file.close();
		return false;
	}
}


void SchemaValidation::validate()
{
//	qDebug() << "validate start";

	bool errorOccured = false;
	QApplication::setOverrideCursor(Qt::WaitCursor); //show wait cursor while validating because it can take some time
	if ( !loadSchema() ) {
		errorOccured = true;
	} else {
//		qDebug() << "schema loaded";

		QXmlSchemaValidator valildator(schema);
		if ( !valildator.validate(instanceFile)){
			errorOccured = true;
		}
	}
	QApplication::restoreOverrideCursor(); //restore cursor
	if ( errorOccured ) {
		moveCursor(messageHandler.line(), messageHandler.column());
		qDebug() << "File is not valid";
		//TODO:Insert errordialog

	} else {
		qDebug() << "File is valid!";
	}
	displayValidationMessage(errorOccured);
}

void SchemaValidation::displayValidationMessage(bool errorOccured)
{
	QColor color;
	MainWindow *mw = helpers::getMainWindow();
//	QTextEdit *messageLog = mainwindow->getMessageLog();
	if ( errorOccured ){
		color.setNamedColor("#d30102");
//		messageLog->setTextColor(color);
//		messageLog->setText(messageHandler.statusMessage());
		mw->setStatusBarTextColor(color);
		mw->showTempStatusOnStatusBar(messageHandler.statusMessage());
	} else {
		color.setNamedColor("#5f8700");
//		messageLog->setTextColor(color);
//		messageLog->setPlainText("validation successful");
		mw->showTempStatusOnStatusBar("validation successful");
	}
}


void SchemaValidation::moveCursor(int line, int column)
{

	CodeEditor *ce = mainwindow->getEditor();
	ce->moveCursor(QTextCursor::Start);
	for (int i = 1; i < line; ++i) {
		ce->moveCursor(QTextCursor::Down);
	}

	for (int i = 1; i < column; ++i) {
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
