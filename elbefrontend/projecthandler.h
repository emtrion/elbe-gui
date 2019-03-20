#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QString>

class ElbeHandler;
class ProjectManager;
class XmlFileManager;


class ProjectHandler
{
	public:
		ProjectHandler();
		ProjectHandler(QString path, QString name);
		void createProject();
		void closeProject();
		void openProject(QString path);
		void deleteProject(QString path);

		ProjectManager *getProjectmanager() const;


	private:
		QString newProjectPath;
		QString newProjectName;
		ProjectManager *projectmanager;
		ElbeHandler *elbehandler;
		bool checkIfProjectHasXML(QString path);
		XmlFileManager *filemanager;
};

#endif // PROJECTHANDLER_H
