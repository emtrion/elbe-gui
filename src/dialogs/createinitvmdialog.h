#ifndef CREATEINITVMDIALOG_H
#define CREATEINITVMDIALOG_H

#include <QDialog>

namespace Ui {
	class CreateInitvmDialog;
}

class CreateInitvmDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit CreateInitvmDialog(QWidget *parent = 0);
		~CreateInitvmDialog();

	private slots:
		void on_buttonBox_accepted();

		void on_initVMEntry_textChanged(const QString &arg1);

	private:
		Ui::CreateInitvmDialog *ui;

		QPixmap acceptIcon;
		QPixmap errorIcon;
		bool validateEntry(QString str);
};

#endif // CREATEINITVMDIALOG_H
