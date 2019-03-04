#include "elbehandler.h"

#include <QProcessEnvironment>
#include <QString>
#include <QDebug>

#include "helpers.h"

ElbeHandler::ElbeHandler()
{
	elbeProcess.setWorkingDirectory("/home/hico/elbe");
}


QString ElbeHandler::checkSystemForElbeVersion()
{



	return execCommand("./elbe").section("\n", 0, 0);
}

QString ElbeHandler::createProjectElbeInstance()
{
	return execCommand("./elbe control create_project").section("\n", 0, 0);
}

bool ElbeHandler::deleteProjectElbeInstance(QString projectPath)
{

	QString id = helpers::getProjectID(projectPath);
	//check if project exists
	if ( !getElbeProjects().contains(id) ) {
		qDebug() << "ERROR from "<<__func__<<" selected project does not exist in elbe";
		return false;
	}

	QString out = execCommand("./elbe control del_project "+id);

	if ( out.isEmpty() ) {
		qDebug() << "Deleted.";
		return true;
	} else {
		qDebug() << "ERROR from "<<__func__<<": "<<out;
		return false;
	}
	//del_project only returns output if an error occured

}

bool ElbeHandler::projectIsInElbe(QString projectPath)
{
	QStringList elbeProjects = getElbeProjects();
	return elbeProjects.contains(helpers::getProjectID(projectPath));
}

QStringList ElbeHandler::getElbeProjects()
{
	QStringList list;

	// returns a list of substrings, sectioned wherever the given seperator occurs
	QStringList tempList = execCommand("./elbe control list_projects").split("\n", QString::SkipEmptyParts);
	foreach (QString str, tempList) {
		list.append(str.section("\t", 0, 0));
//		qDebug() << str;
	}
	return list;
}

void ElbeHandler::startBuildProcess()
{
	XmlFileManager *filemanager = XmlFileManager::getInstance();
	ProjectManager *projectmanager = ProjectManager::getInstance();
	if ( !setXmlFile(filemanager->getCurrentFilePath(), projectmanager->getElbeID()) ) {
		qDebug() << "xml wasn't set -> don't commence build";
		return;
	}

	qDebug() << "now start build process!";
	if ( !execCommand("./elbe control build "+projectmanager->getElbeID()).isEmpty() ) {
		qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
		return;
	}
	qDebug() << "build finished";

}

bool ElbeHandler::setXmlFile(QString file, QString elbeID)
{
	qDebug() << "./elbe control set_xml "+elbeID+" "+file;
	QString out = execCommand("./elbe control set_xml "+elbeID+" "+file);

	qDebug() << __func__<<": "<<out;

	if ( out.compare("upload of xml finished\n") != 0 ) { //check if set_xml was successful
		qDebug() << "ERROR from "<<__func__;
		return false;
	} else {
		return true;
	}
}


QString ElbeHandler::execCommand(QString command)
{
	QByteArray outputByteArray;
	QString outputString;

	elbeProcess.start(command);
	if ( elbeProcess.waitForFinished() ) {
		outputByteArray = elbeProcess.readAllStandardOutput();
	} else {
		qDebug() << "timed out";
		qDebug() << QString().fromUtf8(elbeProcess.readAllStandardOutput());
		return outputString; //which is empty at that point
	}
	elbeProcess.close();

	outputString = QString().fromUtf8(outputByteArray);
	return outputString;
}
