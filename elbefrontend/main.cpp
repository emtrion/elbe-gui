#include <iostream>
#include "mainwindow.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QProcessEnvironment>
#include <QRect>
#include <QScreen>
#include "projectmanager.h"
#include "helpers.h"
#include "elbehandler.h"
#include "existingprojects.h"
#include "buildmanager.h"
#include "applicationconfig.h"
#include "projecthandler.h"
#include "buildprocess.h"
#include "filedownloaddialog.h"
#include "elbesettingsdialog.h"
#include "helpers.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MainWindow w;

	QString id;
	auto existing = new ExistingsProjects();
	auto process = new BuildProcess();
	auto dialog = new FileDownloadDialog();
	auto elbehandler = new ElbeHandler();
	auto projecthandler = new ProjectHandler();
	auto appConf = new ApplicationConfig();
	auto buildmanager = BuildManager::getInstance();

	QFile f(":qdarkstyle/style.qss");
	if (!f.exists())
	{
		printf("Unable to set stylesheet, file not found\n");
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		qApp->setStyleSheet(ts.readAll());
	}

	//set the starting position of mainwindow
	QRect screenrect = a.primaryScreen()->geometry();
	w.move(screenrect.left(), screenrect.top());

	ProjectManager *pm = ProjectManager::getInstance();
	pm->setProjectOpened(false);

	w.show();
	helpers::initSystemWatcher();



	if ( !appConf->exists() ) {
		appConf->createDirectory();
		appConf->createFile();
		helpers::showMessageBox("Warning",
							   "There is no initVM selected. Please set the initVM you would like to use.",
							   QMessageBox::StandardButtons(QMessageBox::Ok),
							   QMessageBox::Ok);
		auto elbeSettingsDialog = new ElbeSettingsDialog();
		elbeSettingsDialog->show();
	}

//		qDebug() << buildmanager->getElbeWorkingDir() << appConf->elbeExe();

	if ( appConf->elbeExe().compare("default") == 0 ) {
		buildmanager->setElbeWorkingDir("home/hico/");
		buildmanager->setElbeCommandPrefix("");
	} else {
		buildmanager->setElbeWorkingDir(appConf->elbeExe());
		buildmanager->setElbeCommandPrefix("./");
	}

	QString projectPath;
	projectPath = existing->checkForOpenFlag();
	if ( !projectPath.isEmpty() ) {
		projecthandler->openProject(projectPath);
	}

	projectPath = existing->checkForBusyFlag();
	if ( !projectPath.isEmpty() ) {
		id = helpers::getProjectID(projectPath);
		if ( elbehandler->checkIfBusy(id) ) {
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
					projecthandler->openProject(projectPath);
					dialog->show();
					break;
				case QMessageBox::No :
					break;
				default:
					//shouldn't be reached
					break;
			}
		}
	} else {
		qDebug() << "no busyflag found";
	}

	//retrieve elbe version and show it in statusbar
	w.setElbeVersion(elbehandler->checkElbeVersion());



    return a.exec();
}



