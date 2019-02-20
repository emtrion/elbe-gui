#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include "xmlfilehandler.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileSystemWatcher>

class ProjectItemModel : public QStandardItemModel
{
		Q_OBJECT

	public:
		ProjectItemModel(QObject* parent = 0);
		~ProjectItemModel();
		void setProjectDetails(QString path,QString name);
		void populateProjects(QString path, QString name);
		void createDirectoryItem(QString dirName, QStandardItem *parentItem = NULL);
		QString getItemPath(QModelIndex index);

//		void addPathToWatcher(QString path);
//		void removePathFromWatcher(QString path);
	private:
		QStandardItem *rootItem;
		QIcon dirIcon;
		QIcon fileIcon;
		void initSystemWatcher(QString path);


};

#endif // PROJECTITEMMODEL_H
