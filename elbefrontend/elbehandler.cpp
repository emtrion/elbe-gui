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
//		qDebug() << "insdide: "<<__func__;
	QByteArray output;
	elbeProcess.start("./elbe");
	if ( elbeProcess.waitForFinished() ) {
		output = elbeProcess.readAllStandardOutput();
	} else {
		qDebug() << "timed out!";
		qDebug() << QString().fromUtf8(elbeProcess.readAllStandardError());
		return "No elbe found!";
	}

	elbeProcess.close();
	QString outputString = QString().fromUtf8(output);
//		qDebug() << outputString.section("\n", 0, 0);
	return outputString.section("\n", 0, 0);
}

QString ElbeHandler::createProjectElbeInstance()
{
	QByteArray output;
	elbeProcess.start("./elbe control create_project");
	if ( elbeProcess.waitForFinished() ) {
		output = elbeProcess.readAllStandardOutput();
	} else {
		qDebug() << "timed out";
		qDebug() << QString().fromUtf8(elbeProcess.readAllStandardError());
		return "Elbe error";
	}

	elbeProcess.close();
	QString outputString = QString().fromUtf8(output);
	return outputString.section("\n", 0, 0);
}

bool ElbeHandler::deleteProjectElbeInstance(QString projectPath)
{

	QString id = helpers::getProjectID(projectPath);
	//check if project exists
	if ( !getElbeProjects().contains(id) ) {
		qDebug() << "ERROR from "<<__func__<<" selected project does not exist in elbe";
		return false;
	}
	QByteArray output;
	//delete
	elbeProcess.start("./elbe control del_project "+id);
	if ( elbeProcess.waitForFinished() ) {
		output = elbeProcess.readAllStandardOutput();
	} else {
		qDebug() << "timed out";
		qDebug() << QString().fromUtf8(elbeProcess.readAllStandardError());
		return false;
	}

	elbeProcess.close();

	if ( output.isEmpty() ) {
		qDebug() << "Deleted.";
		return true;
	} else {
		qDebug() << "An error occured: "<<output;
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
	QByteArray output;
	QStringList list;
	elbeProcess.start("./elbe control list_projects");
	if ( elbeProcess.waitForFinished() ) {
		output = elbeProcess.readAllStandardOutput();
	} else {
		qDebug() << "timed out";
		qDebug() << QString().fromUtf8(elbeProcess.readAllStandardError());
		return list; //which is empty at that point
	}



	elbeProcess.close();
	QString tempString = QString().fromUtf8(output);
//	qDebug() << tmp;
	QStringList tempList = tempString.split("\n", QString::SkipEmptyParts); // returns a list of substrings, sectioned wherever the given seperator occurs
	foreach (QString str, tempList) {
		list.append(str.section("\t", 0, 0));
//		qDebug() << str;
	}
	return list;
}

