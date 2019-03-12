#include "buildprocessworker.h"
#include "mainwindow.h"

#include "elbehandler.h"
#include <QProcess>
#include "projectmanager.h"
#include "helpers.h"
#include <QDebug>


BuildProcessWorker::BuildProcessWorker(QStringList outputFiles)
{
	//outputfiles which are seleceted in buildprocessstartdialog
	this->outputFiles = outputFiles;

	handler = new ElbeHandler();

	projectmanager = ProjectManager::getInstance();
	buildingElbeID = projectmanager->getElbeID();
	buildingXmlPath = projectmanager->getBuildXmlPath();
	buildingOutPath = projectmanager->getOutPath();

	MainWindow *mw = helpers::getMainWindow();
	//it's not allowed to communicate with the GUI directly from another thread but the mainthread
	connect(this, SIGNAL(outputReady(QString)), this, SLOT(updateMessageLog(QString)));
	connect(this, SIGNAL(messageLogHasUpdate(QString,QString)), mw, SLOT(messageLogAppendText(QString,QString)));
}

BuildProcessWorker::~BuildProcessWorker()
{

}


void BuildProcessWorker::doWork()
{
//	qDebug() << __func__<<"is in: "<<QThread::currentThreadId();

	process = new QProcess(this);

	//two threads  which show the status on statusbar
	StatusBarThread *statusBarBuildWorker = new StatusBarThread();
	StatusBarThread *statusBarLoadWorker = new StatusBarThread();
	statusBarBuildThread = new QThread();
	statusBarLoadThread = new QThread();


	//connect the start and finish routines
	connect(statusBarBuildThread, SIGNAL(started()), statusBarBuildWorker, SLOT(statusBarBuildRunning()));
	connect(statusBarLoadThread, SIGNAL(started()), statusBarLoadWorker, SLOT(statusBarLoadingFile()));
	connect(statusBarBuildThread, SIGNAL(finished()), statusBarBuildWorker, SLOT(deleteLater()));
	connect(statusBarBuildThread, SIGNAL(finished()), statusBarLoadWorker, SLOT(deleteLater()));

	statusBarBuildWorker->moveToThread(statusBarBuildThread);
	statusBarBuildThread->start();


	emit(outputReady("start elbe-build"));

//	wait_busy->setWorkingDirectory("/home/hico/elbe");
	process->setWorkingDirectory("/home/hico/tmp");

//	wait_busy->start("./elbe control wait_busy "+projectmanager->getElbeID());
	process->start("./a.out");

	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(printLog()));
	process->waitForFinished(-1);

	updateMessageLog("finished");
	statusBarBuildThread->requestInterruption();

	//change statusbar message
	statusBarLoadWorker->moveToThread(statusBarLoadThread);
	statusBarLoadThread->start();

	emit(messageTextNeedsChange());
	updateMessageLog(" ");
	updateMessageLog("downloading files...");

	downloadFiles();

}
/*prints the elbe build output in the MessageLog*/
void BuildProcessWorker::printLog()
{
	QString tail; //if the last part of the read output isn't a complete line it's stored in here
	QStringList list;
	output.append(process->readAllStandardOutput());
	if(!output.endsWith("\n")) { //check if last part is a complete line
		list = output.split("\n");
		tail = list.last();
		list.removeLast(); //remove uncomplete part
	} else {
		list = output.split("\n", QString::SkipEmptyParts);
	}
	foreach (QString str, list) {
//		qDebug() << str;
		emit(outputReady(str)); //connected to updateMessageLog
	}

	output = ""; //clear output to read new data
	output.append(tail); //put the last part at the beginning of the new read data
}


void BuildProcessWorker::updateMessageLog(const QString &str)
{
	//qDebug() << __func__<<" is in: "<<QThread::currentThreadId();
	emit(messageLogHasUpdate(str, "#F36363")); //calls mainwindow slot messageLogAppendText
}

void BuildProcessWorker::downloadFiles()
{
//	qDebug() << "XMLPATH: "<<buildingXmlPath;

	if ( outputFiles.contains("Image")) {
		if ( !handler->getImages(buildingXmlPath, buildingOutPath, buildingElbeID ) ) {
			qDebug() << "Could not load all images. Check Output directory and try again";
			updateMessageLog("Could not load all images. Check Output directory and try again");
		}
		outputFiles.removeOne("Image");
	}
	if( !outputFiles.isEmpty() ) {
		if ( !handler->getFiles(outputFiles, buildingOutPath, buildingElbeID) ) {
			qDebug() << "Could not load all files. Check Output directory and try again";
			updateMessageLog("Could not load all files. Check Output directory and try again");
		}
	}

	emit statusBarLoadThread->requestInterruption();

	updateMessageLog("files loaded.");
	updateMessageLog(" ");
	updateMessageLog("finished");

	return;
}
