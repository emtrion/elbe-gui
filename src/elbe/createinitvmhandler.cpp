#include "createinitvmhandler.h"
#include "initvmcreatethread.h"

#include <src/mainwindow/mainwindow.h>
#include "src/app/helpers.h"

CreateInitvmHandler::CreateInitvmHandler(QObject *parent) : QObject(parent)
{

}

void CreateInitvmHandler::createNewInitvm(QString newInitvm)
{
	prepareStart();

	//start thread
	InitvmCreateThread *thread = new InitvmCreateThread();
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(thread, SIGNAL(ready()), this, SLOT(handleFinishedThread()));
	thread->setNewInitvmPath(newInitvm);
	thread->start();
}


void CreateInitvmHandler::prepareStart()
{
	MainWindow *mw = helpers::getMainWindow();
	mw->changeElbeActionsEnabledStatus(false);
}

void CreateInitvmHandler::handleFinishedThread()
{
	MainWindow *mw = helpers::getMainWindow();
	mw->changeElbeActionsEnabledStatus(true);

	helpers::showMessageBox("Initvm created",
							"The new initVM is running. Your initVM-settings were automatically changed to this vm.",
							QMessageBox::Ok,
							QMessageBox::Ok );
}
