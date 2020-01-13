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

#ifndef ELBEHANDLER_H
#define ELBEHANDLER_H

class QStringList;
class QString;
class QFile;

namespace ElbeHandler {

	void isVersionSupported();
	QString checkElbeVersion();

	QString createProjectElbeInstance();
	bool deleteProjectElbeInstance(QString projectID);
	bool projectIsInElbe(QString projectPath);
	bool projectIsBuilding(QString projectPath);

	bool startBuildProcess(bool sourceOptionChecked, bool binOptionChecked);
	bool setXmlFile(QString file, QString elbeID);

	bool getFiles(QStringList filenames);
	bool getFile(QString filename);
	bool getImages();
	bool getFiles(QStringList filenames, QString outPath, QString elbeID);
	bool getFile(QString filename, QString outPath, QString elbeID);
	bool getImages(QString buildXmlPath, QString outPath, QString elbeID);

	bool checkIfBusy(QString id);
	bool restartInitVM(const QString &initVM);

	QStringList buildUpdate();
	QString checkForUpdates();
	QString sourceXMLPath();

	QString prepareUpdateScript(QString);
	QString readUpdateScript();
	QString buildReferenceProject();
	void saveUpdateFiles(QString);
	void createNewInitVM(QString);


	/*------- these are defined in an unnamed namespace ---------------------------------------*
	 *																						   *
	 *  QStringList makeProjectList();											               *
	 * 	QString execCommand(QString command, int timeout, bool forceWithoutPrefix = false);	   *
	 * 	bool getFiles_p(QStringList filenames, QString outPath, QString elbeID);               *
	 * 	bool getFile_p(QString filename, QString outPath, QString elbeID);		               *
	 * 	bool getImages_p(QString buildXmlPath, QString outPath, QString elbeID);               *
	 * 	void removeFalseLoadedFiles(QString outPath, QString filename);			               *
	 *	void forwardOutputToMessageLog(QString output);						                   *
	 *																			               *
	 *-----------------------------------------------------------------------------------------*/

}

#endif // ELBEHANDLER_H
