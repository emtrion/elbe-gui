#include "buildmanager.h"
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
	buildingProjectPath = projectmanager->getProjectPath();



	MainWindow *mw = helpers::getMainWindow();
	connect(this, SIGNAL(messageLogHasUpdate(QString,QString)), mw, SLOT(messageLogAppendText(QString,QString)));
}

BuildProcessWorker::~BuildProcessWorker()
{

}


void BuildProcessWorker::doWork()
{
//	qDebug() << __func__<<"is in: "<<QThread::currentThreadId();

	BuildManager *buildmanager = BuildManager::getInstance();
	process = new QProcess(this);

	//it's not allowed to communicate with the GUI directly from another thread but the mainthread
	connect(this, SIGNAL(outputReady(QString)), this, SLOT(updateMessageLog(QString)));
	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(printLog()));

	//exec the wait_busy command where we wait for the build to finish and receive the build log messages
	waitBusy();
	//was set before the actual build command in "buildmanager". After waitBusy returns we can be sure the build has finished
	buildmanager->setBuildRunning(false);

	//just for a better look
	updateMessageLog(" ");

	if ( !skipDownload ) {
		//inidcates that a download is currently running...
		buildmanager->setLoadingFiles(true);
		downloadFiles();
		//...now it's done
		buildmanager->setLoadingFiles(false);
	}

	emit(done());
}


void BuildProcessWorker::waitBusy()
{
	//start to show "build" in the status bar
	showBuildingInStatusBar();

	emit(outputReady("start elbe-build"));

	process->setWorkingDirectory("/home/hico/elbe");
	process->start("./elbe control wait_busy "+buildingElbeID); //change this. It depends on the opened project which is not itended

//	process->setWorkingDirectory("/home/hico/tmp");
//	process->start("./a.out");

	//wait until build has finished. Events are still handled so messages are coming through
	process->waitForFinished(-1);

	updateMessageLog("finished");

	//stop the loop inside the thread
	statusBarBuildThread->requestInterruption();

	//end the thread
	statusBarBuildThread->quit();
	statusBarBuildThread->wait();
}

void BuildProcessWorker::setSkipDownload(bool value)
{
	skipDownload = value;
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
	//change statusbar message
	showLoadingInStatusBar();

//	qDebug() << "XMLPATH: "<<buildingXmlPath;
	updateMessageLog("downloading files...");

	if ( outputFiles.contains("Image")) {
		if ( !handler->getImages(buildingXmlPath, buildingOutPath, buildingElbeID ) ) {
			qDebug() << "Could not load all images. Check Output directory and try again";
			updateMessageLog("Could not load all images. Check Output directory and try again");
		} else {
			updateMessageLog("images loaded.");
		}
		outputFiles.removeOne("Image");
	}
	if( !outputFiles.isEmpty() ) {
		if ( !handler->getFiles(outputFiles, buildingOutPath, buildingElbeID) ) {
			qDebug() << "Could not load all files. Check Output directory and try again";
			updateMessageLog("Could not load all files. Check Output directory and try again");
		} else {
			updateMessageLog("files loaded.");
		}
	}

	//stop the loop inside the thread
	statusBarLoadThread->requestInterruption();

	statusBarLoadThread->quit();
	statusBarLoadThread->wait();


	updateMessageLog(" ");
	updateMessageLog("finished");

	return;
}

QString BuildProcessWorker::getBuildingElbeID() const
{
	return buildingElbeID;
}

void BuildProcessWorker::setBuildingElbeID(const QString &value)
{
	buildingElbeID = value;
}

QThread *BuildProcessWorker::getStatusBarBuildThread() const
{
	return statusBarBuildThread;
}

void BuildProcessWorker::showBuildingInStatusBar()
{
	StatusBarThread *statusBarBuildWorker = new StatusBarThread();
	statusBarBuildThread = new QThread();
	connect(statusBarBuildThread, SIGNAL(started()), statusBarBuildWorker, SLOT(statusBarBuildRunning()));
	connect(statusBarBuildThread, SIGNAL(finished()), statusBarBuildWorker, SLOT(deleteLater()));
	statusBarBuildWorker->moveToThread(statusBarBuildThread);
	statusBarBuildThread->start();
}

void BuildProcessWorker::showLoadingInStatusBar()
{
	StatusBarThread *statusBarLoadWorker = new StatusBarThread();
	statusBarLoadThread = new QThread();
	connect(statusBarLoadThread, SIGNAL(started()), statusBarLoadWorker, SLOT(statusBarLoadingFile()));
	connect(statusBarBuildThread, SIGNAL(finished()), statusBarLoadWorker, SLOT(deleteLater()));
	statusBarLoadWorker->moveToThread(statusBarLoadThread);
	statusBarLoadThread->start();
}

QString BuildProcessWorker::getBuildingProjectPath() const
{
	return buildingProjectPath;
}
