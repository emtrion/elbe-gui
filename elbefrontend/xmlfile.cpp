#include "xmlfile.h"

#include <QFileInfo>

#include "mainwindow.h"
#include "helpers.h"


/* Null, because instance will be initialized on demand. */
XmlFile* XmlFile::instance = NULL;

XmlFile::XmlFile()
{}

XmlFile* XmlFile::getInstance()
{
	if (instance == NULL) {
		instance = new XmlFile();
	}

	return instance;
}

void XmlFile::setCurrentFileName(const QString &currentFileName)
{
	m_currentFileName = currentFileName;
}

bool XmlFile::saving() const
{
	return m_saving;
}

void XmlFile::setSaving(bool value)
{
	m_saving = value;
}

QString XmlFile::currentFileName() const
{
	return m_currentFileName;
}

bool XmlFile::isSaved() const
{
	return m_isSaved;
}

void XmlFile::setIsSaved(bool value)
{
	m_isSaved = value;
	saveStatusChanged(value);
}

void XmlFile::saveStatusChanged(bool saveStatus)
{
	MainWindow *mw = helpers::getMainWindow();
	mw->updateEditorTabSaveStatus(saveStatus);
}

QString XmlFile::currentFilePath() const
{
	return m_currentFilePath;
}

void XmlFile::setCurrentFilePath(const QString &value)
{
	m_currentFilePath = value;
	m_currentFileName = QFileInfo(value).fileName();
}

bool XmlFile::isOpen() const
{
	return m_isOpen;
}

void XmlFile::setIsOpen(bool value)
{
	m_isOpen = value;
}

void XmlFile::setToDefault()
{
	m_currentFilePath = "";
	m_currentFileName = "";
	m_isSaved = true;
	m_isOpen = false;
}



