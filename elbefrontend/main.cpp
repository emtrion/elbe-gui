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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MainWindow w;

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
	ElbeHandler *elbe = new ElbeHandler();
	w.setElbeVersion(elbe->checkSystemForElbeVersion());

    return a.exec();
}



