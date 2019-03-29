#ifndef XMLUTILITIES_H
#define XMLUTILITIES_H

class QString;
class QFile;
class QDomDocument;
class QStringList;
class QByteArray;

namespace xmlUtilities {

	QDomDocument parseXMLFile(QFile *file);
	bool saveXMLChanges(QFile *file, QByteArray byteArray);
	QString getProjectID(QString confFile);
	QString getProjectName(QString confFile);
	QString parseProjectDescriptionForNodeValue(QString confFile, QString node);
	QStringList getImageFiles(QString buildXmlPath);
	bool setProjectMetadata(QString projectName, QString projectPath);
}

#endif // XMLUTILITIES_H
