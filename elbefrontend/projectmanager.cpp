#include "projectmanager.h"
#include "helpers.h"
#include <QDebug>
/* This is a singelton class...
 * because their can always be just one open project at the time.
 * Should another project be opened, the previous will be closed and the instance
 * of this class will be deleted and new assigned*/

/* Null, because instance will be initialized on demand. */
ProjectManager* ProjectManager::instance = 0;

QString ProjectManager::getSrcPath() const
{
	return srcPath;
}

QString ProjectManager::getOutPath() const
{
	return outPath;
}

ProjectManager::ProjectManager()
{}

ProjectManager::projectSettings ProjectManager::getNewProjectSettings() const
{
	return newProjectSettings;
}

void ProjectManager::setNewProjectSettings(const projectSettings &value)
{
	newProjectSettings = value;
}

QString ProjectManager::getProjectPath() const
{
	return projectPath;
}

void ProjectManager::setProjectPath(const QString &value)
{
	projectPath = value;
}

void ProjectManager::update(QString path /*should only get path which directs to .project file*/)
{
	this->projectPath = path;
	QFile file(path);
	QDomDocument doc = helpers::parseXMLFile(&file);
	QDomNode root = doc.firstChildElement("projectDescription");
//	qDebug() << root.toElement().tagName();
	QDomNode childNode = root.firstChild();
	while (!childNode.isNull()) {
		if ( childNode.toElement().tagName().compare("source_directory") == 0) {
			srcPath = childNode.firstChild().nodeValue();
		} else if( childNode.toElement().tagName().compare("output_directory") == 0){
			outPath = childNode.firstChild().nodeValue();
		}
		childNode = childNode.nextSibling();
	}


}

ProjectManager* ProjectManager::getInstance()
{
	if (instance == 0)
	{
		instance = new ProjectManager();
	}

	return instance;
}
