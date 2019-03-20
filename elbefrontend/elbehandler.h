#ifndef ELBEHANDLER_H
#define ELBEHANDLER_H

class QStringList;
class QString;

class ElbeHandler
{
	public:
		ElbeHandler();
		QString createProjectElbeInstance();
		QString checkSystemForElbeVersion();
		bool deleteProjectElbeInstance(QString projectID);

		bool projectIsInElbe(QString projectPath);
		bool startBuildProcess(bool sourceOptionChecked, bool binOptionChecked);
		bool setXmlFile(QString file, QString elbeID);

		bool getFiles(QStringList filenames);
		bool getFile(QString filename);
		bool getImages();

		bool getFiles(QStringList filenames, QString outPath, QString elbeID);
		bool getFile(QString filename, QString outPath, QString elbeID);
		bool getImages(QString buildXmlPath, QString outPath, QString elbeID);


		void removeFalseLoadedFiles(QString outPath, QString filename);
		bool checkIfBusy(QString id);
	private:
		QStringList makeProjectList();
		QString execCommand(QString command, int timeout = 30000);
		bool getFiles_p(QStringList filenames, QString outPath, QString elbeID);
		bool getFile_p(QString filename, QString outPath, QString elbeID);
		bool getImages_p(QString buildXmlPath, QString outPath, QString elbeID);
};

#endif // ELBEHANDLER_H
