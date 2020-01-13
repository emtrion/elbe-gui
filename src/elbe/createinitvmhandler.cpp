/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

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
