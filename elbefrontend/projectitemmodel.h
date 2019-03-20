#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H


#include <QStandardItemModel>


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
};

#endif // PROJECTITEMMODEL_H
