#include "xmlfilemanager.h"


/* Null, because instance will be initialized on demand. */
XmlFileManager* XmlFileManager::instance = NULL;

XmlFileManager::XmlFileManager()
{

}

bool XmlFileManager::getIsSaved() const
{
	return isSaved;
}

void XmlFileManager::setIsSaved(bool value)
{
	isSaved = value;
}

QString XmlFileManager::getCurrentFilePath() const
{
	return currentFilePath;
}

void XmlFileManager::setCurrentFilePath(const QString &value)
{
	currentFilePath = value;
}

XmlFileManager* XmlFileManager::getInstance()
{
	if (instance == NULL) {
		instance = new XmlFileManager();
	}

	return instance;
}
