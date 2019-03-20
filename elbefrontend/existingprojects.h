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
		void addNewProjectToList(QString projectPath);

		void removeProjectFromList(QString projectPath);

		QList<ProjectListItem *> getExistingProjects();

		void addBusyFlag(const QString &projectPath);

		QString checkForBusyFlag();
		void addOpenFlag(const QString &projectPath);
		QString checkForOpenFlag();
	signals:

	public slots:

	private:
		QFile *projectListFile;
		QList<ProjectListItem*> existingProjects;
		QStringList projectFileList;
		QStringList projectDirectoryList;
		void updateList();
		void initFileList();
		void putItemInList(QString name, QString path);
		QString removeBusyFlag(int index);
		QString removeOpenFlag(int index);
};

#endif // EXISTINGPROJECTS_H
