#include "statusbarthread.h"
#include <QThread>
#include <QDebug>

StatusBarThread::StatusBarThread(QObject *parent) : QObject(parent)
{

}

void StatusBarThread::changeStatusBar()
{
	long i = -1;
	QStringList list = {"build running", "build running.", "build running..", "build running..." };
	while ( true ) {
		i++;
		emit(statusBarHasChanged(list.at(i%4)));
		QThread::sleep(1);
		if (QThread::currentThread()->isInterruptionRequested()) {
			//clear the statusbar when build is over
			emit(statusBarHasChanged(""));
			return;
		}
	}
}



