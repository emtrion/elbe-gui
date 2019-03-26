#include "elbehandler.h"

#include <QProcessEnvironment>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QThread>

#include "helpers.h"
#include "projectmanager.h"
#include "buildmanager.h"

ElbeHandler::ElbeHandler()
{

}


//namespace {
	bool ElbeHandler::getImages_p(QString buildXmlPath, QString outPath, QString elbeID)
	{
		//qDebug() << __func__<<" is in: "<<QThread::currentThreadId();
		bool check;
		QStringList imageList = helpers::getImageFiles(buildXmlPath);
		foreach(QString filename, imageList) {

	//		qDebug() << __func__<<": "<< filename;

			if (!filename.endsWith(".gz") ) {
				filename.append(".gz");
			}

			if( !getFile_p(filename, outPath, elbeID) ) {
				removeFalseLoadedFiles(outPath, filename);
				//adjust filename in case getFile failed
				filename.chop(3);
				//try a second time with adjusted filename, in case the file wasn't zipped with gz
				if ( !getFile_p(filename, outPath, elbeID) ) {
					removeFalseLoadedFiles(outPath, filename);
					check = false;
				} else {
					check = true;
				}
	//			qDebug() << "ERROR from "<<__func__<<": didnt't get image file "<<filename;
	//			check = false;

			} else {
				check = true; //set check=true because getFile succeeded
			}
		}
		return check;
	}

	bool ElbeHandler::getFile_p(QString filename, QString outPath, QString elbeID)
	{
		auto buildmanager = BuildManager::getInstance();
		QString commandPrefix = buildmanager->getElbeCommandPrefix();

	//	qDebug() << __func__<<" loading: "<<filename<<" in Thread: "<<QThread::currentThreadId();

		QString out = execCommand(commandPrefix+"elbe control --output "+outPath+" get_file "+elbeID+" "+filename, -1/*causes the waitForFinished to wait without any timeout*/);
		qDebug() << __func__<<" says: "<<outPath+filename+" saved\n";
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
			qDebug() << "filename: "<<str<<" in "<<__func__;
			check = getFile_p(str, outPath, elbeID);
		}
		return check;
	}
//}

bool ElbeHandler::isVersionSupported()
{
	qDebug() << checkElbeVersion();
	return true;
}

QString ElbeHandler::checkElbeVersion()
{
	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();
	//returns the first section of the string which is always the version
	return execCommand(commandPrefix+"elbe").section("\n", 0, 0);
}

QString ElbeHandler::createProjectElbeInstance()
{
	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();
	return execCommand(commandPrefix+"elbe control create_project").section("\n", 0, 0);
}

bool ElbeHandler::deleteProjectElbeInstance(QString projectPath)
{
	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();

	QString id = helpers::getProjectID(projectPath);
	//check if project exists
	if ( !makeProjectList().contains(id) ) {
		qDebug() << "ERROR from "<<__func__<<" selected project does not exist in elbe";
		return false;
	}

	QString out = execCommand(commandPrefix+"elbe control del_project "+id);

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
	QStringList elbeProjects = makeProjectList();
	return elbeProjects.contains(helpers::getProjectID(projectPath));
}

QStringList ElbeHandler::makeProjectList()
{
	QStringList list;

	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();

	// returns a list of substrings, sectioned wherever the given seperator occurs
	QStringList tempList = execCommand(commandPrefix+"elbe control list_projects").split("\n", QString::SkipEmptyParts);
	foreach (QString str, tempList) {
		list.append(str.section("\t", 0, 0));
//		qDebug() << str;
	}
	return list;
}

