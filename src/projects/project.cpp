#include "project.h"

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>

#include "src/xml/xmlutilities.h"


/* Null, because instance will be initialized on demand. */
Project* Project::instance = NULL;

Project::Project()
{}

Updates *Project::getUpdates() const
{
	return updates;
}

void Project::setUpdates(Updates *value)
{
	updates = value;
}

Project* Project::getInstance()
{
	if (instance == NULL)
	{
		instance = new Project();
	}

	return instance;
}

QString Project::srcPath() const
{
	return m_srcPath;
}

QString Project::outPath() const
{
	return m_outPath;
}

QString Project::projectName() const
{
	return m_projectName;
}

bool Project::isProjectOpened() const
{
	return m_projectOpened;
}

void Project::setProjectOpened(bool value)
{
	m_projectOpened = value;
}

QString Project::projectDirectory() const
{
	return m_projectDirectory;
}

void Project::setProjectDirectory(const QString &value)
{
	m_projectDirectory = value;
}

bool Project::isProjectModified() const
{
	return m_projectModified;
}

void Project::setProjectModified(bool value)
{
	m_projectModified = value;
}

QString Project::elbeID() const
{
	return m_elbeID;
}

void Project::setElbeID(const QString &value)
{
	m_elbeID = value;
}

bool Project::projectHasFile() const
{
	return m_projectHasFile;
}

void Project::setProjectHasFile(bool value)
{
	m_projectHasFile = value;
}

QString Project::buildXmlPath() const
{
	return m_buildXmlPath;
}

void Project::setBuildXmlPath(const QString &value)
{
	m_buildXmlPath = value;
}

ProjectProperties* Project::newProjectProperties() const
{
	return m_newProjectProperties;
}

QString Project::projectPath() const
{
	return m_projectPath;
}

void Project::setProjectPath(const QString &value)
{
	m_projectPath = value;
}

//path has to be .project file
void Project::update(const QString path)
{
	if ( path.isNull() ) {
		m_projectPath = QString();
		m_srcPath = QString();
		m_outPath = QString();
		m_projectName = QString();
		m_newProjectProperties->reset();
		return;
	}

	this->m_projectPath = path;
	this->m_projectDirectory = QFileInfo(path).absolutePath();

	QFile file(path);
	QDomDocument doc = xmlUtilities::parseXMLFile(&file);
	QDomNode root = doc.firstChildElement("projectDescription");
	QDomNode projectNode;
	QDomNode childNode = root.firstChild();
	while ( !childNode.isNull() ) {
		if ( childNode.toElement().tagName().compare("projectname") == 0 ) {
			m_projectName = childNode.firstChild().nodeValue();
		} else if ( childNode.toElement().tagName().compare("elbe_id") == 0 ) {
			m_elbeID = childNode.firstChild().nodeValue();
		} else if ( childNode.toElement().tagName().compare("source_directory") == 0 ) {
			m_srcPath = childNode.firstChild().nodeValue();
		} else if ( childNode.toElement().tagName().compare("output_directory") == 0 ) {
			m_outPath = childNode.firstChild().nodeValue();
		} else if( childNode.toElement().tagName().compare("project") == 0) {
			projectNode = childNode;
		}
		childNode = childNode.nextSibling();
	}

	QDomNode mirrorNode;
	QDomNode projectChildNode = projectNode.firstChild();
	while ( !projectChildNode.isNull() ) {
		QDomElement projectChildElement = projectChildNode.toElement();
		if (projectChildElement.tagName().compare("name") == 0) {
			m_newProjectProperties->setName(projectChildNode.firstChild().nodeValue());
		} else if (projectChildElement.tagName().compare("version") == 0) {
			m_newProjectProperties->setVersion(projectChildNode.firstChild().nodeValue());
		} else if (projectChildElement.tagName().compare("description") == 0) {
			m_newProjectProperties->setDescription(projectChildNode.firstChild().nodeValue());
		} else if (projectChildElement.tagName().compare("buildtype") == 0) {
			m_newProjectProperties->setBuildtype(projectChildNode.firstChild().nodeValue());
		} else if (projectChildElement.tagName().compare("suite") == 0) {
			m_newProjectProperties->setSuite(projectChildNode.firstChild().nodeValue());
		} else if (projectChildElement.tagName().compare("mirror") == 0) {
			mirrorNode = projectChildNode; //assign "mirror" to a node to access children
		}
		projectChildNode = projectChildNode.nextSibling();
	}

	QDomNode mirrorChildNode = mirrorNode.firstChild();
	while ( !mirrorChildNode.isNull() ) {
		QDomElement mirrorChildElement = mirrorChildNode.toElement();
		if (mirrorChildElement.tagName().compare("primary_host") == 0) {
			m_newProjectProperties->setHost(mirrorChildNode.firstChild().nodeValue());
		} else if (mirrorChildElement.tagName().compare("primary_path") == 0) {
			m_newProjectProperties->setPath(mirrorChildNode.firstChild().nodeValue());
		} else if (mirrorChildElement.tagName().compare("primary_proto") == 0) {
			m_newProjectProperties->setProto(mirrorChildNode.firstChild().nodeValue());
		}
		mirrorChildNode = mirrorChildNode.nextSibling();
	}
}
