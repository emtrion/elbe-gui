#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QString>

class Project;
class XmlFile;

namespace ProjectHandler {

	void createProject(const QString newProjectPath, const QString newProjectName);
	void closeProject();
	void openProject(QString path);
	void deleteProject(QString path);
	bool checkIfProjectHasXML(QString path);
}

#endif // PROJECTHANDLER_H
