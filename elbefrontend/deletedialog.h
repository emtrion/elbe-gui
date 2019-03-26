#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>

class ProjectListItem;
class ExistingsProjects;

namespace Ui {
	class DeleteDialog;
}

class DeleteDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit DeleteDialog(QWidget *parent = 0);
		~DeleteDialog();

	private slots:
		void on_buttonBox_accepted();

	private:
		Ui::DeleteDialog *ui;
		bool listIsEmpty();
		void addListItem(QString item);
		void addItems(QList<ProjectListItem*> itemList);
		ExistingsProjects *existingProjectsHandler;
};

#endif // DELETEDIALOG_H
