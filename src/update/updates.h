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

#ifndef UPDATES_H
#define UPDATES_H

#include "updatelistitem.h"

#include <QFile>
#include <QObject>
#include "updatethread.h"
#include "checkupdatethread.h"

class Updates : public QObject
{
		Q_OBJECT

	public:
		Updates(QString);

		void checkForUpdates();
		void makeUpdate();
		void updatesAvailable(int avialablePkgs);

		QList<UpdateListItem> getUpdateList() const;


		void makeInitialEntry();
		int nextUpdateNumber();

		void initUpdateList();
	signals:
		void notifyViewAfterCheckUpdates(UpdateListItem);
		void notifyViewAfterBuildUpdates(UpdateListItem);

	public slots:
		void handleCheckResult(QString result);

		void handleUpdateResult();
		void showErrorMessage(QStringList result);
	private:

		UpdateListItem makeListEntry(QStringList list);
		QList<UpdateListItem> updateList;
		UpdateListItem modifyLastEntry(QString availableUpdates);

		UpdateThread *uthread;
		CheckUpdateThread *cuthread;


		QFile *updateHistoryFile;
		void saveListEntryToHistory();
};

#endif // UPDATES_H

