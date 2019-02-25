#ifndef PROJECTLISTITEM_H
#define PROJECTLISTITEM_H

#include <QListWidgetItem>

class ProjectListItem : public QListWidgetItem
{
	public:
		ProjectListItem(QString text, QString path);
		QString getProjectPath() const;
		void setProjectPath(const QString &value);

	private:
		QString projectPath;
};

#endif // PROJECTLISTITEM_H
