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

#include "projecthandler.h"

#include <QDir>
#include <QDebug>

#include "src/app/helpers.h"
#include "src/xml/xmlfilehandler.h"
#include "src/elbe/elbehandler.h"
#include "existingprojects.h"
#include "src/mainwindow/mainwindow.h"
#include "src/xml/xmlutilities.h"
#include "src/app/filesystemwatcher.h"
#include "project.h"


namespace ProjectHandler {

	void createProject(const QString newProjectPath, const QString newProjectName)
	{
		Project *projectmanager = Project::getInstance();

		if ( QDir().exists(newProjectPath) ) {
			//should not be happening
			//that case is supposed to be already handled in new project wizard
			return;
		} else {
			//create directory and all parentdirectories necessary
			if ( !QDir().mkpath(newProjectPath) ) {
				qDebug() << "ERROR from "<<__func__<<" problem while creating project directory";
				return;
			}
		}

		projectmanager->setElbeID(ElbeHandler::createProjectElbeInstance());

		QFile::copy(":/projectconfig.xml", newProjectPath+"/.project");
		QFile confFile(newProjectPath+"/.project");
		if ( !xmlUtilities::setProjectMetadata(newProjectName, newProjectPath) ) {
			confFile.remove();
			qDebug() << "ERROR from "<<__func__<<" problem while creating config file";
			return;
		}

		if ( !QDir().mkpath(newProjectPath+"src/") || !QDir().mkpath(newProjectPath+"out/") ) {
			confFile.remove();
			qDebug() << "ERROR from "<<__func__<<" problem while creating source and output directories";
			return;
		}


		//create update_history file
		QFile updateFile(newProjectPath+"/.update_history");
		updateFile.open(QIODevice::ReadWrite);
		updateFile.close();


		QFileInfo fi(confFile);
		ExistingProjects().addNewProjectToListFile(fi.absoluteFilePath());
		projectmanager->setProjectHasFile(false);
		openProject(fi.absoluteFilePath());
	}

	//path to .project file
	void openProject(QString path)
	{
		Project *projectmanager = Project::getInstance();

		if ( projectmanager->isProjectOpened() ) {
			//close open project before opening a new one
			closeProject();
		}

		projectmanager->update(path);
		MainWindow *mw = helpers::getMainWindow();
		mw->updateProjectStructure();
		projectmanager->setProjectOpened(true);
		mw->enableActionsOnProjectOpen(true);
		filesystemWatcher::addPath(projectmanager->srcPath());
		filesystemWatcher::addPath(projectmanager->outPath());

		if ( checkIfProjectHasXML(path) ) {
			projectmanager->setProjectHasFile(true);
			mw->changeNewXmlButtonEnabledStatus(false);
			mw->changeImportButtonEnabledStatus(false);		
		}

		//show updates
		Updates *updates = new Updates(projectmanager->projectDirectory());
		projectmanager->setUpdates(updates);
		UpdateTab *utab = new UpdateTab();
		utab->updateView(updates);
		mw->showUpdateTab(utab);

		updates->initUpdateList();
	}

	bool checkIfProjectHasXML(QString path)
	{
		Project *projectmanager = Project::getInstance();
		MainWindow *mainwindow = helpers::getMainWindow();

		QDir project(path);
		if ( !project.cd("../src/") ) {//navigate to /src/
			qDebug() << "ERROR from"<<__func__<<": src directory doesn't exist for "<<projectmanager->projectName();
			return false;
		}

		//get all files from src
		QFileInfoList entryList = project.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
		if ( !project.isEmpty() ) {
			if ( entryList.size() == 1 ) { //entry list should be exactly one as only one xml file is expected
				projectmanager->setBuildXmlPath(entryList.first().absoluteFilePath());
			} else {
				mainwindow->messageLogAppendText("WARNING: sourcedirectory has got more than one file. "
												 "Couldn't determine which file is the intended. Please remove all files but the intended",
												 "#F36363");
			}
			return true;
		} else {
			return false;
		}
	}

	void closeProject()
	{
		Project *projectmanager = Project::getInstance();
		XmlFile *filemanager = XmlFile::getInstance();
		MainWindow *mw = helpers::getMainWindow();

		if ( !projectmanager->isProjectOpened() ) {
			//no project to close...
			return;
		}

		filesystemWatcher::removePath(projectmanager->srcPath());
		filesystemWatcher::removePath(projectmanager->outPath());

		//call update() with a null-string -> all properties of ProjectManager are reset
		projectmanager->update(QString());
		mw->updateProjectStructure();

		if ( filemanager->isOpen() ) {
			XmlFileHandler::closeFile();
		}

		projectmanager->setProjectOpened(false);
		mw->enableActionsOnProjectOpen(false);

		mw->hideUpdateTab();
	}

	void deleteProject(QString path)
	{
		Project *projectmanager = Project::getInstance();

		if ( projectmanager->projectPath().compare(path) == 0 ) {//check if the selected project is currently open
			closeProject(); //if so, close it
		}

		//deleteElbeInstance
		if ( !ElbeHandler::deleteProjectElbeInstance(path) ) {
			qDebug() << "ERROR from"<<__func__<<": delete from elbe failed! It may be already deleted.";
		}
		ExistingProjects().removeProjectFromListFile(path);
		//delete Project from filesystem
		QDir projectDir(path);
		projectDir.cdUp(); //move one up because path = .../.project
		if ( !projectDir.removeRecursively() ) {
			qDebug() << "ERROR from "<<__func__<<" could not remove project from filesystem";
			return;
		}
	}

	void reopenProject()
	{
		Project *project = Project::getInstance();
		QString path = project->projectPath();

		closeProject();
		openProject(path);
	}
}
