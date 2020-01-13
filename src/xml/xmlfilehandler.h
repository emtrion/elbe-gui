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

#ifndef XMLFILEHANDLER_H
#define XMLFILEHANDLER_H


#include <QString>

class XmlFile;
class Project;

namespace XmlFileHandler {

	void openFile(const QString filePath);
	void createFile(const QString projectPath, const QString fileName);
	void XMLautoGenerate(const QString filePath);
	void saveFile();
	void closeFile();
	void handleFileModification(QString file);
	void handleRemotelyDeletedFile();
}


#endif // XMLFILEHANDLER_H
