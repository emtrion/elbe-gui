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

#ifndef BUILDPROCESSWORKER_H
#define BUILDPROCESSWORKER_H


#include <QObject>

class QProcess;
class QThread;
class BuildProcessStatusBarUpdate;
class Project;
class BuildManager;

class BuildProcessWorker : public QObject
{
		Q_OBJECT

	public:
		explicit BuildProcessWorker(QStringList outputFiles);
		~BuildProcessWorker();

		QString buildingProjectPath() const;
		void setSkipDownload(bool value);
		QThread *statusBarBuildThread() const;
		void setBuildingElbeID(const QString &value);
		QString buildingElbeID() const;

	signals:
		void resultReady();
		void outputReady(const QString &str);
		void messageLogHasUpdate(const QString &str, const QString &colorHexValue);
		void startStatusBarBuild();
		void startStatusBarLoad();
		void done();

	public slots:
		void doWork();
		void printLog();
		void updateMessageLog(const QString &str);
		void downloadFiles();

	private:
		QProcess *process;
		QString output;
		QStringList outputFiles;
		QString m_buildingProjectPath;
		QString m_buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
		Project *projectmanager;
		BuildManager *buildmanager;
		BuildProcessStatusBarUpdate *statusBarWorker;
		bool skipDownload;
		QThread *m_statusBarBuildThread;
		QThread *statusBarLoadThread;

	private:
		void showLoadingInStatusBar();
		void showBuildingInStatusBar();
		void waitBusy();





};

#endif // BUILDPROCESSWORKER_H
