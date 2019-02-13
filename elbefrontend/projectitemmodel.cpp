#include "projectitemmodel.h"

#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFileIconProvider>

ProjectItemModel::ProjectItemModel(QObject *parent) : QStandardItemModel(parent)
{
	rootItem = this->invisibleRootItem();
	dirIcon = QApplication::style()->standardIcon(QStyle::SP_DirIcon);
//	fileIcon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);

	this->setColumnCount(1);
}

ProjectItemModel::~ProjectItemModel()
{

}

void ProjectItemModel::setProjectDetails(QString path, QString name)
{
	populateProjects(path, name);
}

void ProjectItemModel::populateProjects(QString path, QString name)
{
	QStandardItem *parent;
	parent = new QStandardItem(dirIcon, name);
	parent->setAccessibleDescription(path);
	rootItem->appendRow(parent);
	createDirectoryItem(path, parent);
}

void ProjectItemModel::createDirectoryItem(QString dirPath, QStandardItem *parentItem)
{
	QDir dir(dirPath);
	QFileInfoList subFolders;
	QFileInfo folderName;
	QStandardItem *child;
	subFolders = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	foreach (folderName, subFolders) {
		if (folderName.isFile()) {
			QFileInfo info(folderName);
			fileIcon = QFileIconProvider().icon(info);
			child = new QStandardItem(fileIcon, folderName.fileName());
			child->setAccessibleDescription(folderName.filePath());
		} else {
			child = new QStandardItem(dirIcon, folderName.fileName());
			child->setAccessibleDescription(folderName.filePath());
		}
		parentItem->appendRow(child);
		createDirectoryItem(folderName.filePath(), child);
	}
}

QString ProjectItemModel::getItemPath(QModelIndex index)
{
	QStandardItem *item = this->itemFromIndex(index);
	return item->accessibleDescription();
}


