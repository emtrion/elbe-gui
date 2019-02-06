#ifndef OPENPROJECTDIALOG_H
#define OPENPROJECTDIALOG_H

#include <QWidget>

namespace Ui {
	class OpenProjectDialog;
}

class OpenProjectDialog : public QWidget
{
		Q_OBJECT

	public:
		explicit OpenProjectDialog(QWidget *parent = 0);
		~OpenProjectDialog();

	private:
		Ui::OpenProjectDialog *ui;
};

#endif // OPENPROJECTDIALOG_H
