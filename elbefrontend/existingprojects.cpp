#include "elbehandler.h"
#include "existingprojects.h"
#include "projectlistitem.h"

#include <QString>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QList>
#include <QDebug>
#include "helpers.h"

ExistingProjects::ExistingProjects(QObject *parent) : QObject(parent)
{
	projectListFile = new QFile("/home/hico/.elbefrontend");

}

void ExistingProjects::addNewProjectToList(QString projectPath) //the application holds a ".elbefrontend" file where all existing projects on the system are listed
{
	if ( !projectListFile->open(QIODevice::ReadWrite | QIODevice::Append) ) {
		qDebug() << "ERROR from "<<__func__<<" Could not open/create file";
		return;
	}
	projectPath = projectPath+"\n";
	QByteArray input = (projectPath).toUtf8();
	projectListFile->write(input);
	projectListFile->close();
}

void ExistingProjects::removeProjectFromList(QString projectPath)
{
	for (int i = 0; i < projectFileList.size(); ++i) {
		QString pathToRemove = projectFileList.at(i);
		if ( pathToRemove.compare(projectPath) == 0 ) {
			projectFileList.removeAt(i);
		}
	}

	projectListFile->resize(0); //clear file to avoid doubled content


	foreach (QString str, projectFileList) {
//		qDebug() << str;
		addNewProjectToList(str);
	}
}

QList<ProjectListItem *> ExistingProjects::getExistingProjects()
{
	initFileList();
	updateList();
	return existingProjects;
}

void ExistingProjects::updateList() //is called from getExistingProjects
{
	ElbeHandler *elbe = new ElbeHandler();
	bool isInElbe;
	existingProjects.clear(); //clear the list to avoid doubled items
	for (int i = 0; i < projectFileList.size(); ++i) {
		QFileInfo file(projectFileList.at(i));

//		qDebug() << "absoluteFilePath: "<<file.absoluteFilePath();
		isInElbe = elbe->projectIsInElbe(file.absoluteFilePath());
//		qDebug() << "isInElbe = "<<isInElbe;
//		qDebug() << file.absolutePath();
		//check if the projects are in any unwanted state, i.e. ...
		if ( !file.exists() && !isInElbe ) {//...not being existend at all
//			qDebug() <<__func__<<": "<<file.absoluteFilePath()<<" doesn't exist";
			removeProjectFromList(file.absoluteFilePath());
		} else if ( file.exists() && !isInElbe ) { //...existend on filesystem but not in elbe
			putItemInList(helpers::getProjectName(file.absoluteFilePath())+" (has no elbe instance)" ,file.absoluteFilePath());
		} else if ( !file.exists() && isInElbe ) { //...existend in elbe but not on filesystem
			putItemInList(helpers::getProjectName(file.absoluteFilePath())+" (only in elbe)", file.absoluteFilePath());
		} else {
			putItemInList(helpers::getProjectName(file.absoluteFilePath()), file.absoluteFilePath());
		}
	}
}

/*marks the project which was still building when the application was closed*/
void ExistingProjects::addBusyFlag(const QString &projectPath)
{
	int index = 0;
	initFileList();
	foreach (QString str, projectFileList) {
		if ( str.compare(projectPath) == 0 ) {
			projectFileList.replace(index, str+" (busy)");
			break;
		}
		index++;
	}

	projectListFile->resize(0);

	foreach (QString str, projectFileList) {
		addNewProjectToList(str);
	}
}

QString ExistingProjects::checkForBusyFlag()
{
	QString projectPath = "";
	int index = 0;
	initFileList();
	foreach (QString str, projectFileList) {
		if ( str.endsWith(" (busy)") ) {
			projectPath = removeBusyFlag(index);
		}
		index++;
	}
	//if no busyFlag was found it returns an empty string
	return projectPath;
}

QString ExistingProjects::removeBusyFlag(int index)
{
	QStringList strList;
	QString busyProject = projectFileList.at(index);
	//split at space character to get the directory seperated
	strList = busyProject.split(QRegExp("\\s"), QString::SkipEmptyParts);
	//there can be only two entries and the first one (index = 0) has to be the directory
	projectFileList.replace(index, strList.at(0));

	projectListFile->resize(0);
	foreach (QString s, projectFileList) {
		addNewProjectToList(s);
	}

	//return the projectPath without the busy flag
	return strList.at(0);
}

void ExistingProjects::initFileList()
{
	if ( !projectListFile->exists() ) {
		qDebug() << "ERROR from "<<__func__<<"file does not exist";
		return;
	}
	if ( !projectListFile->open(QIODevice::ReadOnly) ) {
		qDebug() << "ERROR from "<<__func__<<" Could not open file";
		return;
	}
	QByteArray contentByteArray = projectListFile->readAll();
	projectListFile->close();

	QString contentString = QString().fromUtf8(contentByteArray);

	projectFileList = contentString.split("\n", QString::SkipEmptyParts);
}

void ExistingProjects::putItemInList(QString name, QString path)
{
	existingProjects.append(new ProjectListItem(name, path));
}







