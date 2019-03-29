#ifndef EXISTINGPROJECTS_H
#define EXISTINGPROJECTS_H


#include <QObject>

class QStringList;
class QFile;
class ProjectListItem;

class ExistingProjects : public QObject
{
		Q_OBJECT

	public:
		explicit ExistingProjects(QObject *parent = 0);

		void addNewProjectToListFile(QString projectPath);
		void removeProjectFromListFile(QString projectPath);
		QList<ProjectListItem *> existingProjects();
		void addBusyFlag(const QString &projectPath);
		void addOpenFlag(const QString &projectPath);
		QString checkForBusyFlag();
		QString checkForOpenFlag();
		bool exists();
		void createprojectListFile();
	private:
		QFile *projectListFile;

	private:
		QStringList initFileList();
		QList<ProjectListItem *> createExistingProjectsList();
		QString removeBusyFlag(int index, QStringList list);
		QString removeOpenFlag(int index, QStringList list);
		void updateListFile(const QStringList &projectFileList);
};

#endif // EXISTINGPROJECTS_H
