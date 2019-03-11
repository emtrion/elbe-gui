#include "buildprocess.h"
#include "elbehandler.h"

#include "buildprocessworker.h"
#include <QThread>
#include <QDebug>
#include "statusbarthread.h"
BuildProcess::BuildProcess(QObject *parent) : QObject(parent)
{
	handler = new ElbeHandler();
	ProjectManager *manager = ProjectManager::getInstance();
	buildingElbeID = manager->getElbeID();
	buildingXmlPath = manager->getBuildXmlPath();
	buildingOutPath = manager->getOutPath();
}

BuildProcess::~BuildProcess()
{
}

void BuildProcess::startBuild()
{
	handler->startBuildProcess();
	buildThreadInit();
}

void BuildProcess::buildThreadInit()
{
	buildThread = new QThread();
	statusBarThread = new QThread();

	BuildProcessWorker *buildWorker = new BuildProcessWorker();
	StatusBarThread *statusBarWorker = new StatusBarThread();

	connect(buildThread, SIGNAL(started()), buildWorker, SLOT(doWork()));
	connect(buildWorker, SIGNAL(resultReady(QString)), this, SLOT(handleResults(QString)));
	connect(buildWorker, SIGNAL(outputReady(QString)), this, SLOT(updateMessageLog(QString)));


	connect(statusBarThread, SIGNAL(started()), statusBarWorker, SLOT(changeStatusBar()));
	connect(statusBarWorker, SIGNAL(statusBarHasChanged(QString)), this, SLOT(updateStatusBar(QString)));

	connect(buildWorker, SIGNAL(readyToLoadFiles()), this, SLOT(downloadFiles()));

	statusBarWorker->moveToThread(statusBarThread);
	buildWorker->moveToThread(buildThread);

	statusBarThread->start();
	buildThread->start();


	qDebug() << statusBarThread->currentThreadId();
	qDebug() << QThread::currentThreadId();
}

void BuildProcess::handleResults(const QString &result)
{
	//end the statusbar thread
	statusBarThread->requestInterruption();

	updateMessageLog(result);
	qDebug() << result;
}

void BuildProcess::downloadFiles()
{
	if ( outputFiles.contains("Image")) {
		if ( !handler->getImages(buildingXmlPath, buildingOutPath, buildingElbeID ) ) {
			qDebug() << "Could not load all images. Check Output directory and try again";
		}
		outputFiles.removeOne("Image");
	}
	if ( !handler->getFiles(outputFiles, buildingOutPath, buildingElbeID) ) {
		qDebug() << "Could not load all files. Check Output directory and try again";
	}
}

void BuildProcess::updateMessageLog(const QString &str)
{
	MainWindow *mw = helpers::getMainWindow();
	QColor color;
	color.setNamedColor("#F36363");
	mw->getMessageLog()->setTextColor(color);
	mw->getMessageLog()->append(str);
}

void BuildProcess::updateStatusBar(const QString &str)
{
	MainWindow *mw = helpers::getMainWindow();
	mw->showTempStatusOnStatusBar(str);
}

QStringList BuildProcess::getOutputFiles() const
{
	return outputFiles;
}

void BuildProcess::setOutputFiles(const QStringList &value)
{
	outputFiles = value;
}


