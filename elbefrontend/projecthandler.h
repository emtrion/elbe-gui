#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QString>
#include "projectmanager.h"


class ProjectHandler
{
	public:
		ProjectHandler();
		ProjectHandler(QString path, QString name);
		void createProject();
		void closeProject();
		void openProject(QString path);

		ProjectManager *getProjectmanager() const;

	private:
		QString newProjectPath;
		QString newProjectName;
		ProjectManager *projectmanager;
};

#endif // PROJECTHANDLER_H
