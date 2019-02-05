#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
/* Singelton class
 * see projectmanager.cpp for more info
*/

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

	private:
   /* Here will be the instance stored. */
   static ProjectManager* instance;
   QString projectPath; //path config file where all important data is saved
   QString srcPath;
   QString outPath;
   projectSettings newProjectSettings;

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
};




#endif // PROJECTMANAGER_H
