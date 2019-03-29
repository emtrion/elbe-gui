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
