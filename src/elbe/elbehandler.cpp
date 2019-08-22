#include "elbehandler.h"

#include <QProcessEnvironment>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QThread>

#include "src/app/helpers.h"
#include "src/projects/project.h"
#include "buildmanager.h"
#include "src/xml/xmlutilities.h"
#include "src/mainwindow/mainwindow.h"
#include  "buildprocessworker.h"

#include <src/dialogs/elbesettingsdialog.h>


namespace ElbeHandler {
	namespace { //unnamed namespace

		//remove files from out-directory because even if it failes, "elbe control get_file ..." creates an empty file in the ouput directory
		void removeFalseLoadedFiles(QString outPath, QString filename)
		{
			QDir dir(outPath);
			dir.remove(filename);
		}

		void forwardOutputToMessageLog(QString output)
		{
			MainWindow *mainwindow = helpers::getMainWindow();
			QString msgLogText;
			QStringList lines, words;
			lines = output.split("\n", QString::SkipEmptyParts);
			foreach (QString str, lines) {
				words = str.split("\t", QString::SkipEmptyParts);
				msgLogText = words.join(" ");
				mainwindow->messageLogAppendText(msgLogText, "#F36363");
			}
		}

		//executes the given command as a QProcess
		QString execCommand(QString command, int timeout = 30000, bool forceWithoutPrefix = false)
		{
			BuildManager *buildmanager = BuildManager::getInstance();
			QProcess process;
			process.setWorkingDirectory(buildmanager->elbeWorkingDir());

			//all output is directed to stdOut
			process.setProcessChannelMode(QProcess::MergedChannels);
			QByteArray outputByteArray;
			QString outputString;

			QString commandPrefix;
			if ( !forceWithoutPrefix ) {
				commandPrefix = buildmanager->elbeCommandPrefix();
			}

			qDebug() << commandPrefix+command;
			process.start(commandPrefix+command);
			if ( process.waitForFinished(timeout) ) {
				outputByteArray = process.readAll();
			} else {
				forwardOutputToMessageLog(QString().fromUtf8(process.readAll()));
				return outputString; //which is empty at that point
			}
			process.close();
			outputString = QString().fromUtf8(outputByteArray);
			if ( outputString.startsWith("Failed") ) {
				forwardOutputToMessageLog(outputString);
			}
			return outputString;
		}

		QStringList makeProjectList()
		{
			QStringList list;

			// returns a list of substrings, sectioned wherever the given seperator occurs
			QStringList tempList = execCommand("elbe control list_projects").split("\n", QString::SkipEmptyParts);
			foreach (QString str, tempList) {
				list.append(str.section("\t", 0, 0));
			}
			return list;
		}

		//loads a file specified by filename from initvm
		bool getFile_p(QString filename, QString outPath, QString elbeID)
		{


			qDebug() << outPath;
			QString out = execCommand("elbe control --output "+outPath+" get_file "+elbeID+" "+filename, -1/*causes the waitForFinished to wait without timeout*/);
			qDebug() << out;
			if ( out.compare(outPath+filename+" saved\n" ) != 0 ) {
				qDebug() << "ERROR from "<<__func__<<" while downloading "<<filename;
				return false;
			} else {
				return true;
			}
		}

		//loads all images from initvm
		bool getImages_p(QString buildXmlPath, QString outPath, QString elbeID)
		{
			bool succeeded;
			//retieve the image filenames from the build xml
			QStringList imageList = xmlUtilities::getImageFiles(buildXmlPath);
			foreach(QString filename, imageList) {
				//in most cases elbe makes .gz files, irrespective whether it was originally defined or not
				if (!filename.endsWith(".gz") ) {
					filename.append(".gz");
				}
				//when getFile fails, the created empty file has to be removed from output directory
				if( !getFile_p(filename, outPath, elbeID) ) {
					removeFalseLoadedFiles(outPath, filename);
					//when it fails it is normally because of the fileextension. Chop 3 character (".gz") and try again
					filename.chop(3);
					//try a second time with adjusted filename, in case the file wasn't zipped with gz
					if ( !getFile_p(filename, outPath, elbeID) ) {
						removeFalseLoadedFiles(outPath, filename);
						//giving up after second try. There must be a different problem
						succeeded = false;
					} else {
						succeeded = true;
					}
				} else {
					succeeded = true;
				}
			}
			return succeeded;
		}

