#ifndef PROJECT_H
#define PROJECT_H

#include <QString>

#include "projectproperties.h"

class QFileSystemWatcher;

class Project
{
	public:
		/* Static access method. */
		static Project* getInstance();

		ProjectProperties* newProjectProperties() const;
		QString projectPath() const;
		void setProjectPath(const QString &value);
		void update(const QString path);
		QString srcPath() const;
		QString outPath() const;
		QString projectName() const;
		bool isProjectOpened() const;
		void setProjectOpened(bool value);
		QString projectDirectory() const;
		void setProjectDirectory(const QString &value);
		bool isProjectModified() const;
		void setProjectModified(bool value);
		QString elbeID() const;
		void setElbeID(const QString &value);
		bool projectHasFile() const;
		void setProjectHasFile(bool value);
		QString buildXmlPath() const;
		void setBuildXmlPath(const QString &value);
		QFileSystemWatcher *watcher;

	private:
		static Project* instance;
		// Private constructor to prevent instancing from outside
		Project();

		QString m_projectDirectory;
		QString m_projectPath;
		QString m_srcPath;
		QString m_outPath;
		QString m_projectName;
		QString m_elbeID;
		QString m_buildXmlPath;
		ProjectProperties *m_newProjectProperties = new ProjectProperties();
		bool m_projectOpened = false;
		bool m_projectModified = false;
		bool m_projectHasFile = false;
};




#endif // PROJECT_H
