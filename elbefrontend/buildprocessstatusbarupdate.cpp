#include "buildprocessstatusbarupdate.h"
#include <QThread>
#include <QDebug>
#include "mainwindow.h"
#include "helpers.h"
#include <QStatusBar>

BuildProcessStatusBarUpdate::BuildProcessStatusBarUpdate(QObject *parent) : QObject(parent)
{

}

void BuildProcessStatusBarUpdate::changeStatusBar()
{
	connect(this, SIGNAL(statusBarHasChanged(QString)), this, SLOT(updateStatusBar(QString)));
	//qDebug() << __func__<<" is in: "<<QThread::currentThreadId();
	long i = -1; //-1 because first index is 0
	while ( true ) {
		i++; //counting up...
		emit(statusBarHasChanged(list.at(i%4))); //...but still get numbers from 0 to 4
		QThread::sleep(1); //wait for 1 second
		if (QThread::currentThread()->isInterruptionRequested()) {//check if interrupt is requested from outside
//			qDebug() << "interrupt requested";
			//clear the statusbar when build is over
			emit(statusBarHasChanged(""));
			return;
		}
	}
}

void BuildProcessStatusBarUpdate::updateStatusBar(const QString &str)
{
	//qDebug() << __func__<<" is in: "<<QThread::currentThreadId();

	MainWindow *mw = helpers::getMainWindow();
//	mw->showTempStatusOnStatusBar(str);
	mw->statusBar()->showMessage(str);
}


//runs in thread during the build
void BuildProcessStatusBarUpdate::statusBarBuildRunning()
{
//	qDebug() << __func__<<" called which means signal came through";
	list = {QStringList() << "build running" << "build running."<<"build running.."<< "build running..." };
	changeStatusBar();
}


//runs in thread during the files are loaded after the thread
void BuildProcessStatusBarUpdate::statusBarLoadingFile()
{
//	qDebug() << __func__<<" called which means signal came through";
	list = {QStringList()<<"loading files"<< "loading files."<< "loading files.."<< "loading files..." };
	changeStatusBar();
}
