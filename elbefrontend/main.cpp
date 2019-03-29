#include "mainwindow.h"

#include <QApplication>
#include <QTextStream>

#include "elbehandler.h"
#include "existingprojects.h"
#include "buildmanager.h"
#include "applicationconfig.h"
#include "projecthandler.h"
#include "buildprocess.h"
#include "filedownloaddialog.h"
#include "elbesettingsdialog.h"
#include "helpers.h"
#include "xmlutilities.h"
#include <QMessageBox>
#include "filesystemwatcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MainWindow w;

	QString id;
	auto existing = new ExistingProjects();
	auto process = new BuildProcess();
	auto dialog = new FileDownloadDialog();
	auto applicationConfig = new ApplicationConfig();
	auto buildmanager = BuildManager::getInstance();


	//load stylesheet
	QFile stylesheet(":qdarkstyle/style.qss");
	if ( !stylesheet.exists() )
	{
		printf("Unable to set stylesheet, file not found\n");
	} else {
		stylesheet.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&stylesheet);
		qApp->setStyleSheet(ts.readAll());
	}

	filesystemWatcher::init();

	if ( !applicationConfig->exists() ) {
		applicationConfig->createDirectory();
		applicationConfig->createFile();
		helpers::showMessageBox("Warning",
							   "There is no initVM selected. Please set the initVM you would like to use.",
							   QMessageBox::StandardButtons(QMessageBox::Ok),
							   QMessageBox::Ok);
		auto elbeSettingsDialog = new ElbeSettingsDialog();
		elbeSettingsDialog->show();
	}
	if ( applicationConfig->elbeExe().compare("default") == 0 ) {
		//setElbeWorkingDirectory changes "default" to system home directory
		buildmanager->setElbeWorkingDir("default");
		buildmanager->setElbeCommandPrefix("");
	} else {
		buildmanager->setElbeWorkingDir(applicationConfig->elbeExe());
		buildmanager->setElbeCommandPrefix("./");
	}

	//open the project which was still open when prior session was closed
	QString openProject = existing->checkForOpenFlag();
	if ( !openProject.isEmpty() ) {
		ProjectHandler::openProject(openProject);
	}

	//checks if there was a build running when prior session was closed
	QString busyProject = existing->checkForBusyFlag();
	if ( !busyProject.isEmpty() ) {
		id = xmlUtilities::getProjectID(busyProject);
		if ( ElbeHandler::checkIfBusy(id) ) {
			process->waitBusyWithoutStartingBuild(id);
			helpers::showMessageBox("Information",
									"The build you started in a previous session is still in progess",
									QMessageBox::StandardButtons(QMessageBox::Ok),
									QMessageBox::Ok);
		} else {
			int ret = helpers::showMessageBox("The build you started in a previous session has finished",
											  "Couldn't download your selected files as the application was closed. Would you like to do that now?",
											  QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
											  QMessageBox::Yes);
			switch ( ret ) {
				case QMessageBox::Yes :
					ProjectHandler::openProject(busyProject);
					dialog->show();
					break;
				case QMessageBox::No :
					break;
				default:
					//shouldn't be reached
					break;
			}
		}
	}

	//open mainwindow. First time to see the actual application
	w.show();

	//check if version is supported
	ElbeHandler::isVersionSupported();
	//retrieve elbe version and show it in statusbar
	w.setElbeVersion(ElbeHandler::checkElbeVersion());

    return a.exec();
}



