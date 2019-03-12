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

	signals:
		void resultReady();
		void outputReady(const QString &str);
		void messageLogHasUpdate(const QString &str, const QString &colorHexValue);
		void messageTextNeedsChange();
		void startStatusBarBuild();
		void startStatusBarLoad();


	public slots:
		void doWork(/*const QString &parameter*/);
		void printLog();



	private slots:
		void updateMessageLog(const QString &str);
		void downloadFiles();
	private:
		QStringList outputFiles;
		ElbeHandler *handler;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
		ProjectManager *projectmanager;
		StatusBarThread *statusBarWorker;

		QThread *statusBarBuildThread;
		QThread *statusBarLoadThread;

};

#endif // BUILDPROCESSWORKER_H
