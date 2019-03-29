#include "buildprocess.h"

#include <QThread>
#include <QDebug>

#include "elbehandler.h"
#include "buildmanager.h"
#include "buildprocessworker.h"
#include "buildprocessstatusbarupdate.h"
#include "mainwindow.h"
#include "helpers.h"

BuildProcess::BuildProcess(QObject *parent) : QObject(parent)
{
	buildmanager = BuildManager::getInstance();
	buildWorker = new BuildProcessWorker(buildmanager->outputFiles());
}

BuildProcess::~BuildProcess()
{
}

void BuildProcess::startBuild(bool sourceOptionChecked, bool binOptionChecked)
{
	MainWindow *mw = helpers::getMainWindow();

	mw->setWindowTitle("elbeFrontend (build running)");

	if ( ElbeHandler::checkIfBusy(buildWorker->buildingElbeID()) ) {
		buildWorker->updateMessageLog("The project is busy right now");
		return;
	}

	buildmanager->setBuildRunning(true);
	mw->changeElbeActionsEnabledStatus(false);
	if ( !ElbeHandler::startBuildProcess(sourceOptionChecked, binOptionChecked) ) {
		buildmanager->setBuildRunning(false);
		mw->changeElbeActionsEnabledStatus(true);
		buildWorker->updateMessageLog("Couldn't start build. Most likely there is a problem with your initVM");
		return;
	}
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

}


void BuildProcess::cleanup()
{
	MainWindow *mw = helpers::getMainWindow();
	mw->changeElbeActionsEnabledStatus(true);
	mw->setWindowTitle("elbeFrontend");
	buildThread->quit();
	buildThread->wait();
}


