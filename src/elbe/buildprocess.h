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

#ifndef BUILDPROCESS_H
#define BUILDPROCESS_H


#include <QObject>

class QThread;
class BuildManager;
class BuildProcessWorker;


class BuildProcess : public QObject
{
		Q_OBJECT

	public:
		explicit BuildProcess(QObject *parent = 0);
		~BuildProcess();

		void startBuild(bool sourceOptionChecked, bool binOptionChecked);
		void waitBusyWithoutStartingBuild(QString id);

	private slots:
		void cleanup();

	private:
		void buildThreadInit();

	private:
		QThread *buildThread;
		BuildManager *buildmanager;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
		BuildProcessWorker *buildWorker;
};

#endif // BUILDPROCESS_H
