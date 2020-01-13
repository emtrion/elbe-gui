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
