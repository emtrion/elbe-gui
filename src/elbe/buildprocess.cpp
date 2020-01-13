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

#include "buildprocess.h"

#include <QThread>
#include <QDebug>

#include "elbehandler.h"
#include "buildmanager.h"
#include "buildprocessworker.h"
#include "buildprocessstatusbarupdate.h"
#include "src/mainwindow/mainwindow.h"
#include "src/app/helpers.h"
#include "src/projects/projecthandler.h"

BuildProcess::BuildProcess(QObject *parent) : QObject(parent)
{
	buildmanager = BuildManager::getInstance();
	buildWorker = new BuildProcessWorker(buildmanager->outputFiles());
}

BuildProcess::~BuildProcess()
{
}

void BuildProcess::startBuild(bool sourceOptionChecked, bool binOptionChecked)
{
	MainWindow *mw = helpers::getMainWindow();

	mw->setWindowTitle("elbeFrontend (build running)");

	if ( ElbeHandler::checkIfBusy(buildWorker->buildingElbeID()) ) {
		buildWorker->updateMessageLog("The project is busy right now");
		return;
	}

	buildmanager->setBuildRunning(true);
	mw->changeElbeActionsEnabledStatus(false);
	if ( !ElbeHandler::startBuildProcess(sourceOptionChecked, binOptionChecked) ) {
		buildmanager->setBuildRunning(false);
		mw->changeElbeActionsEnabledStatus(true);
		buildWorker->updateMessageLog("Couldn't start build. Most likely there is a problem with your initVM");
		mw->setWindowTitle("elbeFrontend");
		return;
	}
	buildWorker->setSkipDownload(false);
	buildThreadInit();
}


/*This method is used when the application was closed and restarted during a build.
 * It makes sure that the build has finished or if not just contionues as normal*/
void BuildProcess::waitBusyWithoutStartingBuild(QString elbeid)
{
	MainWindow *mw = helpers::getMainWindow();
	buildmanager->setBuildRunning(true);
	mw->changeElbeActionsEnabledStatus(false);
	buildWorker->setSkipDownload(true);
	buildWorker->setBuildingElbeID(elbeid);
	buildThreadInit();
}

void BuildProcess::buildThreadInit()
{
	buildThread = new QThread();

	//pass the instance to buildmanager so the threads can be handled outside
	buildmanager->setProcessWorkerPointer(buildWorker);

	connect(buildThread, SIGNAL(started()), buildWorker, SLOT(doWork()));
	connect(buildThread, SIGNAL(finished()), buildWorker, SLOT(deleteLater()));
	connect(buildWorker, SIGNAL(done()), this, SLOT(cleanup()));

	buildWorker->moveToThread(buildThread);
	buildThread->start();
}


void BuildProcess::cleanup()
{
	MainWindow *mw = helpers::getMainWindow();
	mw->changeElbeActionsEnabledStatus(true);
	mw->setWindowTitle("elbeFrontend");
	buildThread->quit();
	buildThread->wait();

	Project *project = Project::getInstance();
	Updates *updates = project->getUpdates();
	updates->makeInitialEntry();
	mw->getUpdatetab()->enableButtons();
}


