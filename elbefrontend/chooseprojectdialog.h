#ifndef CHOOSEPROJECTDIALOG_H
#define CHOOSEPROJECTDIALOG_H

#include "projectlistitem.h"

#include <QDialog>

namespace Ui {
	class ChooseProjectDialog;
}

class ChooseProjectDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ChooseProjectDialog(QWidget *parent = 0);
		~ChooseProjectDialog();

	private slots:
		void on_buttonBox_accepted();

	private:
		Ui::ChooseProjectDialog *ui;
		bool listIsEmpty();
		void addListItem(QString item);
		void addItems(QList<ProjectListItem*> itemList);
};

#endif // CHOOSEPROJECTDIALOG_H
