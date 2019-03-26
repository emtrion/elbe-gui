#ifndef BUILDMANAGER_H
#define BUILDMANAGER_H


#include <QString>
#include <QThread>
//#include "buildprocessworker.h"

class BuildProcessWorker;

class BuildManager
{
	private:
		/* The instance will be stored here. */
		static BuildManager* instance;

		/* Private constructor to prevent instantiation. From outside*/
		BuildManager();

		bool buildRunning = false;
		//is true if the build is finished and the files are loading
		bool loadingFiles = false;
		//the ID of the project currently building
		QString currentBuildDirectory;
		//pointer to the worker in which the build happens
		BuildProcessWorker *processWorkerPointer;
		QString elbeWorkingDir;
		QString elbeCommandPrefix;


   public:
		/* Static access method. */
		static BuildManager* getInstance();

		bool isBuildRunning() const;
		void setBuildRunning(bool value);
		QString getCurrentBuildDirectory() const;
		void setCurrentBuildDirectory(const QString &value);
		BuildProcessWorker *getProcessWorkerPointer() const;
		void setProcessWorkerPointer(BuildProcessWorker *value);
		bool isLoadingFiles() const;
		void setLoadingFiles(bool value);
		QString getElbeWorkingDir() const;
		void setElbeWorkingDir(const QString &value);
		QString getElbeCommandPrefix() const;
		void setElbeCommandPrefix(const QString &value);
};

#endif // BUILDMANAGER_H
