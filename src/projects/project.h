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

#ifndef PROJECT_H
#define PROJECT_H

#include <QString>

#include <src/update/updates.h>

#include "projectproperties.h"

class QFileSystemWatcher;

class Project
{
	public:
		/* Static access method. */
		static Project* getInstance();

		ProjectProperties* newProjectProperties() const;
		QString projectPath() const;
		void setProjectPath(const QString &value);
		void update(const QString path);
		QString srcPath() const;
		QString outPath() const;
		QString projectName() const;
		bool isProjectOpened() const;
		void setProjectOpened(bool value);
		QString projectDirectory() const;
		void setProjectDirectory(const QString &value);
		bool isProjectModified() const;
		void setProjectModified(bool value);
		QString elbeID() const;
		void setElbeID(const QString &value);
		bool projectHasFile() const;
		void setProjectHasFile(bool value);
		QString buildXmlPath() const;
		void setBuildXmlPath(const QString &value);
		QFileSystemWatcher *watcher;

		Updates *getUpdates() const;
		void setUpdates(Updates *value);

	private:
		static Project* instance;
		// Private constructor to prevent instancing from outside
		Project();

		QString m_projectDirectory;
		QString m_projectPath;
		QString m_srcPath;
		QString m_outPath;
		QString m_projectName;
		QString m_elbeID;
		QString m_buildXmlPath;
		ProjectProperties *m_newProjectProperties = new ProjectProperties();
		bool m_projectOpened = false;
		bool m_projectModified = false;
		bool m_projectHasFile = false;

		Updates *updates;
};




#endif // PROJECT_H
