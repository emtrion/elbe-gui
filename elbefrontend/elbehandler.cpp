#include "elbehandler.h"

#include <QProcessEnvironment>
#include <QString>
#include <QDebug>

ElbeHandler::ElbeHandler()
{

}


QString ElbeHandler::checkSystemForElbeVersion()
{
//		qDebug() << "insdide: "<<__func__;
	QByteArray output;
	QProcess findElbe;
	findElbe.setWorkingDirectory("/home/hico/elbe");
	findElbe.start("./elbe");
	if ( findElbe.waitForFinished() ) {
		output = findElbe.readAllStandardOutput();
	} else {
		qDebug() << "timed out!";
		qDebug() << QString().fromUtf8(findElbe.readAllStandardError());
		return "No elbe found!";
	}

	findElbe.close();
	QString outputString = QString().fromUtf8(output);
//		qDebug() << outputString.section("\n", 0, 0);
	return outputString.section("\n", 0, 0);
}

QString ElbeHandler::createProjectElbeInstance()
{
	QByteArray output;
	QProcess createElbeProject;
	createElbeProject.setWorkingDirectory("/home/hico/elbe");
	createElbeProject.start("./elbe control create_project");
	if ( createElbeProject.waitForFinished() ) {
	output = createElbeProject.readAllStandardOutput();
} else {
	qDebug() << "timed out";
	qDebug() << QString().fromUtf8(createElbeProject.readAllStandardError());
	return "Elbe error";
}

	createElbeProject.close();
	QString outputString = QString().fromUtf8(output);
	return outputString.section("\n", 0, 0);
}
