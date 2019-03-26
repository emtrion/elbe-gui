#include "projecthandler.h"

#include <QDir>
#include <QString>
#include <QDebug>
#include <QAction>

#include "helpers.h"
#include "xmlfilehandler.h"


#include "deletedialog.h"
#include "elbehandler.h"
#include "existingprojects.h"
#include "mainwindow.h"


ProjectHandler::ProjectHandler()
{
	this->projectmanager = ProjectManager::getInstance();
	elbehandler = new ElbeHandler();
	filemanager = XmlFileManager::getInstance();
}

ProjectHandler::ProjectHandler(QString path, QString name)
{
	this->newProjectPath = path;
	this->newProjectName = name;

	this->projectmanager = ProjectManager::getInstance();
	elbehandler = new ElbeHandler();
	filemanager = XmlFileManager::getInstance();
}


void ProjectHandler::createProject()
{ //creates the directory
	if ( QDir().exists(newProjectPath) ) {
		//no need to create an existing project
	} else {
		if ( !QDir().mkpath(newProjectPath) ) {//create directory and all parentdirectories necessary
			//returns false when it couldn't create the directory
			qDebug() << "ERROR from "<<__func__<<" problem while creating project directory";
			return;
		}
	}

	projectmanager->setElbeID(elbehandler->createProjectElbeInstance());

	QFile::copy(":/projectconfig.xml", newProjectPath+"/.project"); //copy conf-file template to directory
	QFile confFile(newProjectPath+"/.project");
	if ( !helpers::setProjectMetadata(newProjectName, newProjectPath) ) {//add project specific data
		confFile.remove(); //if an error occurs while creating ".project", it will be removed from the directory
		qDebug() << "ERROR from "<<__func__<<" problem while creating config file";
		return;
	}

	if ( !QDir().mkpath(newProjectPath+"src/") || !QDir().mkpath(newProjectPath+"out/") ) {
		//create directory for the elbe-XML file and the output from elbe
		confFile.remove();//if an error occurs while creating "src" and "out", ".project" has to be removed
		qDebug() << "ERROR from "<<__func__<<" problem while creating source and output directories";
		return;
	}

	QFileInfo fi(confFile);
	ExistingsProjects().addNewProjectToList(fi.absoluteFilePath());
	projectmanager->setProjectHasFile(false);
	openProject(fi.absoluteFilePath());
}

void ProjectHandler::openProject(QString path) //path to .project file
{
	if ( projectmanager->isProjectOpened() ) {
		//close open project before opening a new one
		closeProject();
	}

	projectmanager->update(path);
	MainWindow *mw = helpers::getMainWindow();
	mw->updateProjectStructure();
	projectmanager->setProjectOpened(true);

	mw->enableActionsOnProjectOpen(true);
	helpers::watcherAddPath(projectmanager->getSrcPath());
	helpers::watcherAddPath(projectmanager->getOutPath());

	if ( checkIfProjectHasXML(path) ) {
		projectmanager->setProjectHasFile(true);
		mw->changeNewXmlButtonEnabledStatus(false);
		mw->changeImportButtonEnabledStatus(false);
	}

//	qDebug() << "ProjectOpened: " << path;
}


bool ProjectHandler::checkIfProjectHasXML(QString path)
{
	QDir project(path);
	if ( !project.cd("../src/") ) {//navigate to /src/
		qDebug() << "src dir doesn't exist";
		return false;
	}

	//get all files from src
	QFileInfoList entryList = project.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

//	qDebug() << entryList.first().absoluteFilePath();

	if ( !project.isEmpty() ) {
		if ( entryList.size() == 1 ) { //entry list should be exactly one as only one xml file is expected
			projectmanager->setBuildXmlPath(entryList.first().absoluteFilePath());
		} else {
			qDebug() << "WARNING: src has got more than one file. Couldn't determine which file is the intended. Please remove all files but the intended";
		}
		return true;
	} else {
		return false;
	}
}

void ProjectHandler::closeProject()
{
	if ( !projectmanager->isProjectOpened() ) {
		//no project to close...
		return;
	}

	helpers::watcherRemovePath(projectmanager->getSrcPath());
	helpers::watcherRemovePath(projectmanager->getOutPath());

	projectmanager->update(QString()); //call update() with a null-string -> all properties of ProjectManager are reset
	MainWindow *mw = helpers::getMainWindow();
	mw->updateProjectStructure();


	if ( filemanager->getIsOpen() ) {
		XmlFileHandler *filehandler = new XmlFileHandler();
		filehandler->closeFile();
	}

	projectmanager->setProjectOpened(false);
	mw->enableActionsOnProjectOpen(false);
}

void ProjectHandler::deleteProject(QString path)
{

	if ( projectmanager->getProjectPath().compare(path) == 0 ) {//check if the selected project is currently open
		closeProject(); //if so, close it
	}

	//deleteElbeInstance
	if ( !elbehandler->deleteProjectElbeInstance(path) ) {
		qDebug() << "delete from elbe failed! It may be already deleted.";
	}
	ExistingsProjects().removeProjectFromList(path);
	//delete Project from src Directory
	QDir projectDir(path);
	projectDir.cdUp(); //move one up because path = .../.project
	if ( !projectDir.removeRecursively() ) {
		qDebug() << "ERROR from "<<__func__<<" could not remove project from filesystem";
		return;
	}

}

ProjectManager *ProjectHandler::getProjectmanager() const
{
	return projectmanager;
}
