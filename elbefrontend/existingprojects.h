#ifndef EXISTINGPROJECTS_H
#define EXISTINGPROJECTS_H

#include "projectlistitem.h"

#include <QFile>
#include <QFileSystemWatcher>

class ExistingProjects : public QObject
{
		Q_OBJECT
	public:
		explicit ExistingProjects(QObject *parent = 0);
		void addNewProjectToList(QString projectPath);

		void removeProjectFromList(QString projectPath);

		QList<ProjectListItem *> getExistingProjects() const;

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
};

#endif // EXISTINGPROJECTS_H
