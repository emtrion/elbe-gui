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

#ifndef XMLFILE_H
#define XMLFILE_H


#include <QString>

class XmlFile
{	
	public:
		/* Static access method. */
		static XmlFile *getInstance();

		QString currentFilePath() const;
		void setCurrentFilePath(const QString &value);
		bool isSaved() const;
		void setIsSaved(bool value);
		QString currentFileName() const;
		void setCurrentFileName(const QString &currentFileName);
		bool saving() const;
		void setSaving(bool value);
		bool isOpen() const;
		void setIsOpen(bool value);
		void setToDefault();

	private:
		XmlFile();
		static XmlFile *instance;

		QString m_currentFilePath;
		QString	m_currentFileName;
		bool m_isSaved;
		bool m_saving;
		bool m_isOpen;

	private:
		void saveStatusChanged(bool saveStatus);



};

#endif // XMLFILEMANAGER_H
