#include "xmlfilehandler.h"

#include <QString>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include "helpers.h"
#include "projectmanager.h"

XmlFileHandler::XmlFileHandler()
{
	this->filemanager = XmlFileManager::getInstance();
}

XmlFileHandler::XmlFileHandler(QString path, QString name)
{
	if ( QDir(path).exists() ) {
		filePath = path+name+".xml";
	} else {
		qDebug() << "ERROR from "<<__func__<<" Path does not exist!";
	}

	this->fileName = name+".xml";
	this->filemanager = XmlFileManager::getInstance();

	qDebug() << fileName << filePath;
}

XmlFileHandler::XmlFileHandler(QString file)
{//alternative Constructor with complete file path as parameter
	if ( !file.isNull() ) {
		if ( QFile(file).exists() ) {
			this->filePath = file;
		} else {
			qDebug() << "ERROR from "<<__func__<<" Path does not exist!";
		}
	}

	this->fileName = file.section("/", -1);
	this->filemanager = XmlFileManager::getInstance();
}

XmlFileHandler::~XmlFileHandler()
{

}

void XmlFileHandler::createFile()
{

	QFile file(filePath);

	if ( !file.exists() ) {
		if ( !file.open(QIODevice::ReadWrite) ) {
			qDebug() << "ERROR from "<<__func__<<" Could not create file";
		}
	} else {
		qDebug() << "ERROR from "<<__func__<<" File does already exist!";
	}

	file.close();

	XMLautoGenerate();
	openFile();
}

void XmlFileHandler::openFile()
{
	QFile file(filePath);
	QString content;
	if ( file.exists() ) {
		if ( !file.open(QIODevice::ReadWrite) ) {
			qDebug() << "ERROR from "<<__func__<<" Could not open file";
			return;
		}
	} else {
		qDebug() << "ERROR from "<<__func__<<" File does not exist";
		return;
	}

	content = QString::fromUtf8(file.readAll());
	file.close();

	MainWindow *mw = helpers::getMainWindow();
	mw->getEditor()->setPlainText(content);
	mw->getEditor()->setEnabled(true);
	mw->getEditor()->setLineNumberAreaVisible(true);
	mw->setEditorTabVisible(true);
	mw->setOpenFileNameLabelText(fileName);

	filemanager->setCurrentFilePath(filePath);

	return;
}

void XmlFileHandler::XMLautoGenerate()
{
	ProjectManager *projectmanager = ProjectManager::getInstance();
	ProjectManager::projectSettings set = projectmanager->getNewProjectSettings();

	QFile templateFile(":/autogeneratedXML.xml");
	QDomDocument doc;
	doc = helpers::parseXMLFile(&templateFile);

	QDomElement root = doc.firstChildElement("ns0:RootFileSystem");
//	qDebug() << root.tagName();

	QDomNode projectNode;
	QDomNode targetNode;
	QDomNode rootChildNode = root.firstChild();
	while (!rootChildNode.isNull()) {
		QDomElement childElement = rootChildNode.toElement();
//		qDebug() << childElement.tagName();
		if ( childElement.tagName().compare("project") == 0 ) {
			projectNode = rootChildNode;
		} else if (childElement.tagName().compare("target") == 0) {
			targetNode = rootChildNode;
		}
		rootChildNode = rootChildNode.nextSibling();
	}

//	qDebug() << set.name;
//	qDebug() << set.version;
//	qDebug() << set.description;
//	qDebug() << set.buildtype;
//	qDebug() << set.suite;
//	qDebug() << set.host;
//	qDebug() << set.path;
//	qDebug() << set.proto;

	QDomNode mirrorNode;
	QDomNode projectChildNode = projectNode.firstChild();
	while( !projectChildNode.isNull() ) {
		QDomElement projectChildElement = projectChildNode.toElement();
//		qDebug() << projectChildElement.tagName();
		if (projectChildElement.tagName().compare("name") == 0) {
			projectChildNode.appendChild(doc.createTextNode(set.name));
		} else if(projectChildElement.tagName().compare("version") == 0){
			projectChildNode.appendChild(doc.createTextNode(set.version));
		} else if(projectChildElement.tagName().compare("description") == 0){
			projectChildNode.appendChild(doc.createTextNode(set.description));
		} else if(projectChildElement.tagName().compare("buildtype") == 0){
			projectChildNode.appendChild(doc.createTextNode(set.buildtype));
		} else if(projectChildElement.tagName().compare("suite") == 0){
			projectChildNode.appendChild(doc.createTextNode(set.suite));
		} else if(projectChildElement.tagName().compare("mirror") == 0){
			mirrorNode = projectChildNode;
		}

		projectChildNode = projectChildNode.nextSibling();
	}

	QDomNode mirrorChildNode = mirrorNode.firstChild();
	while ( !mirrorChildNode.isNull() ) {
		QDomElement mirrorChildElement = mirrorChildNode.toElement();
//		qDebug() << mirrorChildElement.tagName();
		if(mirrorChildElement.tagName().compare("primary_host") == 0){
			mirrorChildNode.appendChild(doc.createTextNode(set.host));
		} else if(mirrorChildElement.tagName().compare("primary_path") == 0 ){
			mirrorChildNode.appendChild(doc.createTextNode(set.path));
		} else if(mirrorChildElement.tagName().compare("primary_proto") == 0){
			mirrorChildNode.appendChild(doc.createTextNode(set.proto));
		}

		mirrorChildNode = mirrorChildNode.nextSibling();
	}

	/* Append empty values to nodes to avoid them being self-closing*/
	QDomNode targetChildNode = targetNode.firstChild();
	while (!targetChildNode.isNull()) {
		targetChildNode.appendChild(doc.createTextNode(""));
		targetChildNode = targetChildNode.nextSibling();
	}


	QByteArray xml = doc.toByteArray(4);

	QFile file(filePath);
	helpers::saveXMLChanges(&file, xml);
}


void XmlFileHandler::saveFile()
{
	MainWindow *mw = helpers::getMainWindow();

	QByteArray content = mw->getEditor()->toPlainText().toUtf8();
	QFile file(filemanager->getCurrentFilePath());

	QFileInfo info(file);
	qDebug() << info.filePath();

	if ( file.open(QIODevice::ReadWrite | QIODevice::Truncate) ) {
		if ( file.write(content) < 0 ) {//check if write was successful
			qDebug() << "ERROR from "<<__func__<<" Cannot write to file";
		}
	} else {
		qDebug() << "ERROR from "<<__func__<<" Cannot open file";
		return;
	}
	file.close();

	filemanager->setIsSaved(true);
}




void XmlFileHandler::closeFile()
{
	//if saved:
	MainWindow *mw = helpers::getMainWindow();
	mw->getEditor()->clear();
	mw->getEditor()->setEnabled(false);
	mw->getEditor()->setLineNumberAreaVisible(false);
	mw->setEditorTabVisible(false);
	//if not show dialog to save
	return;
}