		//loads more than one file from initvm
		bool getFiles_p(QStringList filenames, QString outPath, QString elbeID)
		{
			bool check;
			//iterate over filenames and load them one by one
			foreach( QString str, filenames) {
				check = getFile_p(str, outPath, elbeID);
			}
			return check;
		}



} //END unnamed namespace

	//checks if the detected elbe version is higher than 2.4 or lower than 2.0
	void isVersionSupported()
	{
		QString version = checkElbeVersion();

		if ( version.isEmpty() ) {
			helpers::showMessageBox("Information", "It seems there is no elbe installed on the system.\n"
												   "Please set an elbe executable",
									QMessageBox::StandardButton(QMessageBox::Ok),
									QMessageBox::Ok
									);
			return;
		}


//		first: cut the "elbe v"-part
		version = version.split("v", QString::SkipEmptyParts).last();
//		second: split the number
		QStringList numbers = version.split(".", QString::SkipEmptyParts);

		bool informationIsNeeded = false;
		QString informativeText;

		if ( numbers.isEmpty() ) {
			//should not happen
			qDebug() << "ERROR in"<<__func__<<": didn't find version";
			return;
		}

		if ( QString(numbers.first()).toInt() < 2 ) {
			informativeText =  "Version below v2 might work but is neither supported nor recommended";
			informationIsNeeded = true;

		} else if ( QString(numbers.first()).toInt() > 2 ) {
			informativeText = "Version v3 or higher might work but is not yet supported";
			informationIsNeeded = true;
		} else {
			if ( QString(numbers.at(1)).toInt() > 4 ) {
				informativeText = "Version higher v2.4 might work but is not supported";
				informationIsNeeded = true;
			}
		}

		if ( informationIsNeeded ) {
			helpers::showMessageBox("Elbeversion not supported", informativeText,
								QMessageBox::StandardButtons(QMessageBox::Ok),
								QMessageBox::Ok);
		}
	}

	//returns elbe version as string like this "elbe vX.Y"
	QString checkElbeVersion()
	{
		//returns the first section of the string which is always the version
		return execCommand("elbe").section("\n", 0, 0);
	}

	//creates a project in initvm
	//returns elbeID
	QString createProjectElbeInstance()
	{
		return execCommand("elbe control create_project").section("\n", 0, 0);
	}

	//deletes project in initvm
	bool deleteProjectElbeInstance(QString projectPath)
	{
		QString id = xmlUtilities::getProjectID(projectPath);
		//check if project exists
		if ( !makeProjectList().contains(id) ) {
			qDebug() << "ERROR from "<<__func__<<" selected project does not exist in elbe";
			return false;
		}

		QString out = execCommand("elbe control del_project "+id);

		if ( out.isEmpty() ) {
			return true;
		} else {
			qDebug() << "ERROR from "<<__func__<<": "<<out;
			return false;
		}
		//only returns output if an error occured
	}

	//checks if the project projectPath exists in initvm
	bool projectIsInElbe(QString projectPath)
	{
		QStringList elbeProjects = makeProjectList();
		return elbeProjects.contains(xmlUtilities::getProjectID(projectPath));
	}


	bool projectIsBuilding(QString projectPath)
	{
		BuildManager *buildmanager = BuildManager::getInstance();
		if ( buildmanager->isBuildRunning() ) {
			return projectPath.compare(buildmanager->processWorkerPointer()->buildingProjectPath()) == 0;
		}
		return false;
	}

