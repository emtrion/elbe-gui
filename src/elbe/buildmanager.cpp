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

#include "buildmanager.h"

#include <QFileInfo>

#include "buildprocessworker.h"
#include "src/app/helpers.h"

BuildManager* BuildManager::instance = NULL;

BuildManager::BuildManager()
{

}

QStringList BuildManager::outputFiles() const
{
	return m_outputFiles;
}

void BuildManager::setOutputFiles(const QStringList &outputFiles)
{
	m_outputFiles = outputFiles;
}

QString BuildManager::elbeCommandPrefix() const
{
	return m_elbeCommandPrefix;
}

void BuildManager::setElbeCommandPrefix(const QString &value)
{
	m_elbeCommandPrefix = value;
}

QString BuildManager::elbeWorkingDir() const
{
	return m_elbeWorkingDir;
}

void BuildManager::setElbeWorkingDir(const QString &value)
{
	QFileInfo file(value);

	if ( value.compare("default") == 0 ) {
			m_elbeWorkingDir = helpers::getHomeDirectoryFromSystem();
	} else {
		//absolutPath() returns the path excluding the filename
		m_elbeWorkingDir = file.absolutePath();
	}
}

bool BuildManager::isLoadingFiles() const
{
	return m_loadingFiles;
}

void BuildManager::setLoadingFiles(bool value)
{
	m_loadingFiles = value;
}

BuildProcessWorker *BuildManager::processWorkerPointer() const
{
	return m_processWorkerPointer;
}

void BuildManager::setProcessWorkerPointer(BuildProcessWorker *value)
{
	m_processWorkerPointer = value;
}

QString BuildManager::currentBuildDirectory() const
{
	return m_currentBuildDirectory;
}

void BuildManager::setCurrentBuildDirectory(const QString &value)
{
	m_currentBuildDirectory = value;
}

bool BuildManager::isBuildRunning() const
{
	return m_buildRunning;
}

void BuildManager::setBuildRunning(bool value)
{
	m_buildRunning = value;
}

BuildManager *BuildManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new BuildManager();
	}

	return instance;
}


