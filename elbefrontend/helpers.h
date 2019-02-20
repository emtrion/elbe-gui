#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include "newprojectwizard.h"
#include "mainwindow.h"
#include <QDomDocument>
#include <QFile>
#include <QByteArray>
#include <QPixmap>

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
}

#endif // HELPERS_H
