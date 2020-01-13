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

#ifndef EXISTINGPROJECTS_H
#define EXISTINGPROJECTS_H


#include <QObject>

class QStringList;
class QFile;
class ProjectListItem;

class ExistingProjects : public QObject
{
		Q_OBJECT

	public:
		explicit ExistingProjects(QObject *parent = 0);

		void addNewProjectToListFile(QString projectPath);
		void removeProjectFromListFile(QString projectPath);
		QList<ProjectListItem *> existingProjects();
		void addBusyFlag(const QString &projectPath);
		void addOpenFlag(const QString &projectPath);
		QString checkForBusyFlag();
		QString checkForOpenFlag();
		bool exists();
		void createprojectListFile();
	private:
		QFile *projectListFile;

	private:
		QStringList initFileList();
		QList<ProjectListItem *> createExistingProjectsList();
		QString removeBusyFlag(int index, QStringList list);
		QString removeOpenFlag(int index, QStringList list);
		void updateListFile(const QStringList &projectFileList);
};

#endif // EXISTINGPROJECTS_H
