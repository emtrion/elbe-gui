#ifndef BUILDMANAGER_H
#define BUILDMANAGER_H


#include <QString>
#include <QThread>
//#include "buildprocessworker.h"

class BuildProcessWorker;

class BuildManager
{
	public:

	private:
		/* Here, the instance will be stored. */
		static BuildManager* instance;

		/* Private constructor to prevent instancing. */
		BuildManager();

		bool m_buildRunning = false;
		//is true if the build is finished and the files are loading
		bool m_loadingFiles = false;
		//the ID of the project currently building
		QString m_currentBuildDirectory;
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
