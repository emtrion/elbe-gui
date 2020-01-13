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

#include "filesystemwatcher.h"

#include <QString>
#include <QDebug>
#include <QObject>
#include <QFileSystemWatcher>

#include "src/projects/project.h"
#include "src/mainwindow/mainwindow.h"
#include "helpers.h"


namespace filesystemWatcher{

	//watches filesystem to recognize changes on project from outside
	void init()
	{
		Project *projectmanager = Project::getInstance();
		//store the watcher in projectmanager to make it usable for the whole application
		projectmanager->watcher = new QFileSystemWatcher();

		MainWindow *mw = helpers::getMainWindow();
		QObject::connect(projectmanager->watcher, SIGNAL(directoryChanged(QString)), mw, SLOT(updateItemModel(QString)));
		QObject::connect(projectmanager->watcher, SIGNAL(fileChanged(QString)), mw, SLOT(updateCurrentFile(QString)));
	}

	void addPath(QString path)
	{
		Project *projectmanager = Project::getInstance();
		if ( !projectmanager->watcher->addPath(path) ) {
			qDebug() << "ERROR from"<<__func__<<": "<<path<<" could not be added";
		}
	}

	void removePath(QString path)
	{
		Project *projectmanager = Project::getInstance();
		if ( !projectmanager->watcher->removePath(path) ) {
			qDebug() << "ERROR from"<<__func__<<": "<<path<<" could not be removed";
		}
	}
}
