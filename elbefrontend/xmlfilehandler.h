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

		void handleFileModification(QString file);

		void openFile(QString filePath);
	private:
		QString filePath;
		QString fileName;
		XmlFileManager *filemanager;
		void openFile_p(QString path);
};
#endif // XMLFILEHANDLER_H
