#ifndef ELBEHANDLER_H
#define ELBEHANDLER_H

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
	private:
		QProcess elbeProcess;
		QStringList getElbeProjects();
		QString execCommand(QString command);
};

#endif // ELBEHANDLER_H
