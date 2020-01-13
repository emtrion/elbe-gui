/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "projectitemmodel.h"

#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QFileIconProvider>


ProjectItemModel::ProjectItemModel(QObject *parent) :
	QStandardItemModel(parent)
{
	rootItem = this->invisibleRootItem();
	dirIcon = QApplication::style()->standardIcon(QStyle::SP_DirIcon);

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

QString ProjectItemModel::itemPath(QModelIndex index)
{
	QStandardItem *item = this->itemFromIndex(index);
	return item->accessibleDescription();
}
