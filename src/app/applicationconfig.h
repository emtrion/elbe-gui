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

#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QDir>
#include <QFile>

#include "yaml-cpp/yaml.h"

class ApplicationConfig
{
	public:
		ApplicationConfig();
		void createDirectory();
		bool exists();
		void createFile();
		void parseFile();
		QString elbeExe() const;
		QString initVM() const;
		void saveElbe(const QString &userInput);
		void saveInitVM(const QString &userInput);
		void saveSchemaFile(const QString &filename);

		QString schemaFile() const;

		QString workspace() const;
		void saveWorkspace(const QString &directory);
	private:
		const QDir *dir; //.elbefrontend
		 QFile *file; //.elbefrontend/config.yaml
		 QString m_elbeExe;
		 QString m_initVM;
		 QString m_schemaFile;
		 QString m_workspace;

	private:
		 YAML::Node loadFile();
		 void writeToFile(const QString &key, const QString &value);
		 QString filePath() const;
};

#endif // APPLICATIONCONFIG_H
