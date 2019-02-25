#include "projecthandler.h"

#include <QDir>
#include <QString>
#include <QDebug>
#include <QAction>

#include "helpers.h"
#include "xmlfilehandler.h"


#include "chooseprojectdialog.h"


ProjectHandler::ProjectHandler()
{
	this->projectmanager = ProjectManager::getInstance();
	elbehandler = new ElbeHandler();


}

ProjectHandler::ProjectHandler(QString path, QString name)
{
	this->newProjectPath = path;
	this->newProjectName = name;

	this->projectmanager = ProjectManager::getInstance();
	elbehandler = new ElbeHandler();
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
	if ( !helpers::setProjectMetadata(newProjectName, newProjectPath) ) {//add project specific data to
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
	helpers::addNewProjectToLookup(fi.absoluteFilePath());
	openProject(fi.absoluteFilePath());
}

void ProjectHandler::openProject(QString path)
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

//	qDebug() << "ProjectOpened: " << path;
}


void ProjectHandler::closeProject()
{
	if ( !projectmanager->isProjectOpened() ) {
		//no project to close...
		return;
	}

	helpers::watcherRemovePath(projectmanager->getSrcPath());

	projectmanager->update(QString()); //call update() with a null-string -> all properties of ProjectManager are reset
	MainWindow *mw = helpers::getMainWindow();
	mw->updateProjectStructure();

	XmlFileHandler *filehandler = new XmlFileHandler();
	filehandler->closeFile();

	projectmanager->setProjectOpened(false);
	mw->enableActionsOnProjectOpen(false);
}

void ProjectHandler::deleteProject(QString path)
{

	if ( projectmanager->getProjectPath().compare(path) == 0 ) {//check if the selected project is currently open
		closeProject();
	}

	helpers::removeProjectFromLookup(path);

	//deleteElbeInstance
//	elbehandler->deleteProjectElbeInstance();

	//delete Project from src Directory

}

ProjectManager *ProjectHandler::getProjectmanager() const
{
	return projectmanager;
}
