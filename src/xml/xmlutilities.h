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

#ifndef XMLUTILITIES_H
#define XMLUTILITIES_H

class QString;
class QFile;
class QDomDocument;
class QStringList;
class QByteArray;

namespace xmlUtilities {

	QDomDocument parseXMLFile(QFile *file);
	bool saveXMLChanges(QFile *file, QByteArray byteArray);
	QString getProjectID(QString confFile);
	QString getProjectName(QString confFile);
	QString parseProjectDescriptionForNodeValue(QString confFile, QString node);
	QStringList getImageFiles(QString buildXmlPath);
	bool setProjectMetadata(QString projectName, QString projectPath);
}

#endif // XMLUTILITIES_H
