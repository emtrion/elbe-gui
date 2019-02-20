#include "xmlfilemanager.h"

#include <QFileInfo>
#include "mainwindow.h"
#include "helpers.h"


/* Null, because instance will be initialized on demand. */
XmlFileManager* XmlFileManager::instance = NULL;

XmlFileManager::XmlFileManager()
{

}

bool XmlFileManager::getSaving() const
{
	return saving;
}

void XmlFileManager::setSaving(bool value)
{
	saving = value;
}

QString XmlFileManager::getCurrentFileName() const
{
	return currentFileName;
}



bool XmlFileManager::getIsSaved() const
{
	return isSaved;
}

void XmlFileManager::setIsSaved(bool value)
{
	isSaved = value;
	saveStatusChanged(value);
}

QString XmlFileManager::getCurrentFilePath() const
{
	return currentFilePath;
}

void XmlFileManager::setCurrentFilePath(const QString &value)
{
	currentFilePath = value;
	currentFileName = QFileInfo(value).fileName();
}

XmlFileManager* XmlFileManager::getInstance()
{
	if (instance == NULL) {
		instance = new XmlFileManager();
	}

	return instance;
}


void XmlFileManager::setToDefault()
{
	currentFilePath = "";
	currentFileName = "";
	isSaved = true;
	isOpen = false;
}

void XmlFileManager::saveStatusChanged(bool saveStatus)
{
	MainWindow *mw = helpers::getMainWindow();
	mw->updateEditorTabSaveStatus(saveStatus);
}

bool XmlFileManager::getIsOpen() const
{
	return isOpen;
}

void XmlFileManager::setIsOpen(bool value)
{
	isOpen = value;
}