bool ElbeHandler::startBuildProcess(bool sourceOptionChecked, bool binOptionChecked)
{
	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();

	ProjectManager *projectmanager = ProjectManager::getInstance();

	if ( !setXmlFile(projectmanager->getBuildXmlPath(), projectmanager->getElbeID()) ) {
		qDebug() << "xml wasn't set. Abort";
		return false;
	}

	qDebug() << "now starting build process!";
	if ( sourceOptionChecked && binOptionChecked ) {
		if ( !execCommand(commandPrefix+"elbe control --build-bin --build-sources build "+projectmanager->getElbeID()).isEmpty() ) {
			qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
			return false;
		}
	} else if ( sourceOptionChecked && !binOptionChecked ) {
		if ( !execCommand(commandPrefix+"elbe control --build-sources build "+projectmanager->getElbeID()).isEmpty() ) {
			qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
			return false;
		}
	} else if ( !sourceOptionChecked && binOptionChecked ) {
		if ( !execCommand(commandPrefix+"elbe control --build-bin build "+projectmanager->getElbeID()).isEmpty() ) {
			qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
			return false;
		}
	} else {
		if ( !execCommand(commandPrefix+"elbe control build "+projectmanager->getElbeID()).isEmpty() ) {
			qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
			return false;
		}
	}

	qDebug() << "build started";
	return true;
}


//remove files from out-directory because even if it failes, "elbe control ..." creates a file in the ouput directory
void ElbeHandler::removeFalseLoadedFiles(QString outPath, QString filename)
{
	QDir dir(outPath);

	dir.remove(filename);
}



bool ElbeHandler::setXmlFile(QString file, QString elbeID)
{
	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();

//	qDebug() << "./elbe control set_xml "+elbeID+" "+file;
	QString out = execCommand(commandPrefix+"elbe control set_xml "+elbeID+" "+file);

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
	ProjectManager *projectmanager = ProjectManager::getInstance();
	return getFiles_p(filenames, projectmanager->getOutPath(), projectmanager->getElbeID());
}

bool ElbeHandler::getFile(QString filename)
{
	ProjectManager *projectmanager = ProjectManager::getInstance();
	return getFile_p(filename, projectmanager->getOutPath(), projectmanager->getElbeID());
}

bool ElbeHandler::getImages()
{
	ProjectManager *projectmanager = ProjectManager::getInstance();
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


bool ElbeHandler::checkIfBusy(QString id)
{
	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();

	QString out;
	QStringList projects;
	QStringList details;
	out = execCommand(commandPrefix+"elbe control list_projects");

	projects = out.split("\n", QString::SkipEmptyParts);
//	qDebug() << "projects: ";
	foreach (QString str, projects) {
		if ( str.indexOf(id) != -1 ) {
			details = str.split("\t", QString::SkipEmptyParts);
			break;
		}
	}

//	qDebug() << "details: ";
	foreach (QString str, details) {
//		qDebug() << str;
		if ( str.compare("busy") == 0 ) {
			return true;
		}
	}
	return false;
}

bool ElbeHandler::restartInitVM(const QString &initVM)
{
	QString out;

	auto buildmanager = BuildManager::getInstance();
	QString commandPrefix = buildmanager->getElbeCommandPrefix();

	execCommand(commandPrefix+"elbe initvm stop");
	out = execCommand(commandPrefix+"elbe initvm --directory "+initVM+" start");

	if ( !out.compare("*****") ) {
		qDebug() << "an error occured while trying to start "<<initVM;
		return false;
	} else {
		return true;
	}

}


QString ElbeHandler::execCommand(QString command, int timeout)
{
	auto buildmanager = BuildManager::getInstance();
	QProcess p;
	p.setWorkingDirectory(buildmanager->getElbeWorkingDir());

	//all output is directed to stdOut
	p.setProcessChannelMode(QProcess::MergedChannels);
	QByteArray outputByteArray;
	QString outputString;

//	qDebug() << __func__<<" is in: "<<QThread::currentThreadId();

	qDebug() << "Exec: "<<command;

	p.start(command);
	if ( p.waitForFinished(timeout) ) {
		outputByteArray = p.readAll();
	} else {
		qDebug() << "timed out";
		qDebug() << QString().fromUtf8(p.readAllStandardOutput());
		return outputString; //which is empty at that point
	}
	p.close();

	outputString = QString().fromUtf8(outputByteArray);

	qDebug() << __func__<<" got: "<<outputString;
	return outputString;
}
