#ifndef CHANGEWORKSPACEDIALOG_H
#define CHANGEWORKSPACEDIALOG_H

#include <QDialog>

#include <src/app/applicationconfig.h>

namespace Ui {
	class ChangeWorkspaceDialog;
}

class ChangeWorkspaceDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ChangeWorkspaceDialog(QWidget *parent = 0);
		~ChangeWorkspaceDialog();

	private slots:
		void on_directoryEntry_textChanged(const QString &arg1);

		void on_defaultCheckbox_toggled(bool checked);

		void on_buttonBox_accepted();

		void on_browseButton_clicked();

	private:
		Ui::ChangeWorkspaceDialog *ui;

		QPixmap acceptIcon;
		QPixmap errorIcon;
		QString defaultEntry;
		void checkIfValid(QString input);
		ApplicationConfig *appConfig;



};

#endif // CHANGEWORKSPACEDIALOG_H
