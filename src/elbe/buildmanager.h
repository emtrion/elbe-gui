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

#ifndef BUILDMANAGER_H
#define BUILDMANAGER_H


#include <QString>
#include <QThread>

class BuildProcessWorker;

class BuildManager
{
	public:
		 /* Static access method. */
		 static BuildManager* getInstance();

		 bool isBuildRunning() const;
		 void setBuildRunning(bool value);
		 QString currentBuildDirectory() const;
		 void setCurrentBuildDirectory(const QString &value);
		 BuildProcessWorker *processWorkerPointer() const;
		 void setProcessWorkerPointer(BuildProcessWorker *value);
		 bool isLoadingFiles() const;
		 void setLoadingFiles(bool value);
		 QString elbeWorkingDir() const;
		 void setElbeWorkingDir(const QString &value);
		 QString elbeCommandPrefix() const;
		 void setElbeCommandPrefix(const QString &value);
		 QStringList outputFiles() const;
		 void setOutputFiles(const QStringList &outputFiles);

	private:
		static BuildManager* instance;
		//Private constructor to prevent instantiation from outside
		BuildManager();

		bool m_buildRunning = false;
		bool m_loadingFiles = false;
		QString m_currentBuildDirectory;
		BuildProcessWorker *m_processWorkerPointer;
		QString m_elbeWorkingDir;
		QString m_elbeCommandPrefix;
		QStringList m_outputFiles;
};

#endif // BUILDMANAGER_H
