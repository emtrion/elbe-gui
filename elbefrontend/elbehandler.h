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
	private:
		QProcess elbeProcess;
		QStringList getElbeProjects();
};

#endif // ELBEHANDLER_H
