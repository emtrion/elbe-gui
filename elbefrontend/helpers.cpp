#include <iostream>
#include <QString>
#include <QProcessEnvironment>

#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "helpers.h"




namespace helpers {
	QString getHomeDirectoryFromSystem()
	{//get the homedirectory
        QProcessEnvironment proEnv = QProcessEnvironment::systemEnvironment();
        QString homeDir;
        homeDir = proEnv.value("HOME", NULL);

        return homeDir;
    }


	bool setProjectMetadata(QString projectName, QString projectPath)
	{//adds metadata to the config file

		/*----------------------------- create DOM from XML --------------------------*/

		QDomDocument doc("configFile");
		QFile file(projectPath+"/.project");
		if (!file.open(QIODevice::ReadOnly)) {// Open file
			qDebug() << "Cannot open file";
			return false;
		}

		if (!doc.setContent(&file)) {// Parse file
			qDebug() << "Cannot parse the content";
			file.close();
			return false;
		}
		file.close();

		/*------------------------------- Modify DOM ---------------------------------*/

		QDomElement root = doc.firstChildElement("projectDescription"); //get root element


//		qDebug() << "root node: "+root.toElement().tagName();

		QDomNodeList children = root.childNodes(); //get list of all children
		if (children.size() < 1) {
			qDebug() << "Cannot find children";
			return false;
		}

		QDomNode childNode = root.firstChild();
		while (!childNode.isNull()) { //iterate over all child elements
			QDomElement childElement = childNode.toElement();
			//look for nodes which have to be modified
			if ( childElement.tagName().compare("name") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectName));
			} else if ( childElement.tagName().compare("source_directory") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectPath+"/src/"));
			} else if ( childElement.tagName().compare("output_directory") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectPath+"/out/"));
			} else {}
			childNode = childNode.nextSibling(); //go to next child element
		}

		/*-------------------------------- Save changes ------------------------------------*/

		QFileDevice::Permissions p = file.permissions(); //get permissions
		file.setPermissions(QFileDevice::WriteUser | p); //add permission to write
		if ( !file.open(QIODevice::Truncate | QIODevice::WriteOnly) ) {
			qDebug() << "Cannot open file";
			return false;
		}
		QByteArray xml = doc.toByteArray(); /*converts the DOM to its textual representation.
												Returns a QByteArray containing UTF-8-encoded data.
												QByteArray always contains a zero-terminated string*/
		if ( file.write(xml) < 0 ) {//check if write was successful
			qDebug() << "Cannot write to file";
			return false;
		}
		file.close();

		return true;
	}


//	void deleteFile(QString path){}
}


