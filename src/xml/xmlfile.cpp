/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "xmlfile.h"

#include <QFileInfo>

#include "src/mainwindow/mainwindow.h"
#include "src/app/helpers.h"


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