	bool startBuildProcess(bool sourceOptionChecked, bool binOptionChecked)
	{

		Project *projectmanager = Project::getInstance();

		if ( !setXmlFile(projectmanager->buildXmlPath(), projectmanager->elbeID()) ) {
			qDebug() << "ERROR from"<<__func__<<": xml wasn't set. Abort!";
			return false;
		}

		//adding optional paramters if selected
		if ( sourceOptionChecked && binOptionChecked ) {
			if ( !execCommand("elbe control --build-bin --build-sources build "+projectmanager->elbeID()).isEmpty() ) {
				qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
				return false;
			}
		} else if ( sourceOptionChecked && !binOptionChecked ) {
			if ( !execCommand("elbe control --build-sources build "+projectmanager->elbeID()).isEmpty() ) {
				qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
				return false;
			}
		} else if ( !sourceOptionChecked && binOptionChecked ) {
			if ( !execCommand("elbe control --build-bin build "+projectmanager->elbeID()).isEmpty() ) {
				qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
				return false;
			}
		} else {
			if ( !execCommand("elbe control build "+projectmanager->elbeID()).isEmpty() ) {
				qDebug() << "ERROR from "<<__func__<<". "<<"Build failed.";
				return false;
			}
		}
		return true;
	}


	bool setXmlFile(QString file, QString elbeID)
	{

		QString out = execCommand("elbe control set_xml "+elbeID+" "+file);
		if ( out.compare("upload of xml finished\n") != 0 ) {
			qDebug() << "ERROR from "<<__func__;
			return false;
		} else {
			return true;
		}
	}

	bool getFiles(QStringList filenames)
	{
		Project *projectmanager = Project::getInstance();
		return getFiles_p(filenames, projectmanager->outPath(), projectmanager->elbeID());
	}

	bool getFile(QString filename)
	{
		Project *projectmanager = Project::getInstance();
		return getFile_p(filename, projectmanager->outPath(), projectmanager->elbeID());
	}

	bool getImages()
	{
		Project *projectmanager = Project::getInstance();
		return getImages_p(projectmanager->buildXmlPath(), projectmanager->outPath(), projectmanager->elbeID());
	}

	bool getFiles(QStringList filenames, QString outPath, QString elbeID)
	{
		return getFiles_p(filenames, outPath, elbeID);
	}

	bool getFile(QString filename, QString outPath, QString elbeID)
	{
		return getFile_p(filename, outPath, elbeID);
	}

	bool getImages(QString buildXmlPath, QString outPath, QString elbeID)
	{
		return getImages_p(buildXmlPath, outPath, elbeID);
	}


	//checks in list_projects if status is busy
	bool checkIfBusy(QString id)
	{
		QString out;
		QStringList projects;
		QStringList details;
		out = execCommand("elbe control list_projects");

		//split the lines to get each project alone
		projects = out.split("\n", QString::SkipEmptyParts);
		foreach (QString str, projects) {
			//indexOf returns -1 if it doesn't find the given string
			if ( str.indexOf(id) != -1 ) {
				//str is now the project specified by id
				//the different fields are seperated with \t
				details = str.split("\t", QString::SkipEmptyParts);
				break;
			}
		}
		foreach (QString str, details) {
			if ( str.compare("busy") == 0 ) {
				//if we find "busy" in the line we know the project is busy right now
				return true;
			}
		}
		return false;
	}

	//stops the current and starts the new initVM
	bool restartInitVM(const QString &initVM)
	{
		QString out;

		execCommand("elbe initvm stop");
		out = execCommand("elbe initvm --directory "+initVM+" start");

		//initvm start prints "*****" on success
		if ( !out.compare("*****") ) {
			qDebug() << "ERROR from "<<__func__<<": could not start "<<initVM;
			return false;
		} else {
			return true;
		}
	}

