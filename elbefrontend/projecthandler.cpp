#include "projecthandler.h"

#include <QDir>
#include <QString>
#include <QDebug>

#include "helpers.h"


ProjectHandler::ProjectHandler()
{
	this->projectmanager = ProjectManager::getInstance();
}

ProjectHandler::ProjectHandler(QString path, QString name)
{
	this->newProjectPath = path;
	this->newProjectName = name;

	this->projectmanager = ProjectManager::getInstance();
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
	openProject(fi.absoluteFilePath());
}

void ProjectHandler::openProject(QString path)
{
	/*--------- Hard coded path. Should be current project later TODO ---------*/
//	path = "/home/hico/elbefrontFilehandlingTestFolder/bsp1/.project";
	/*-------------------------------------------------------------------------*/
//	projectmanager->setProjectPath(path);

	projectmanager->setProjectOpened(true);
	projectmanager->update(path);

	MainWindow *mw = helpers::getMainWindow();
	mw->updateProjectStructure();

	qDebug() << "ProjectOpened: " << path;
}


void ProjectHandler::closeProject()
{
	projectmanager->update(QString()); //call update() with a null-string -> all properties of ProjectManager are reset
	projectmanager->setProjectOpened(false);
}

ProjectManager *ProjectHandler::getProjectmanager() const
{
	return projectmanager;
}
