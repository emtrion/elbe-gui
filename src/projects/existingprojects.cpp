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

#include "existingprojects.h"

#include <QString>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QList>
#include <QDebug>

#include "src/app/helpers.h"
#include "src/xml/xmlutilities.h"
#include "src/projects/projectlistitem.h"
#include "src/elbe/elbehandler.h"

ExistingProjects::ExistingProjects(QObject *parent) :
	QObject(parent)
{
	//the application holds a file where all existing projects on the system are listed
	projectListFile = new QFile(helpers::getHomeDirectoryFromSystem()+"/.elbefrontend/existingProjects");
}

/*----------------------- interaction with the file ------------------------*/

bool ExistingProjects::exists()
{
	return projectListFile->exists();
}

void ExistingProjects::createprojectListFile()
{
	if ( !projectListFile->open(QIODevice::ReadWrite) ) {
		qDebug() << "ERROR from"<<__func__<<" Could not create file";
		return;
	} else {
		projectListFile->close();
	}
}

void ExistingProjects::addNewProjectToListFile(QString projectPath)
{
	if ( !projectListFile->open(QIODevice::ReadWrite | QIODevice::Append) ) {
		qDebug() << "ERROR from "<<__func__<<" Could not open file";
		return;
	}
	projectPath = projectPath+"\n";
	QByteArray input = (projectPath).toUtf8();
	projectListFile->write(input);
	projectListFile->close();
}

void ExistingProjects::removeProjectFromListFile(QString projectPath)
{
	QStringList projectFileList = initFileList();
	for (int i = 0; i < projectFileList.size(); ++i) {
		QString pathToRemove = projectFileList.at(i);
		if ( pathToRemove.compare(projectPath) == 0 ) {
			projectFileList.removeAt(i);
		}
	}
	updateListFile(projectFileList);
}

void ExistingProjects::updateListFile(const QStringList &projectFileList)
{
	//clear the file to avoid doubled content
	projectListFile->resize(0);
	foreach (QString str, projectFileList) {
		addNewProjectToListFile(str);
	}
}

/*--------------- copy file change content and write it back ----------------*/

//creates a representation of the existing projects file to work with
QStringList ExistingProjects::initFileList()
{
	QStringList emptyList;
	if ( !projectListFile->exists() ) {
		qDebug() << "ERROR from "<<__func__<<"file does not exist";
		return emptyList;
	}
	if ( !projectListFile->open(QIODevice::ReadOnly) ) {
		qDebug() << "ERROR from "<<__func__<<" Could not open file";
		return emptyList;
	}
	QByteArray contentByteArray = projectListFile->readAll();
	projectListFile->close();

	QString contentString = QString().fromUtf8(contentByteArray);
	return contentString.split("\n", QString::SkipEmptyParts);
}

void ExistingProjects::addOpenFlag(const QString &projectPath)
{
	int index = 0;
	QStringList projectFileList = initFileList();
	foreach (QString str, projectFileList) {
		if ( str.compare(projectPath+" (busy)") == 0 || str.compare(projectPath) == 0 ) {
			projectFileList.replace(index, str+" (open)");
			break;
		}
		index++;
	}
	updateListFile(projectFileList);
}

QString ExistingProjects::checkForOpenFlag()
{
	QString projectPath = "";
	int index = 0;
	QStringList projectFileList = initFileList();
	foreach (QString str, projectFileList) {
		if ( str.endsWith(" (open)") ) {
			projectPath = removeOpenFlag(index, projectFileList);
		}
		index++;
	}
	return projectPath;
}

QString ExistingProjects::removeOpenFlag(int index, QStringList projectFileList)
{
	QStringList strList;
	QString openedProject = projectFileList.at(index);
	//split at space character to get the directory seperated
	strList = openedProject.split(" ", QString::SkipEmptyParts);
	//open is always at the end
	strList.removeLast();
	//replace entry at index with joined strList
	//strList is joined to one string seperated with a space
	projectFileList.replace(index, strList.join(" "));

	//clear the file to avoid doubled content
	projectListFile->resize(0);

	foreach (QString s, projectFileList) {
		addNewProjectToListFile(s);
	}

	//first entry of strList holds the file only, without any flags
	return strList.at(0);
}

//marks the project which was still building when the application was closed
void ExistingProjects::addBusyFlag(const QString &projectPath)
{
	int index = 0;
	QStringList projectFileList = initFileList();
	foreach (QString str, projectFileList) {
		if ( str.compare(projectPath) == 0 ) {
			projectFileList.replace(index, str+" (busy)");
			break;
		}
		index++;
	}
	updateListFile(projectFileList);
}


QString ExistingProjects::checkForBusyFlag()
{
	QString projectPath = "";
	int index = 0;
	QStringList projectFileList = initFileList();
	foreach (QString str, projectFileList) {
		if ( str.endsWith(" (busy)") ) {
			projectPath = removeBusyFlag(index, projectFileList);
		}
		index++;
	}
	//if no busyFlag was found it returns an empty string
	return projectPath;
}

QString ExistingProjects::removeBusyFlag(int index, QStringList projectFileList)
{
	QStringList strList;
	QString busyProject = projectFileList.at(index);
	//split at space character to get the directory seperated
	strList = busyProject.split(" ", QString::SkipEmptyParts);
	//there can be only two entries and the first one (index = 0) has to be the directory
	projectFileList.replace(index, strList.at(0));

	updateListFile(projectFileList);

	//return the projectPath without the busy flag
	return strList.at(0);
}

/*----------- create and return a equal list but with ProjectListItems ----------*/

//creates a list of ProjectListItems so it can be used in a QListWidget later
QList<ProjectListItem *> ExistingProjects::createExistingProjectsList()
{
	QStringList projectFileList = initFileList();
	bool isInElbe;
	bool isBusy;
	QList<ProjectListItem *> existingProjects;

	for (auto i = 0; i < projectFileList.size(); ++i) {
		QFileInfo file(projectFileList.at(i));
		isInElbe = ElbeHandler::projectIsInElbe(file.absoluteFilePath());
		isBusy = ElbeHandler::projectIsBuilding(file.absoluteFilePath());
		//check if the projects are in any unwanted state, i.e. ...
		if ( !file.exists() && !isInElbe ) {//...not being existend at all
			removeProjectFromListFile(file.absoluteFilePath());
		} else if ( file.exists() && !isInElbe ) { //...existend on filesystem but not in elbe
			existingProjects.append(new ProjectListItem(
										xmlUtilities::getProjectName(file.absoluteFilePath())+" (has no elbe instance)" ,file.absoluteFilePath())
									);
		} else if ( !file.exists() && isInElbe ) { //...existend in elbe but not on filesystem
			existingProjects.append(new ProjectListItem(
										xmlUtilities::getProjectName(file.absoluteFilePath())+" (exists only in elbe)", file.absoluteFilePath())
									);
		} else if ( file.exists() && isBusy ) { //...existend but busy
			//skip this one
			//we don't want to delete a busy project
		} else {
			existingProjects.append(new ProjectListItem(
										xmlUtilities::getProjectName(file.absoluteFilePath()), file.absoluteFilePath())
									);
		}
	}
	return existingProjects;
}

QList<ProjectListItem *> ExistingProjects::existingProjects()
{
	return createExistingProjectsList();
}








