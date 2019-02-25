#include "helpers.h"

#include <iostream>
#include <QString>
#include <QProcessEnvironment>

#include <QWidget>
#include <QObject>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QListWidgetItem>

#include "newprojectwizard.h"
#include "projectmanager.h"
#include "mainwindow.h"


#include "projectlistitem.h"


namespace helpers {

	QString getHomeDirectoryFromSystem()
	{//get the homedirectory
        QProcessEnvironment proEnv = QProcessEnvironment::systemEnvironment();
        QString homeDir;
        homeDir = proEnv.value("HOME", NULL);

        return homeDir;
    }


	QDomDocument parseXMLFile(QFile *file)
	{
		QDomDocument doc("configFile");

		if (!file->open(QIODevice::ReadOnly)) {// Open file
			qDebug() << "ERROR from "<<__func__<<" Cannot open file";
		}

		if (!doc.setContent(file)) {// Parse file
			qDebug() << "ERROR from "<<__func__<<" Cannot parse the content";
			file->close();
		}
		file->close();
		return doc;
	}

	bool setProjectMetadata(QString projectName, QString projectPath)
	{//adds metadata to the config file

		ProjectManager *projectmanager = ProjectManager::getInstance();
		QString elbeId = projectmanager->getElbeID();
		/*----------------------------- create DOM from XML --------------------------*/

		QFile file(projectPath+"/.project");
		QDomDocument doc = helpers::parseXMLFile(&file);

		/*------------------------------- Modify DOM ---------------------------------*/

		QDomElement root = doc.firstChildElement("projectDescription"); //get root element

		QDomNode projectSettingParentNode;
		QDomNode childNode = root.firstChild();
		while (!childNode.isNull()) { //iterate over all child nodes
			QDomElement childElement = childNode.toElement();
			//look for nodes which have to be modified
			if ( childElement.tagName().compare("projectname") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectName));
			} else if (childElement.tagName().compare("elbe_id") == 0 ) {
				childNode.appendChild(doc.createTextNode(elbeId));
			} else if ( childElement.tagName().compare("source_directory") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectPath+"src/"));
			} else if ( childElement.tagName().compare("output_directory") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectPath+"out/"));
			} else if (childElement.tagName().compare("project") == 0) {
				projectSettingParentNode = childNode; //assign "project" to another node to access children
			}
			childNode = childNode.nextSibling();
		}



		ProjectManager::projectSettings settings = projectmanager->getNewProjectSettings();

		QDomNode mirrorSettingParentNode;
		QDomNode projectSettingNode = projectSettingParentNode.firstChild();


		while (!projectSettingNode.isNull()) {//iterate over "project" children nodes
			QDomElement projectSettingElement = projectSettingNode.toElement();
			if (projectSettingElement.tagName().compare("name") == 0) {
				projectSettingNode.appendChild(doc.createTextNode(settings.name));
			} else if (projectSettingElement.tagName().compare("version") == 0) {
				projectSettingNode.appendChild(doc.createTextNode(settings.version));
			} else if (projectSettingElement.tagName().compare("description") == 0) {
				projectSettingNode.appendChild(doc.createTextNode(settings.description));
			} else if (projectSettingElement.tagName().compare("buildtype") == 0) {
				projectSettingNode.appendChild(doc.createTextNode(settings.buildtype));
			} else if (projectSettingElement.tagName().compare("suite") == 0) {
				projectSettingNode.appendChild(doc.createTextNode(settings.suite));
			} else if (projectSettingElement.tagName().compare("mirror") == 0) {
				mirrorSettingParentNode = projectSettingNode; //assign "mirror" to a node to access children
			}
			projectSettingNode = projectSettingNode.nextSibling();
		}

		QDomNode mirrorSettingNode = mirrorSettingParentNode.firstChild();
		while (!mirrorSettingNode.isNull()) { //iterate over all children
			QDomElement mirrorSettingElement = mirrorSettingNode.toElement();
			if (mirrorSettingElement.tagName().compare("primary_host") == 0) {
				mirrorSettingNode.appendChild(doc.createTextNode(settings.host));
			} else if (mirrorSettingElement.tagName().compare("primary_path") == 0) {
				mirrorSettingNode.appendChild(doc.createTextNode(settings.path));
			} else if (mirrorSettingElement.tagName().compare("primary_proto") == 0) {
				mirrorSettingNode.appendChild(doc.createTextNode(settings.proto));
			}
			mirrorSettingNode = mirrorSettingNode.nextSibling();
		}

		QByteArray xml = doc.toByteArray(4);
		return saveXMLChanges(&file, xml);
	}

	bool saveXMLChanges(QFile *file, QByteArray byteArray)
	{
		QFileDevice::Permissions p = file->permissions(); //get permissions
		file->setPermissions(QFileDevice::WriteUser | p); //add permission to write
		if ( !file->open(QIODevice::Truncate | QIODevice::WriteOnly) ) {
			qDebug() << "ERROR from "<<__func__<<" Cannot open file: "<<file->fileName();
			return false;
		}

		if ( file->write(byteArray) < 0 ) {//check if write was successful
			qDebug() << "ERROR from "<<__func__<<" Cannot write to file";
			return false;
		}
		file->close();
		return true;
	}

	MainWindow* getMainWindow()
	{
		foreach( QWidget *widget, qApp->topLevelWidgets()){
			if ( MainWindow *mainWindow = qobject_cast<MainWindow*>(widget)){
				return mainWindow;
			}
		}
		return NULL;
	}

