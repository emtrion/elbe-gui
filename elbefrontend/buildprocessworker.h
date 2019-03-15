#ifndef BUILDPROCESSWORKER_H
#define BUILDPROCESSWORKER_H


#include "mainwindow.h"
#include "projectmanager.h"
#include "statusbarthread.h"

#include <QObject>
#include <QProcess>
#include <QDebug>
#include "helpers.h"
#include <QThread>



class BuildProcessWorker : public QObject
{
		Q_OBJECT
	public:
		explicit BuildProcessWorker(QStringList outputFiles);
		~BuildProcessWorker();
		QProcess *process;
		QString output;

		QString getBuildingProjectPath() const;

		void setSkipDownload(bool value);

		QThread *getStatusBarBuildThread() const;

		void setBuildingElbeID(const QString &value);

		QString getBuildingElbeID() const;

	signals:
		void resultReady();
		void outputReady(const QString &str);
		void messageLogHasUpdate(const QString &str, const QString &colorHexValue);
		void startStatusBarBuild();
		void startStatusBarLoad();
		void done();


	public slots:
		void doWork();
		void printLog();
		void updateMessageLog(const QString &str);
		void downloadFiles();

	private:
		QStringList outputFiles;
		ElbeHandler *handler;
		QString buildingProjectPath;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
		ProjectManager *projectmanager;
		StatusBarThread *statusBarWorker;

		QThread *statusBarBuildThread;
		QThread *statusBarLoadThread;

		void showLoadingInStatusBar();
		void showBuildingInStatusBar();
		void waitBusy();

		bool skipDownload;
};

#endif // BUILDPROCESSWORKER_H
