#include "buildmanager.h"
#include "buildprocessworker.h"

BuildManager* BuildManager::instance = NULL;

BuildManager::BuildManager()
{

}

bool BuildManager::isLoadingFiles() const
{
	return loadingFiles;
}

void BuildManager::setLoadingFiles(bool value)
{
	loadingFiles = value;
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
	return currentBuildDirectory;
}

void BuildManager::setCurrentBuildDirectory(const QString &value)
{
	currentBuildDirectory = value;
}

bool BuildManager::isBuildRunning() const
{
	return buildRunning;
}

void BuildManager::setBuildRunning(bool value)
{
	buildRunning = value;
}

BuildManager *BuildManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new BuildManager();
	}

	return instance;
}


