#include "projectlistitem.h"

ProjectListItem::ProjectListItem(QString text, QString path)
{
	this->setText(text);
	this->projectPath = path;
}

QString ProjectListItem::getProjectPath() const
{
	return projectPath;
}

void ProjectListItem::setProjectPath(const QString &value)
{
	projectPath = value;
}
