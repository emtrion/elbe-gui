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

#include "applicationconfig.h"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <fstream>

#include "helpers.h"
#include "yaml-cpp/yaml.h"


ApplicationConfig::ApplicationConfig()
{
	dir = new QDir(helpers::getHomeDirectoryFromSystem()+"/.elbefrontend");
	file = new QFile(dir->absolutePath()+"/config.yaml");

	if (file->exists() ) {
		parseFile();
	}
}

bool ApplicationConfig::exists()
{
	return dir->exists();
}

void ApplicationConfig::createDirectory()
{
	if ( !QDir().mkpath(dir->absolutePath()) ) {
		qDebug() << "ERROR from "<<__func__<<": couldn't create directory";
	}
}

QString ApplicationConfig::filePath() const
{
	QFileInfo info(*file);
	return info.absoluteFilePath();
}

void ApplicationConfig::createFile()
{
	QFile::copy(":/globalConfig.yaml", this->filePath());
	file->setPermissions(QFileDevice::WriteUser | QFileDevice::ReadUser);

	parseFile();
}

YAML::Node ApplicationConfig::loadFile()
{
	return YAML::LoadFile(this->filePath().toStdString());
}

void ApplicationConfig::saveElbe(const QString &userInput)
{
	writeToFile("elbe", userInput);
}

void ApplicationConfig::saveInitVM(const QString &userInput)
{
	writeToFile("initVM", userInput);
}

void ApplicationConfig::saveSchemaFile(const QString &filename)
{
	writeToFile("schemaFile", filename);
}

void ApplicationConfig::saveWorkspace(const QString &directory)
{
	writeToFile("workspace", directory);
}

QString ApplicationConfig::schemaFile() const
{
	return m_schemaFile;
}

void ApplicationConfig::writeToFile(const QString &key, const QString &value)
{
	YAML::Node config = loadFile();

	config[key.toStdString()] = value.toStdString();

	std::ofstream fout(this->filePath().toStdString());
	fout << config;
}

void ApplicationConfig::parseFile()
{
	YAML::Node config = loadFile();

	if ( config["elbe"] ) {
		m_elbeExe = QString().fromStdString(config["elbe"].as<std::string>());
	}

	if (config["initVM"] ){
		m_initVM = QString().fromStdString(config["initVM"].as<std::string>());
	}

	if (config["schemaFile"] ){
		m_schemaFile = QString().fromStdString(config["schemaFile"].as<std::string>());
	}

	if ( config["workspace"] ) {
		m_workspace = QString().fromStdString(config["workspace"].as<std::string>());
	}
}

QString ApplicationConfig::elbeExe() const
{
	return m_elbeExe;
}

QString ApplicationConfig::initVM() const
{
	return m_initVM;
}

QString ApplicationConfig::workspace() const
{
	return m_workspace;
}
