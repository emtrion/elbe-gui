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
	parseFile();
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
}

QString ApplicationConfig::elbeExe() const
{
	return m_elbeExe;
}

QString ApplicationConfig::initVM() const
{
	return m_initVM;
}
