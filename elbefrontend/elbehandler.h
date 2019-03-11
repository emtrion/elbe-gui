#ifndef ELBEHANDLER_H
#define ELBEHANDLER_H

#include "projectmanager.h"

#include <QProcessEnvironment>
#include <QString>



class ElbeHandler
{
	public:
		ElbeHandler();
		QString createProjectElbeInstance();
		QString checkSystemForElbeVersion();
		bool deleteProjectElbeInstance(QString projectID);

		bool projectIsInElbe(QString projectPath);
		void startBuildProcess();
		bool setXmlFile(QString file, QString elbeID);

		bool getFiles(QStringList filenames);
		bool getFile(QString filename);
		bool getImages();

		bool getFiles(QStringList filenames, QString outPath, QString elbeID);
		bool getFile(QString filename, QString outPath, QString elbeID);
		bool getImages(QString buildXmlPath, QString outPath, QString elbeID);


		void removeFalseLoadedFiles(QString outPath, QString filename);
	private:
		QProcess elbeProcess;
		QStringList getElbeProjects();
		QString execCommand(QString command);
		ProjectManager *projectmanager;

		bool getFiles_p(QStringList filenames, QString outPath, QString elbeID);
		bool getFile_p(QString filename, QString outPath, QString elbeID);
		bool getImages_p(QString buildXmlPath, QString outPath, QString elbeID);
};

#endif // ELBEHANDLER_H
