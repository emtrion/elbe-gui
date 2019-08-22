#ifndef ELBEHANDLER_H
#define ELBEHANDLER_H

class QStringList;
class QString;
class QFile;

namespace ElbeHandler {

	void isVersionSupported();
	QString checkElbeVersion();

	QString createProjectElbeInstance();
	bool deleteProjectElbeInstance(QString projectID);
	bool projectIsInElbe(QString projectPath);
	bool projectIsBuilding(QString projectPath);

	bool startBuildProcess(bool sourceOptionChecked, bool binOptionChecked);
	bool setXmlFile(QString file, QString elbeID);

	bool getFiles(QStringList filenames);
	bool getFile(QString filename);
	bool getImages();
	bool getFiles(QStringList filenames, QString outPath, QString elbeID);
	bool getFile(QString filename, QString outPath, QString elbeID);
	bool getImages(QString buildXmlPath, QString outPath, QString elbeID);

	bool checkIfBusy(QString id);
	bool restartInitVM(const QString &initVM);

	QStringList buildUpdate();
	QString checkForUpdates();
	QString sourceXMLPath();

	QString prepareUpdateScript(QString);
	QString readUpdateScript();
	QString buildReferenceProject();
	void saveUpdateFiles(QString);


	/*------- these are defined in an unnamed namespace ---------------------------------------*
	 *																						   *
	 *  QStringList makeProjectList();											               *
	 * 	QString execCommand(QString command, int timeout, bool forceWithoutPrefix = false);	   *
	 * 	bool getFiles_p(QStringList filenames, QString outPath, QString elbeID);               *
	 * 	bool getFile_p(QString filename, QString outPath, QString elbeID);		               *
	 * 	bool getImages_p(QString buildXmlPath, QString outPath, QString elbeID);               *
	 * 	void removeFalseLoadedFiles(QString outPath, QString filename);			               *
	 *	  void forwardOutputToMessageLog(QString output);						               *
	 *																			               *
	 *-----------------------------------------------------------------------------------------*/

}

#endif // ELBEHANDLER_H
