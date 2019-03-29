#ifndef XMLFILEHANDLER_H
#define XMLFILEHANDLER_H


#include <QString>

class XmlFile;
class Project;

namespace XmlFileHandler {

	void openFile(const QString filePath);
	void createFile(const QString projectPath, const QString fileName);
	void XMLautoGenerate(const QString filePath);
	void saveFile();
	void closeFile();
	void handleFileModification(QString file);
}


#endif // XMLFILEHANDLER_H
