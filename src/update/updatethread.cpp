#include "updatethread.h"
#include "src/elbe/elbehandler.h"

#include <QDebug>

#include <src/elbe/buildprocessstatusbarupdate.h>

UpdateThread::UpdateThread()
{

}


void UpdateThread::run()
{
	showUpdateInStatusBar();

	QStringList result;

	result = ElbeHandler::buildUpdate();

	statusBarUpdateThread->requestInterruption();
	statusBarUpdateThread->quit();
	statusBarUpdateThread->wait();


	if ( result.size() < 1 ) {
		emit resultReady(result);
	} else {
		emit errorLog(result);
	}


}

void UpdateThread::showUpdateInStatusBar()
{
	BuildProcessStatusBarUpdate *statusBarUpdateWorker = new BuildProcessStatusBarUpdate();
	statusBarUpdateThread = new QThread();
	connect(statusBarUpdateThread, SIGNAL(started()), statusBarUpdateWorker, SLOT(statusBarUpdateProject()));
	connect(statusBarUpdateThread, SIGNAL(finished()), statusBarUpdateWorker, SLOT(deleteLater()));
	statusBarUpdateWorker->moveToThread(statusBarUpdateThread);
	statusBarUpdateThread->start();
}
