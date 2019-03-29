#ifndef SCHEMAVALIDATION_H
#define SCHEMAVALIDATION_H

#include <QXmlSchema>
#include <QFile>

#include "messagehandler.h"
#include "helpers.h"

class MainWindow;

class SchemaValidation
{
	public:
		SchemaValidation(QFile instanceFile);
		SchemaValidation(QString fileContent);
		void validate();

	private:
		QXmlSchema schema;
		QByteArray instanceFile;
		MessageHandler messageHandler;

	private:
		void moveCursor(int line, int column);
		void displayValidationMessage(bool errorOccured);
		const MainWindow *mainwindow = helpers::getMainWindow();
		bool loadSchema();

};

#endif // SCHEMAVALIDATION_H