	QStringList buildUpdate()
	{ //called from updateThread. All actions invoked here are outside the main thread


		QString referenceProject = buildReferenceProject();

		//take the updateScript from ressources, adapt parameters to system and project, put the script on system for execution
		//returns updateScriptFile
		QFile file(prepareUpdateScript(referenceProject));

		QString out;

		out = execCommand(helpers::getHomeDirectoryFromSystem()+"/.elbefrontend/updateScript.exp", -1, true);
//		QThread::sleep(5);

		qDebug() << out;

		if ( out.contains("error: operation failed: Active console session exists for this domain") ) {

			 return QStringList() << "Update failed. Most likely there is an open session with the initvm." ;
		}

		//remove the file, only used temporarily
		file.remove();

		saveUpdateFiles(referenceProject);
		execCommand("elbe control del_project "+referenceProject);

		return QStringList();
	}

	QString checkForUpdates()
	{
		QString out;

		Project *project = Project::getInstance();
		QList<UpdateListItem> list = project->getUpdates()->getUpdateList();

		if ( list.size() <= 1 ) {
			out = execCommand("elbe check_updates "+sourceXMLPath(), -1);
		} else {

			QString updateNumber = QString().number(list.size());
			QString updateXMLPath = project->projectDirectory()+"/updates/build"+updateNumber+"/source.xml";
			out = execCommand("elbe check_updates "+updateXMLPath, -1);
		}

		QString str = out.section("\n", -2);


		qDebug() << str;

		return str;
	}

	QString prepareUpdateScript(QString referenceProject)
	{
		Project *projectmanager = Project::getInstance();
		QFile file(helpers::getHomeDirectoryFromSystem()+"/.elbefrontend/updateScript.exp");

		QString script = readUpdateScript();

		script.replace("OLDPROJECTDIR", projectmanager->elbeID(), Qt::CaseInsensitive);
		script.replace("NEWPROJECTDIR", referenceProject, Qt::CaseInsensitive);
		script.replace("UPDATEDIR", "/root/update"+projectmanager->projectName()+".upd", Qt::CaseInsensitive);


		if (!file.open(QIODevice::ReadWrite) ) {
			qDebug() << "Could not open or create file in .elbefrontend";
		}

		file.write(script.toUtf8());

		QFileDevice::Permissions permissions = file.permissions();
		file.setPermissions(QFileDevice::ExeUser | permissions);

		file.close();

		return file.fileName();
	}

	QString readUpdateScript()
	{
		QFile file(":/updateScript.exp");
		if (!file.open(QIODevice::ReadOnly)) {
			qDebug() << "Could not open ressource";
		}

		QString content = QString::fromUtf8(file.readAll());
		file.close();
		return content;
	}

	QString sourceXMLPath()
	{
		Project *projectmanager = Project::getInstance();
		QString outP = projectmanager->outPath();
		QFileInfo file(outP+"/source.xml");
		if ( !file.exists() ) {
			getFile("source.xml");
		}

		return file.absoluteFilePath();
	}


	QString buildReferenceProject()
	{
		Project *projectmanager = Project::getInstance();
		QString projectPath = createProjectElbeInstance();
		setXmlFile(projectmanager->buildXmlPath(), projectPath);
		qDebug() << execCommand("elbe control build "+projectPath);
		execCommand("elbe control wait_busy "+projectPath, -1);
		return projectPath;
	}

	void saveUpdateFiles(QString newProject)
	{
		Project *projectmanager = Project::getInstance();

		QString updateNumber = QString().number(projectmanager->getUpdates()->nextUpdateNumber());
		QDir dir(projectmanager->projectDirectory());
		QString updateDir = QString("/updates")+QString("/build")+updateNumber;
		dir.mkdir(updateDir);


		//save source.xml of new version
		getFile_p("source.xml", dir.absolutePath()+"/updates/build"+updateNumber+"/", newProject);

		//save archive.upd
		getFile_p(projectmanager->projectName()+".upd", dir.absolutePath()+"/updates/build"+updateNumber+"/", newProject);

	}
}



