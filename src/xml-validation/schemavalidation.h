#ifndef SCHEMAVALIDATION_H
#define SCHEMAVALIDATION_H

#include <QXmlSchema>
#include <QFile>

#include "messagehandler.h"
#include "src/app/helpers.h"

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

		void displaySchemaError();
		QByteArray modifySchema(QFile *file);
};

#endif // SCHEMAVALIDATION_H
