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

void BuildProcess::startBuild(bool sourceOptionChecked, bool binOptionChecked)
{
//	handler->startBuildProcess(sourceOptionChecked, binOptionChecked);
	buildThreadInit();
}

void BuildProcess::buildThreadInit()
{
	buildThread = new QThread();

	BuildProcessWorker *buildWorker = new BuildProcessWorker(outputFiles);

	connect(buildThread, SIGNAL(started()), buildWorker, SLOT(doWork()));
	connect(buildThread, SIGNAL(finished()), buildWorker, SLOT(deleteLater()));

	buildWorker->moveToThread(buildThread);
	buildThread->start();

//	qDebug() << QThread::currentThreadId();
}



QStringList BuildProcess::getOutputFiles() const
{
	return outputFiles;
}

void BuildProcess::setOutputFiles(const QStringList &value)
{
	outputFiles = value;
}


