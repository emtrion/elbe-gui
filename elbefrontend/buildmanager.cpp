#include "buildmanager.h"

#include <QFileInfo>

#include "buildprocessworker.h"
#include "helpers.h"

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


