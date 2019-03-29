#include "buildprocessstatusbarupdate.h"

#include <QThread>
#include <QDebug>
#include <QStatusBar>

#include "mainwindow.h"
#include "helpers.h"


BuildProcessStatusBarUpdate::BuildProcessStatusBarUpdate(QObject *parent) : QObject(parent)
{

}

void BuildProcessStatusBarUpdate::changeStatusBar()
{
	connect(this, SIGNAL(statusBarHasChanged(QString)), this, SLOT(updateStatusBar(QString)));
	long i = -1;
	while ( true ) {
		i++;
		emit(statusBarHasChanged(list.at(i%4))); //index from 0 to 4 only
		QThread::sleep(1);
		if (QThread::currentThread()->isInterruptionRequested()) {//check if interrupt is requested from outside
			//clear the statusbar when build is over
			emit(statusBarHasChanged(""));
			return;
		}
	}
}

void BuildProcessStatusBarUpdate::updateStatusBar(const QString &str)
{
	MainWindow *mw = helpers::getMainWindow();
	mw->statusBar()->showMessage(str);
}


//runs in thread during the build
void BuildProcessStatusBarUpdate::statusBarBuildRunning()
{
	list = {QStringList() << "build running" << "build running."<<"build running.."<< "build running..." };
	changeStatusBar();
}


//runs in thread during the files are loaded after the thread
void BuildProcessStatusBarUpdate::statusBarLoadingFile()
{
	list = {QStringList()<<"loading files"<< "loading files."<< "loading files.."<< "loading files..." };
	changeStatusBar();
}
