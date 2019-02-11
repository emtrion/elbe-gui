#ifndef SCHEMAVALIDATION_H
#define SCHEMAVALIDATION_H

#include <QXmlSchema>
#include <QFile>
#include "messagehandler.h"
#include "mainwindow.h"
#include "helpers.h"

class SchemaValidation
{
	public:
		SchemaValidation(QFile instanceFile);
		SchemaValidation(QString fileContent);
		void validate();

	private:
		QXmlSchema schema;
		QByteArray instanceFile;
		bool loadSchema();
		MessageHandler messageHandler;
		void moveCursor(int line, int column);
		void displayValidationMessage(bool errorOccured);
		const MainWindow *mainwindow = helpers::getMainWindow();

};

#endif // SCHEMAVALIDATION_H
