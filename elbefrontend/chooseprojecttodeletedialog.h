#ifndef CHOOSEPROJECTTODELETEDIALOG_H
#define CHOOSEPROJECTTODELETEDIALOG_H

#include <QDialog>

class ProjectListItem;
class ExistingProjects;

namespace Ui {
	class ChooseProjectToDeleteDialog;
}

class ChooseProjectToDeleteDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ChooseProjectToDeleteDialog(QWidget *parent = 0);
		~ChooseProjectToDeleteDialog();

	private slots:
		void on_buttonBox_accepted();

	private:
		Ui::ChooseProjectToDeleteDialog *ui;
		bool listIsEmpty();
		void addListItem(QString item);
		void addItems(QList<ProjectListItem*> itemList);
		ExistingProjects *existingProjectsHandler;
};

#endif // CHOOSEPROJECTTODELETEDIALOG_H
