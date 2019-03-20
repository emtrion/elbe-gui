#include "buildprocess.h"
#include "elbehandler.h"
#include "buildmanager.h"
#include "buildprocessworker.h"
#include <QThread>
#include <QDebug>
#include "buildprocessstatusbarupdate.h"
#include "mainwindow.h"
#include "helpers.h"

BuildProcess::BuildProcess(QObject *parent) : QObject(parent)
{
	elbehandler = new ElbeHandler();
	buildmanager = BuildManager::getInstance();
	buildWorker = new BuildProcessWorker(outputFiles);
}

BuildProcess::~BuildProcess()
{
}

void BuildProcess::startBuild(bool sourceOptionChecked, bool binOptionChecked)
{
	MainWindow *mw = helpers::getMainWindow();

	if ( elbehandler->checkIfBusy(buildWorker->getBuildingElbeID()) ) {
		buildWorker->updateMessageLog("The project is busy right now");
		return;
	}

	buildmanager->setBuildRunning(true);
	mw->changeElbeActionsEnabledStatus(false);
//	if ( !elbehandler->startBuildProcess(sourceOptionChecked, binOptionChecked) ) {
//		buildmanager->setBuildRunning(false);
//		buildWorker->updateMessageLog("Couldn't start build. Most likely there is a problem with your initVM");
//		return;
//	}
	buildWorker->setSkipDownload(false);
	buildThreadInit();
}


/*This method is used when the application was closed and restarted during a build.
 * It makes sure that the build has finished or if not just contionues as normal*/
void BuildProcess::waitBusyWithoutStartingBuild(QString elbeid)
{
	MainWindow *mw = helpers::getMainWindow();
	buildmanager->setBuildRunning(true);
	mw->changeElbeActionsEnabledStatus(false);
	buildWorker->setSkipDownload(true);
	buildWorker->setBuildingElbeID(elbeid);
	buildThreadInit();
}

void BuildProcess::buildThreadInit()
{
	buildThread = new QThread();

	//pass the instance to buildmanager so the threads can be handled outside
	buildmanager->setProcessWorkerPointer(buildWorker);

	connect(buildThread, SIGNAL(started()), buildWorker, SLOT(doWork()));
	connect(buildThread, SIGNAL(finished()), buildWorker, SLOT(deleteLater()));
	connect(buildWorker, SIGNAL(done()), this, SLOT(cleanup()));

	buildWorker->moveToThread(buildThread);
	buildThread->start();

//	qDebug() << QThread::currentThreadId();
}


void BuildProcess::cleanup()
{
	MainWindow *mw = helpers::getMainWindow();
	mw->changeElbeActionsEnabledStatus(true);
	buildThread->quit();
	buildThread->wait();
}

QStringList BuildProcess::getOutputFiles() const
{
	return outputFiles;
}

void BuildProcess::setOutputFiles(const QStringList &value)
{
	outputFiles = value;
}


