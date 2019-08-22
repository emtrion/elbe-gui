#include "checkupdatethread.h"
#include "src/elbe/elbehandler.h"

#include <src/elbe/buildprocessstatusbarupdate.h>

CheckUpdateThread::CheckUpdateThread()
{

}

void CheckUpdateThread::run()
{
	QString result;

	showCheckUpdateInStatusBar();

	result = ElbeHandler::checkForUpdates();


	statusBarCheckUpdateThread->requestInterruption();
	statusBarCheckUpdateThread->quit();
	statusBarCheckUpdateThread->wait();


	emit resultReady(result);

}


void CheckUpdateThread::showCheckUpdateInStatusBar()
{
	BuildProcessStatusBarUpdate *statusBarCheckUpdateWorker = new BuildProcessStatusBarUpdate();
	statusBarCheckUpdateThread = new QThread();
	connect(statusBarCheckUpdateThread, SIGNAL(started()), statusBarCheckUpdateWorker, SLOT(statusBarCheckUpdate()));
	connect(statusBarCheckUpdateThread, SIGNAL(finished()), statusBarCheckUpdateWorker, SLOT(deleteLater()));
	statusBarCheckUpdateWorker->moveToThread(statusBarCheckUpdateThread);
	statusBarCheckUpdateThread->start();
}
