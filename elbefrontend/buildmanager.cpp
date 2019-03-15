#include "buildmanager.h"
#include "buildprocessworker.h"

BuildManager* BuildManager::instance = NULL;

BuildManager::BuildManager()
{

}

bool BuildManager::isLoadingFiles() const
{
	return m_loadingFiles;
}

void BuildManager::setLoadingFiles(bool value)
{
	m_loadingFiles = value;
}

BuildProcessWorker *BuildManager::getProcessWorkerPointer() const
{
	return processWorkerPointer;
}

void BuildManager::setProcessWorkerPointer(BuildProcessWorker *value)
{
	processWorkerPointer = value;
}

QString BuildManager::getCurrentBuildDirectory() const
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