//	void deleteFile(QString path){}


	void initSystemWatcher() //watches filesystem to recognize changes on projects from outside
	{
//		qDebug() << "inside: " << __func__;
		ProjectManager *projectmanager = ProjectManager::getInstance();
		projectmanager->watcher = new QFileSystemWatcher();

		MainWindow *mw = helpers::getMainWindow();
		QObject::connect(projectmanager->watcher, SIGNAL(directoryChanged(QString)), mw, SLOT(updateItemModel(QString)));
		QObject::connect(projectmanager->watcher, SIGNAL(fileChanged(QString)), mw, SLOT(updateCurrentFile(QString)));
	}

	void watcherAddPath(QString path)
	{
		ProjectManager *projectmanager = ProjectManager::getInstance();
		if ( !projectmanager->watcher->addPath(path) ) {
			qDebug() << path<<" could not be added in "<<__func__;
		}

//		foreach (QString str, projectmanager->watcher->directories()) {
//			qDebug() << str;
//		}

//		foreach (QString str, projectmanager->watcher->files()) {
//			qDebug() << str;
//		}
	}

	void watcherRemovePath(QString path)
	{
		ProjectManager *projectmanager = ProjectManager::getInstance();

//		QStringList checkList = projectmanager->watcher->removePaths(QStringList(path));
//		if ( !checkList.isEmpty() ) {
//			qDebug() << path<<" could not be removed in "<<__func__;
//		}

		if ( !projectmanager->watcher->removePath(path) ) {
			qDebug() << path<<" could not be removed in "<<__func__;
		}
	}


	void addNewProjectToLookup(QString projectPath) //the application holds a ".elbefrontend" file where all existing projects on the system are listed
	{
		QFile lookup("/home/hico/.elbefrontend");
		if ( !lookup.open(QIODevice::ReadWrite | QIODevice::Append) ) {
			qDebug() << "ERROR from "<<__func__<<" Could not open/create file";
			return;
		}
		projectPath = projectPath+"\n";
		QByteArray input = (projectPath).toUtf8();
		lookup.write(input);
		lookup.close();
	}

	void removeProjectFromLookup(QString projectPath)
	{
		QFile lookup("/home/hico/.elbefrontend");
		QList<ProjectListItem*> list = getLookupList();

		for (int i = 0; i < list.size(); ++i) {
			QString pathToRemove = list.at(i)->getProjectPath();
			if ( pathToRemove.compare(projectPath) == 0 ) {
				list.removeAt(i);
			}
		}

		lookup.resize(0); //clear file to avoid doubled content

		foreach (ProjectListItem *item, list) {
			addNewProjectToLookup(item->getProjectPath());
		}
	}



	QList<ProjectListItem*> getLookupList()
	{
		QList<ProjectListItem*> list;
		QFile lookup("/home/hico/.elbefrontend");
		if ( !lookup.exists() ) {
			qDebug() << "ERROR from "<<__func__<<"file does not exist";
			return list;
		}
		if ( !lookup.open(QIODevice::ReadOnly) ) {
			qDebug() << "ERROR from "<<__func__<<" Could not open file";
		}
		QByteArray contentByteArray = lookup.readAll();
		QString contentString = QString().fromUtf8(contentByteArray);
		QStringList contentList = contentString.split("\n", QString::SkipEmptyParts);

		QString str;
		QString projectName;
		for (int i = 0; i < contentList.size(); ++i) {
			str = contentList.at(i);
			projectName = str.section("/", -2, -2);
			list.append(new ProjectListItem(projectName, str));
		}

		return list;
	}


}



