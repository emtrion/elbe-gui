#include <iostream>
#include <QString>
#include <QProcessEnvironment>

#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QDir>
#include <QFile>
#include <QDebug>
#include "newprojectwizard.h"

#include "helpers.h"




namespace helpers {
	QString getHomeDirectoryFromSystem()
	{//get the homedirectory
        QProcessEnvironment proEnv = QProcessEnvironment::systemEnvironment();
        QString homeDir;
        homeDir = proEnv.value("HOME", NULL);

        return homeDir;
    }


	bool setProjectMetadata(QString projectName, QString projectPath, NewProjectWizard::projectSettings settings)
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

		QDomNode projectSettingParentNode;
		QDomNode childNode = root.firstChild();
		while (!childNode.isNull()) { //iterate over all child nodes
			QDomElement childElement = childNode.toElement();
			//look for nodes which have to be modified
			if ( childElement.tagName().compare("projectname") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectName));
			} else if ( childElement.tagName().compare("source_directory") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectPath+"/src/"));
			} else if ( childElement.tagName().compare("output_directory") == 0 ) {
				childNode.appendChild(doc.createTextNode(projectPath+"/out/"));
			} else if (childElement.tagName().compare("project") == 0) {
				projectSettingParentNode = childNode; //assign "project" to another node to access children
			}
			childNode = childNode.nextSibling();
		}

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


