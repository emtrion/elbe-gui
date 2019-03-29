#ifndef NEWXMLDIALOG_H
#define NEWXMLDIALOG_H

#include <QDialog>

namespace Ui {
	class NewXMLDialog;
}

class NewXMLDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit NewXMLDialog(QWidget *parent = 0);
		~NewXMLDialog();

		bool isValid();
		void updateUI(bool valid);

	private slots:
		void on_FileNameEntry_textChanged(const QString &arg1);
		void on_buttonBox_accepted();

	private:
		Ui::NewXMLDialog *ui;
		QString fileName;
		QString projectPath;
		QPixmap acceptIcon;
		QPixmap errorIcon;
};

#endif // NEWXMLDIALOG_H
