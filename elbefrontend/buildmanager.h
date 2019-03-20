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
};

#endif // BUILDMANAGER_H
