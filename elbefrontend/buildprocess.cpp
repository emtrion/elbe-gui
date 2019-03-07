#include "buildprocess.h"
#include "elbehandler.h"

#include "buildprocessworker.h"
#include <QThread>
#include <QDebug>
#include "statusbarthread.h"
BuildProcess::BuildProcess(QObject *parent) : QObject(parent)
{
	handler = new ElbeHandler();
}

BuildProcess::~BuildProcess()
{
}

void BuildProcess::startBuild()
{

//	handler->startBuildProcess();
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


	statusBarWorker->moveToThread(statusBarThread);
	buildWorker->moveToThread(buildThread);

	statusBarThread->start();
	buildThread->start();


	qDebug() << statusBarThread->currentThreadId();
	qDebug() << QThread::currentThreadId();
}

void BuildProcess::handleResults(const QString &result)
{
	qDebug() << result;
	foreach (QCheckBox *file, outputFiles) {
		qDebug() << file->text();
		handler->getFile(file->text());
	}

	//end the statusbar thread
	statusBarThread->requestInterruption();
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

QList<QCheckBox *> BuildProcess::getOutputFiles() const
{
	return outputFiles;
}

void BuildProcess::setOutputFiles(const QList<QCheckBox *> &value)
{
	outputFiles = value;
}


