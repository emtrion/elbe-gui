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

#include "buildprocessworker.h"

#include <QProcess>
#include <QDebug>

#include "src/projects/project.h"
#include "src/app/helpers.h"
#include "buildmanager.h"
#include "src/mainwindow/mainwindow.h"
#include "buildprocessstatusbarupdate.h"
#include "elbehandler.h"

BuildProcessWorker::BuildProcessWorker(QStringList outputFiles)
{
	//outputfiles which are seleceted in buildprocessstartdialog
	this->outputFiles = outputFiles;

	projectmanager = Project::getInstance();
	buildmanager = BuildManager::getInstance();
	m_buildingElbeID = projectmanager->elbeID();
	buildingXmlPath = projectmanager->buildXmlPath();
	buildingOutPath = projectmanager->outPath();
	m_buildingProjectPath = projectmanager->projectPath();

	MainWindow *mw = helpers::getMainWindow();
	connect(this, SIGNAL(messageLogHasUpdate(QString,QString)), mw, SLOT(messageLogAppendText(QString,QString)));
}

BuildProcessWorker::~BuildProcessWorker()
{}

void BuildProcessWorker::doWork()
{
	process = new QProcess(this);

	//it's not allowed to communicate with the GUI directly from another thread but the mainthread
	connect(this, SIGNAL(outputReady(QString)), this, SLOT(updateMessageLog(QString)));
	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(printLog()));

	//exec the wait_busy command where we wait for the build to finish and receive the build log messages
	waitBusy();
	//was set before the actual build command in "buildmanager". After waitBusy returns we can be sure the build has finished
	buildmanager->setBuildRunning(false);


	updateMessageLog(" ");

	//
	if ( !skipDownload ) {
		//inidcates that a download is currently running
		buildmanager->setLoadingFiles(true);
		downloadFiles();
		buildmanager->setLoadingFiles(false);
	} else {
		updateMessageLog("skip download");
	}

	emit(done());
}


void BuildProcessWorker::waitBusy()
{
	//start to show "build" in the status bar
	showBuildingInStatusBar();

	emit(outputReady("start elbe-build"));

	process->setWorkingDirectory(buildmanager->elbeWorkingDir());
	process->start(buildmanager->elbeCommandPrefix()+"elbe control wait_busy "+buildingElbeID());

	//wait until build has finished. Events are still handled so messages are coming through
	process->waitForFinished(-1);

	updateMessageLog("finished");

	//stop the loop inside the thread
	m_statusBarBuildThread->requestInterruption();

	//end the thread
	m_statusBarBuildThread->quit();
	m_statusBarBuildThread->wait();
}

void BuildProcessWorker::setSkipDownload(bool value)
{
	skipDownload = value;
}

/*prints the elbe build output in the MessageLog*/
void BuildProcessWorker::printLog()
{
	QString tail; //if the last part of the read output isn't a complete line it's stored in here
	QStringList list;
	output.append(process->readAllStandardOutput());
	if(!output.endsWith("\n")) { //check if last part is a complete line
		list = output.split("\n");
		tail = list.last();
		list.removeLast(); //remove incomplete part
	} else {
		list = output.split("\n", QString::SkipEmptyParts);
	}
	foreach (QString str, list) {
		emit(outputReady(str)); //connected to updateMessageLog
	}

	output = ""; //clear output to read new data
	output.append(tail); //put the last part at the beginning of the new data
}


void BuildProcessWorker::updateMessageLog(const QString &str)
{
	//calls mainwindow slot messageLogAppendText
	emit(messageLogHasUpdate(str, "#F36363"));
}

void BuildProcessWorker::downloadFiles()
{
	//change statusbar message
	showLoadingInStatusBar();
	updateMessageLog("downloading files...");

	//check whether images are selected to download after build
	if ( outputFiles.contains("Image")) {
		if ( !ElbeHandler::getImages(buildingXmlPath, buildingOutPath, m_buildingElbeID ) ) {
			updateMessageLog("Could not load all images. Check Output directory and try again");
		} else {
			updateMessageLog("images loaded.");
		}
		//we don't want to get an error because image is not a filename
		outputFiles.removeOne("Image");
	}
	//if nothing else was selected outputFiles should now be empty
	if( !outputFiles.isEmpty() ) {
		if ( !ElbeHandler::getFiles(outputFiles, buildingOutPath, m_buildingElbeID) ) {
			updateMessageLog("Could not load all files. Check Output directory and try again");
		} else {
			updateMessageLog("files loaded.");
		}
	}

	statusBarLoadThread->requestInterruption();

	statusBarLoadThread->quit();
	statusBarLoadThread->wait();

	updateMessageLog(" ");
	updateMessageLog("finished");

	return;
}

//initialize statusbar during the build
void BuildProcessWorker::showBuildingInStatusBar()
{
	BuildProcessStatusBarUpdate *statusBarBuildWorker = new BuildProcessStatusBarUpdate();
	m_statusBarBuildThread = new QThread();
	connect(m_statusBarBuildThread, SIGNAL(started()), statusBarBuildWorker, SLOT(statusBarBuildRunning()));
	connect(m_statusBarBuildThread, SIGNAL(finished()), statusBarBuildWorker, SLOT(deleteLater()));
	statusBarBuildWorker->moveToThread(m_statusBarBuildThread);
	m_statusBarBuildThread->start();
}

//initialize statusbar during files are downloaded
void BuildProcessWorker::showLoadingInStatusBar()
{
	BuildProcessStatusBarUpdate *statusBarLoadWorker = new BuildProcessStatusBarUpdate();
	statusBarLoadThread = new QThread();
	connect(statusBarLoadThread, SIGNAL(started()), statusBarLoadWorker, SLOT(statusBarLoadingFile()));
	connect(statusBarLoadThread, SIGNAL(finished()), statusBarLoadWorker, SLOT(deleteLater()));
	statusBarLoadWorker->moveToThread(statusBarLoadThread);
	statusBarLoadThread->start();
}


QThread *BuildProcessWorker::statusBarBuildThread() const
{
	return m_statusBarBuildThread;
}

QString BuildProcessWorker::buildingProjectPath() const
{
	return m_buildingProjectPath;
}

QString BuildProcessWorker::buildingElbeID() const
{
	return m_buildingElbeID;
}

void BuildProcessWorker::setBuildingElbeID(const QString &value)
{
	m_buildingElbeID = value;
}
