/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "buildprocessstatusbarupdate.h"

#include <QThread>
#include <QDebug>
#include <QStatusBar>

#include "src/mainwindow/mainwindow.h"
#include "src/app/helpers.h"


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


void BuildProcessStatusBarUpdate::statusBarUpdateProject()
{
	list = {QStringList()<<"building update-package"<< "building update-package."<< "building update-package.."<< "building update-package..." };
	changeStatusBar();
}

void BuildProcessStatusBarUpdate::statusBarCheckUpdate()
{
	list = {QStringList()<<"check for updates"<< "check for updates."<< "check for updates.."<< "check for updates..." };
	changeStatusBar();
}
