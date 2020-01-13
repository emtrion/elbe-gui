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

#include "updatelistitem.h"
#include "updates.h"
#include "src/elbe/elbehandler.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include "updatethread.h"
#include "checkupdatethread.h"
#include <src/mainwindow/mainwindow.h>
#include "src/app/helpers.h"

Updates::Updates(QString projectPath)
{
	updateHistoryFile = new QFile(projectPath+"/.update_history");
}


void Updates::initUpdateList()
{


	if ( !updateHistoryFile->open(QIODevice::ReadWrite) ) {
		qDebug() << "ERROR from "<<__func__<<" could not open or create update-history-file";
		return;
	}
	QString fileContent = QString().fromUtf8(updateHistoryFile->readAll());
	updateHistoryFile->close();
	QStringList list = fileContent.split("\n", QString::SkipEmptyParts);

	foreach (auto str, list) {
		QStringList entry = str.split("\t\t");
		emit(notifyViewAfterBuildUpdates(makeListEntry(entry)));
	}
}

void Updates::saveListEntryToHistory()
{
	QString complete;
	foreach (auto item, updateList) {
		complete.append(item.toString()+"\n");
	}

	if ( !updateHistoryFile->open(QIODevice::ReadWrite) ) {
		qDebug() << "ERROR from "<<__func__<<" could not open or create update-history-file";
		return;
	}
	updateHistoryFile->write(complete.toUtf8());
	updateHistoryFile->close();
}

void Updates::checkForUpdates()
{
	qDebug() << "checking for updates";

	//thread initialization
	cuthread = new CheckUpdateThread();
	connect(cuthread, SIGNAL(resultReady(QString)), this, SLOT(handleCheckResult(QString)));
	connect(cuthread, SIGNAL(finished()), cuthread, SLOT(deleteLater()));


	cuthread->start();
}

void Updates::makeUpdate()
{
	qDebug() << "make update";

	//thread initialization
	uthread = new UpdateThread();
	connect(uthread, SIGNAL(resultReady(QStringList)), this, SLOT(handleUpdateResult()));
	connect(uthread, SIGNAL(errorLog(QStringList)), this, SLOT(showErrorMessage(QStringList)));
	connect(uthread, SIGNAL(finished()), uthread, SLOT(deleteLater()));

	uthread->start();
}

void Updates::makeInitialEntry()
{
	Project *p = Project::getInstance();
	QString version = "Version "+p->newProjectProperties()->version();
	QStringList list = QStringList() << "Build 1" << version << QDate::currentDate().toString("dd.MM.yyyy") << "";

	emit(notifyViewAfterBuildUpdates(makeListEntry(list)));
}

int Updates::nextUpdateNumber()
{
	return updateList.size()+1;
}

//put the information together
UpdateListItem Updates::makeListEntry(QStringList list)
{

	if ( list.isEmpty() ) {

		list.append("Build "+QString().number(updateList.size()+1));

		//TODO: Version is just increased with 0.1 … probably have to change that
		QString newVersion = QString().number(updateList.last().getVersion().toDouble()+0.1);
		list.append("Version "+ newVersion);
		list.append(QDate::currentDate().toString("dd.MM.yyyy"));
		list.append("");
	}


	//BuildNo << Version << Date << Info
	UpdateListItem item(list.at(0), list.at(1), list.at(2), list.at(3));
	updateList.append(item);
	return item;
}

UpdateListItem Updates::modifyLastEntry(QString availableUpdates)
{
	updateList.last().setInfo(availableUpdates);

	return updateList.last();
}

QList<UpdateListItem> Updates::getUpdateList() const
{
	return updateList;
}

void Updates::handleCheckResult(QString result)
{
	emit(notifyViewAfterCheckUpdates(modifyLastEntry(result)));

	cuthread->quit();
	cuthread->wait();

	saveListEntryToHistory();
}

void Updates::handleUpdateResult()
{
	emit(notifyViewAfterBuildUpdates(makeListEntry(QStringList())));

	uthread->quit();
	uthread->wait();

	saveListEntryToHistory();
}

void Updates::showErrorMessage(QStringList result)
{
	MainWindow *mw = helpers::getMainWindow();
	mw->messageLogAppendText(result.first(), "#F36363");
}


