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
