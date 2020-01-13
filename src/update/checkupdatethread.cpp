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

#include "checkupdatethread.h"
#include "src/elbe/elbehandler.h"

#include <src/elbe/buildprocessstatusbarupdate.h>

CheckUpdateThread::CheckUpdateThread()
{

}

void CheckUpdateThread::run()
{
	QString result;

	showCheckUpdateInStatusBar();

	result = ElbeHandler::checkForUpdates();


	statusBarCheckUpdateThread->requestInterruption();
	statusBarCheckUpdateThread->quit();
	statusBarCheckUpdateThread->wait();


	emit resultReady(result);

}


void CheckUpdateThread::showCheckUpdateInStatusBar()
{
	BuildProcessStatusBarUpdate *statusBarCheckUpdateWorker = new BuildProcessStatusBarUpdate();
	statusBarCheckUpdateThread = new QThread();
	connect(statusBarCheckUpdateThread, SIGNAL(started()), statusBarCheckUpdateWorker, SLOT(statusBarCheckUpdate()));
	connect(statusBarCheckUpdateThread, SIGNAL(finished()), statusBarCheckUpdateWorker, SLOT(deleteLater()));
	statusBarCheckUpdateWorker->moveToThread(statusBarCheckUpdateThread);
	statusBarCheckUpdateThread->start();
}
