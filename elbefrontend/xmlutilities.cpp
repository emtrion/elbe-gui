#include "xmlutilities.h"

#include <QDebug>
#include <QDomDocument>
#include <QFile>

#include "project.h"
#include "helpers.h"

namespace xmlUtilities {

	QDomDocument parseXMLFile(QFile *file)
	{
		QDomDocument doc;
		if (!file->open(QIODevice::ReadOnly)) {
			qDebug() << "ERROR from "<<__func__<<" Cannot open file";
			return QDomDocument();
		}
		if (!doc.setContent(file)) {
			qDebug() << "ERROR from "<<__func__<<" Cannot parse the content";
			file->close();
			return QDomDocument();
		}
		file->close();

		return doc;
	}

	bool setProjectMetadata(QString projectName, QString projectPath)
	{
		Project *projectmanager = Project::getInstance();
		QString elbeId = projectmanager->elbeID();
		/*----------------------------- create DOM from XML --------------------------*/

		QFile file(projectPath+"/.project");
		QDomDocument doc = parseXMLFile(&file);

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

		ProjectProperties *properties = projectmanager->newProjectProperties();

		QDomNode mirrorSettingParentNode;
		QDomNode projectSettingNode = projectSettingParentNode.firstChild();

		while ( !projectSettingNode.isNull() ) {
			QDomElement projectSettingElement = projectSettingNode.toElement();
			if ( projectSettingElement.tagName().compare("name") == 0 ) {
				projectSettingNode.appendChild(doc.createTextNode(properties->name()));
			} else if ( projectSettingElement.tagName().compare("version") == 0 ) {
				projectSettingNode.appendChild(doc.createTextNode(properties->version()));
			} else if ( projectSettingElement.tagName().compare("description") == 0 ) {
				projectSettingNode.appendChild(doc.createTextNode(properties->description()));
			} else if ( projectSettingElement.tagName().compare("buildtype") == 0 ) {
				projectSettingNode.appendChild(doc.createTextNode(properties->buildtype()));
			} else if ( projectSettingElement.tagName().compare("suite") == 0 ) {
				projectSettingNode.appendChild(doc.createTextNode(properties->suite()));
			} else if ( projectSettingElement.tagName().compare("mirror") == 0 ) {
				mirrorSettingParentNode = projectSettingNode; //assign "mirror" to a node to access children
			}
			projectSettingNode = projectSettingNode.nextSibling();
		}

		QDomNode mirrorSettingNode = mirrorSettingParentNode.firstChild();
		while ( !mirrorSettingNode.isNull() ) {
			QDomElement mirrorSettingElement = mirrorSettingNode.toElement();
			if ( mirrorSettingElement.tagName().compare("primary_host") == 0 ) {
				mirrorSettingNode.appendChild(doc.createTextNode(properties->host()));
			} else if ( mirrorSettingElement.tagName().compare("primary_path") == 0 ) {
				mirrorSettingNode.appendChild(doc.createTextNode(properties->path()));
			} else if ( mirrorSettingElement.tagName().compare("primary_proto") == 0 ) {
				mirrorSettingNode.appendChild(doc.createTextNode(properties->proto()));
			}
			mirrorSettingNode = mirrorSettingNode.nextSibling();
		}

		//set indent to 4
		QByteArray xml = doc.toByteArray(4);
		return saveXMLChanges(&file, xml);
	}

	bool saveXMLChanges(QFile *file, QByteArray byteArray)
	{
		QFileDevice::Permissions permissions = file->permissions();
		file->setPermissions(QFileDevice::WriteUser | permissions);
		if ( !file->open(QIODevice::Truncate | QIODevice::WriteOnly) ) {
			qDebug() << "ERROR from "<<__func__<<" Cannot open file: "<<file->fileName();
			return false;
		}

		if ( file->write(byteArray) < 0 ) {
			qDebug() << "ERROR from "<<__func__<<" Cannot write to file";
			return false;
		}
		file->close();
		return true;
	}

	QString parseProjectDescriptionForNodeValue(QString confFile, QString node)
	{
		QFile file(confFile);
		QString retVal; //empty string

		QDomDocument doc = parseXMLFile(&file);
		QDomElement root = doc.firstChildElement("projectDescription");
		QDomNode childNode = root.firstChild();
		while ( !childNode.isNull() ) {
			QDomElement childElement = childNode.toElement();
			 if ( childElement.tagName().compare(node) == 0 ) {
				retVal = childNode.firstChild().nodeValue();
				break;
			 }
			 childNode = childNode.nextSibling();
		}
		file.close();

		return retVal;
	}

	QString getProjectID(QString confFile)
	{
		QString retVal = parseProjectDescriptionForNodeValue(confFile, "elbe_id");
		return retVal;
	}

	QString getProjectName(QString confFile)
	{
		QString retVal = parseProjectDescriptionForNodeValue(confFile, "projectname");
		return retVal;
	}

	QStringList getImageFiles(QString buildXmlPath)
	{
		QFile file(buildXmlPath);

		QStringList imageFilenames;
		QDomNode target;

		QDomDocument doc = parseXMLFile(&file);
		QDomElement root = doc.firstChildElement("ns0:RootFileSystem");
		QDomNode childNode = root.firstChild();
		while ( !childNode.isNull() ) {
			if ( childNode.toElement().tagName().compare("target") == 0 ) {
				target = childNode;
			}
			childNode = childNode.nextSibling();
		}

		QDomNode package;
		QDomNode images;

		QDomNode targetChild = target.firstChild();
		while( !targetChild.isNull() ) {
			if ( targetChild.toElement().tagName().compare("package") == 0 ) {
				package = targetChild;
			} else if ( targetChild.toElement().tagName().compare("images") == 0 ) {
				images = targetChild;
			}
			targetChild = targetChild.nextSibling();
		}

		//put the nodes in a list so we can iterate over them later
		QList<QDomNode> nodeList;
		nodeList.append(package);
		nodeList.append(images);

		QDomNode tmpNode;
		QDomNode tmpNode2;
		//iterate over the two nodes packages and images
		//because they both have the same structure we can use the same algorithm
		foreach (QDomNode node, nodeList) {
			QDomNode nodeChild = node.firstChild();
			while ( !nodeChild.isNull() ) {
				tmpNode = nodeChild;
				while ( !tmpNode.isNull() ) {
					tmpNode2 = tmpNode.firstChild();
					while ( !tmpNode2.isNull() ) {
						if ( tmpNode2.toElement().tagName().compare("name") == 0 ) {
							//append every name tag found to the list which is returned
							imageFilenames.append(tmpNode2.firstChild().nodeValue());
							break;
						}
						tmpNode2 = tmpNode2.nextSibling();
					}
					tmpNode = tmpNode.nextSibling();
				}
				nodeChild = nodeChild.nextSibling();
			}
		}
		return imageFilenames;
	}
}
