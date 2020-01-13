#include "projectlistitem.h"

ProjectListItem::ProjectListItem(QString text, QString path)
{
	this->setText(text);
	this->m_projectPath = path;
}

QString ProjectListItem::projectPath() const
{
	return m_projectPath;
}

void ProjectListItem::setProjectPath(const QString &value)
{
	m_projectPath = value;
}
