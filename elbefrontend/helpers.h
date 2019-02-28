#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include "newprojectwizard.h"
#include "mainwindow.h"
#include <QDomDocument>
#include <QFile>
#include <QByteArray>
#include <QPixmap>
#include <QListWidgetItem>
#include "projectlistitem.h"

namespace helpers {
    QString getHomeDirectoryFromSystem();
	bool setProjectMetadata(QString projectName, QString projectPath);
//	void deleteFile(QString path);
	MainWindow* getMainWindow();
	QDomDocument parseXMLFile(QFile *file);
	bool saveXMLChanges(QFile *file, QByteArray byteArray);
	void setPixmaps();
	void initSystemWatcher();
	void watcherAddPath(QString path);
	void watcherRemovePath(QString path);
	QString getProjectID(QString confFile);
	QString getProjectName(QString confFile);
	QString parseProjectDescriptionForNodeValue(QString confFile, QString node);


}

#endif // HELPERS_H
