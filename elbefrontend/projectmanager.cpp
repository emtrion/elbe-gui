#include "projectmanager.h"
#include "helpers.h"
#include <QDebug>
/* This is a singelton class...
 * because their can always be just one open project at the time.
 * Should another project be opened, the previous will be closed and the instance
 * of this class will be deleted and new assigned*/

/* Null, because instance will be initialized on demand. */
ProjectManager* ProjectManager::instance = NULL;

QString ProjectManager::getSrcPath() const
{
	return srcPath;
}

QString ProjectManager::getOutPath() const
{
	return outPath;
}

QString ProjectManager::getProjectName() const
{
	return projectName;
}

bool ProjectManager::isProjectOpened() const
{
	return projectOpened;
}

void ProjectManager::setProjectOpened(bool value)
{
	projectOpened = value;
}

QString ProjectManager::getProjectDirectory() const
{
	return projectDirectory;
}

void ProjectManager::setProjectDirectory(const QString &value)
{
	projectDirectory = value;
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
	if ( path.isNull() ) {
		projectPath = QString();
		srcPath = QString();
		outPath = QString();
		projectName = QString();
		newProjectSettings = ProjectManager::emptyStruct;
		return;
	}

	this->projectPath = path;
	this->projectDirectory = path.section("/", 0, -2);

	QFile file(path);
	QDomDocument doc = helpers::parseXMLFile(&file);
	QDomNode root = doc.firstChildElement("projectDescription");
//	qDebug() << root.toElement().tagName();
	QDomNode projectNode;
	QDomNode childNode = root.firstChild();
	while (!childNode.isNull()) {
		if ( childNode.toElement().tagName().compare("projectname") == 0 ) {
			projectName = childNode.firstChild().nodeValue();
		} else if ( childNode.toElement().tagName().compare("source_directory") == 0) {
			srcPath = childNode.firstChild().nodeValue();
		} else if( childNode.toElement().tagName().compare("output_directory") == 0){
			outPath = childNode.firstChild().nodeValue();
		} else if( childNode.toElement().tagName().compare("project") == 0) {
			projectNode = childNode;
		}
		childNode = childNode.nextSibling();
	}

	QDomNode mirrorNode;
	QDomNode projectChildNode = projectNode.firstChild();
	while (!projectChildNode.isNull()) {
		QDomElement projectChildElement = projectChildNode.toElement();
		if (projectChildElement.tagName().compare("name") == 0) {
			newProjectSettings.name = projectChildNode.firstChild().nodeValue();
		} else if (projectChildElement.tagName().compare("version") == 0) {
			newProjectSettings.version = projectChildNode.firstChild().nodeValue();
		} else if (projectChildElement.tagName().compare("description") == 0) {
			newProjectSettings.description = projectChildNode.firstChild().nodeValue();
		} else if (projectChildElement.tagName().compare("buildtype") == 0) {
			newProjectSettings.buildtype = projectChildNode.firstChild().nodeValue();
		} else if (projectChildElement.tagName().compare("suite") == 0) {
			newProjectSettings.suite = projectChildNode.firstChild().nodeValue();
		} else if (projectChildElement.tagName().compare("mirror") == 0) {
			mirrorNode = projectChildNode; //assign "mirror" to a node to access children
		}
		projectChildNode = projectChildNode.nextSibling();
	}

	QDomNode mirrorChildNode = mirrorNode.firstChild();
	while (!mirrorChildNode.isNull()) { //iterate over all children
		QDomElement mirrorChildElement = mirrorChildNode.toElement();
		if (mirrorChildElement.tagName().compare("primary_host") == 0) {
			newProjectSettings.host = mirrorChildNode.firstChild().nodeValue();
		} else if (mirrorChildElement.tagName().compare("primary_path") == 0) {
			newProjectSettings.path = mirrorChildNode.firstChild().nodeValue();
		} else if (mirrorChildElement.tagName().compare("primary_proto") == 0) {
			newProjectSettings.proto = mirrorChildNode.firstChild().nodeValue();
		}
		mirrorChildNode = mirrorChildNode.nextSibling();
	}
}




ProjectManager* ProjectManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new ProjectManager();
	}

	return instance;
}
