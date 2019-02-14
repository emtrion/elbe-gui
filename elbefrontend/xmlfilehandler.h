#ifndef XMLFILEHANDLER_H
#define XMLFILEHANDLER_H

#include <QString>

#include "xmlfilemanager.h"

class XmlFileHandler
{
	public:
		XmlFileHandler();
		XmlFileHandler(QString path, QString name);
		XmlFileHandler(QString file);
		~XmlFileHandler();
		void openFile();
		void createFile();
		void XMLautoGenerate();
		void saveFile();
		void closeFile();

	private:
		QString filePath;
		QString fileName;

		XmlFileManager *filemanager;
};
#endif // XMLFILEHANDLER_H
