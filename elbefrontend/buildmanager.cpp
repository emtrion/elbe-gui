#include "buildmanager.h"
#include "buildprocessworker.h"
#include "helpers.h"

#include <QFileInfo>

BuildManager* BuildManager::instance = NULL;

BuildManager::BuildManager()
{

}

QString BuildManager::getElbeCommandPrefix() const
{
	return elbeCommandPrefix;
}

void BuildManager::setElbeCommandPrefix(const QString &value)
{
	elbeCommandPrefix = value;
}

QString BuildManager::getElbeWorkingDir() const
{
	return elbeWorkingDir;
}

void BuildManager::setElbeWorkingDir(const QString &value)
{
	QFileInfo file(value);

	if ( value.compare("default") == 0 ) {
			elbeWorkingDir = helpers::getHomeDirectoryFromSystem();
	} else {
		//absolutPath() returns the path excluding the filename
		elbeWorkingDir = file.absolutePath();
	}
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


