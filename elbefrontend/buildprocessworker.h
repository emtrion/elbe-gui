#ifndef BUILDPROCESSWORKER_H
#define BUILDPROCESSWORKER_H

#include "mainwindow.h"
#include "projectmanager.h"

#include <QObject>
#include <QProcess>
#include <QDebug>
#include "helpers.h"
#include <QThread>



class BuildProcessWorker : public QObject
{
		Q_OBJECT
	public:
		explicit BuildProcessWorker();
		~BuildProcessWorker();
		QProcess *wait_busy;
		QString output;

	signals:
		void resultReady(const QString &result);
		void outputReady(const QString &str);
		void readyToLoadFiles();


	public slots:
		void doWork(/*const QString &parameter*/);
		void printLog();


	private:

};

#endif // BUILDPROCESSWORKER_H
