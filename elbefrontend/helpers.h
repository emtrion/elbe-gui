#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include "newprojectwizard.h"
#include "mainwindow.h"
#include <QDomDocument>
#include <QFile>
#include <QByteArray>

namespace helpers {
    QString getHomeDirectoryFromSystem();
	bool setProjectMetadata(QString projectName, QString projectPath);
//	void deleteFile(QString path);
	MainWindow* getMainWindow();
	QDomDocument parseXMLFile(QFile *file);
	bool saveXMLChanges(QFile *file, QByteArray byteArray);
}

#endif // HELPERS_H
