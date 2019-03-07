#include "buildprocessworker.h"
#include "mainwindow.h"

#include "elbehandler.h"
#include <QProcess>
#include "projectmanager.h"
#include "helpers.h"
#include <QDebug>


BuildProcessWorker::BuildProcessWorker()
{

}

BuildProcessWorker::~BuildProcessWorker()
{

}


void BuildProcessWorker::doWork()
{
	QString result;
	ProjectManager *projectmanager = ProjectManager::getInstance();
	wait_busy = new QProcess(this);

//	wait_busy->setWorkingDirectory("/home/hico/elbe");
	wait_busy->setWorkingDirectory("/home/hico/tmp");

//	wait_busy->start("./elbe control wait_busy "+projectmanager->getElbeID());
	wait_busy->start("./a.out");

	connect(wait_busy, &QProcess::readyReadStandardOutput, this, &BuildProcessWorker::printLog);
	wait_busy->waitForFinished(-1);

	result = "build done";
	emit resultReady(result);
}

void BuildProcessWorker::printLog()
{
	QString rest;
	QStringList list;
	output.append(wait_busy->readAllStandardOutput());
	if(!output.endsWith("\n")) {
		list = output.split("\n");
		rest = list.last();
		list.removeLast();
	} else {
		list = output.split("\n", QString::SkipEmptyParts);
	}


	foreach (QString str, list) {
		qDebug() << str;
		emit(outputReady(str));
	}

	output = "";
	output.append(rest);
}



