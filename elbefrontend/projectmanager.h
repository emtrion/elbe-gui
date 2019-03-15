#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
/* Singelton class
 * see projectmanager.cpp for more info
*/

#include <QFileSystemWatcher>
#include <QString>

class ProjectManager
{
	public:
		struct projectSettings{
			 QString name;
			 QString version;
			 QString description;
			 QString buildtype;
			 QString host;
			 QString path;
			 QString proto;
			 QString suite;
		 };

		QFileSystemWatcher *watcher;

	private:
		/* Here, the instance will be stored. */
		static ProjectManager* instance;
		QString projectDirectory;
		QString projectPath; //path to config file where all important data is stored
		QString srcPath;
		QString outPath;
		QString projectName;
		QString elbeID;
		QString buildXmlPath; //path to the xml file of the project
		projectSettings newProjectSettings;
		bool projectOpened = false;
		bool projectModified = false;
		bool projectHasFile = false;


		projectSettings emptyStruct;
		/* Private constructor to prevent instancing. */
		ProjectManager();

   public:
		/* Static access method. */
		static ProjectManager* getInstance();

		projectSettings getNewProjectSettings() const;
		void setNewProjectSettings(const projectSettings&value);
		QString getProjectPath() const;
		void setProjectPath(const QString &value);

		void update(QString path);

		QString getSrcPath() const;
		QString getOutPath() const;
		QString getProjectName() const;
		bool isProjectOpened() const;
		void setProjectOpened(bool value);
		QString getProjectDirectory() const;
		void setProjectDirectory(const QString &value);
		bool isProjectModified() const;
		void setProjectModified(bool value);
		QString getElbeID() const;
		void setElbeID(const QString &value);
		bool getProjectHasFile() const;
		void setProjectHasFile(bool value);
		QString getBuildXmlPath() const;
		void setBuildXmlPath(const QString &value);
};




#endif // PROJECTMANAGER_H
