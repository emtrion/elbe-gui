#include "elbehandler.h"

#include <QProcessEnvironment>
#include <QString>
#include <QDebug>
#include <QDir>

#include "helpers.h"

ElbeHandler::ElbeHandler()
{
	elbeProcess.setWorkingDirectory("/home/hico/elbe");
	projectmanager = ProjectManager::getInstance();
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
	if ( !setXmlFile(projectmanager->getBuildXmlPath(), projectmanager->getElbeID()) ) {
		qDebug() << "xml wasn't set. Abort";
		return;
	}

	qDebug() << "now starting build process!";
	if ( !execCommand("./elbe control build "+projectmanager->getElbeID()).isEmpty() ) {
		qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
		return;
	}
	qDebug() << "build started";

}


//remove files from out-directory because even if it failes, "elbe control ..." creates a file in the ouput directory
void ElbeHandler::removeFalseLoadedFiles(QString outPath, QString filename)
{
	QDir dir(outPath);

	dir.remove(filename);
}

bool ElbeHandler::getImages_p(QString buildXmlPath, QString outPath, QString elbeID)
{
	bool check;
	QStringList imageList = helpers::getImageFiles(buildXmlPath);
	foreach(QString filename, imageList) {

//		qDebug() << __func__<<": "<< filename;

		if( !getFile_p(filename, outPath, elbeID) ) {
			removeFalseLoadedFiles(outPath, filename);
			//adjust filename in case getFile failed
			if (!filename.endsWith(".gz") ) {
				filename.append(".gz");
			} else {
				filename.chop(3);
			}
			//try a second time with adjusted filename, in case the file was/wasn't zipped with gz
			if ( !getFile_p(filename, outPath, elbeID) ) {
				removeFalseLoadedFiles(outPath, filename);
				check = false;
			} else {
				check = true;
			}
//			qDebug() << "ERROR from "<<__func__<<": didnt't get image file "<<filename;
//			check = false;

			//remove false copied files
		} else {
			check = true; //set check=true because getFile succeeded
		}
	}
	return check;
}

bool ElbeHandler::getFile_p(QString filename, QString outPath, QString elbeID)
{

	QString out = execCommand("./elbe control --output "+outPath+" get_file "+elbeID+" "+filename);
//	qDebug() << __func__<<" says: "<<outPath+filename+" saved\n";
	if ( out.compare(outPath+filename+" saved\n" ) != 0 ) {
		qDebug() << "ERROR from "<<__func__<<" while downloading "<<filename;
		return false;
	} else {
//		qDebug() << "saved!";
		return true;
	}
}

bool ElbeHandler::getFiles_p(QStringList filenames, QString outPath, QString elbeID)
{
	bool check;
	foreach( QString str, filenames) {
		check = getFile_p(str, outPath, elbeID);
	}
	return check;
}

bool ElbeHandler::setXmlFile(QString file, QString elbeID)
{
//	qDebug() << "./elbe control set_xml "+elbeID+" "+file;
	QString out = execCommand("./elbe control set_xml "+elbeID+" "+file);

//	qDebug() << __func__<<": "<<out;

	if ( out.compare("upload of xml finished\n") != 0 ) { //check if set_xml was successful
		qDebug() << "ERROR from "<<__func__;
		return false;
	} else {
		return true;
	}
}

bool ElbeHandler::getFiles(QStringList filenames)
{
	return getFiles_p(filenames, projectmanager->getOutPath(), projectmanager->getElbeID());
}

bool ElbeHandler::getFile(QString filename)
{
	return getFile_p(filename, projectmanager->getOutPath(), projectmanager->getElbeID());
}

bool ElbeHandler::getImages()
{
	return getImages_p(projectmanager->getBuildXmlPath(), projectmanager->getOutPath(), projectmanager->getElbeID());
}

bool ElbeHandler::getFiles(QStringList filenames, QString outPath, QString elbeID)
{
	return getFiles_p(filenames, outPath, elbeID);
}

bool ElbeHandler::getFile(QString filename, QString outPath, QString elbeID)
{
	return getFile_p(filename, outPath, elbeID);
}

bool ElbeHandler::getImages(QString buildXmlPath, QString outPath, QString elbeID)
{
	return getImages_p(buildXmlPath, outPath, elbeID);
}


QString ElbeHandler::execCommand(QString command)
{
	elbeProcess.setProcessChannelMode(QProcess::MergedChannels);
	QByteArray outputByteArray;
	QString outputString;

	elbeProcess.start(command);
	if ( elbeProcess.waitForFinished() ) {
		outputByteArray = elbeProcess.readAll();
	} else {
		qDebug() << "timed out";
		qDebug() << QString().fromUtf8(elbeProcess.readAllStandardOutput());
		return outputString; //which is empty at that point
	}
	elbeProcess.close();

	outputString = QString().fromUtf8(outputByteArray);

//	qDebug() << __func__<<" got: "<<outputString;
	return outputString;
}
