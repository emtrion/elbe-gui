#include "applicationconfig.h"
#include "helpers.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include "yaml-cpp/yaml.h"


#include <iostream>
#include <fstream>

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
//	qDebug() << __func__<<": "<<this->filePath();

	YAML::Node config = loadFile();

	config[key.toStdString()] = value.toStdString();

	std::ofstream fout("/home/hico/.elbefrontend/config.yaml"/*this->filePath().toStdString()*/);
	fout << config;
}

void ApplicationConfig::parseFile()
{
	YAML::Node config = loadFile();

	if ( config["elbe"] ) {
		m_elbeExe = QString().fromStdString(config["elbe"].as<std::string>());
//		qDebug() << "Got elbe: "<<m_elbeExe;
	}

	if (config["initVM"] ){
		m_initVM = QString().fromStdString(config["initVM"].as<std::string>());
//		qDebug() << "Got initVM: "<<m_initVM;
	}
}

QString ApplicationConfig::elbeExe() const
{
//	qDebug() << __func__<<" is about to return elbeExe";
//	parseFile();
	return m_elbeExe;
}

QString ApplicationConfig::initVM() const
{
//	parseFile();
	return m_initVM;
}
