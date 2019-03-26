#ifndef ELBESETTINGSDIALOG_H
#define ELBESETTINGSDIALOG_H

#include <QDialog>

class ApplicationConfig;

namespace Ui {
	class ElbeSettingsDialog;
}

class ElbeSettingsDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ElbeSettingsDialog(QWidget *parent = 0);
		~ElbeSettingsDialog();

	private slots:
		void on_defaultCheckbox_toggled(bool checked);

		void on_buttonBox_accepted();

		void on_elbeEntry_textChanged(const QString &arg1);

		void on_initVMEntry_textChanged(const QString &arg1);

	private:
		Ui::ElbeSettingsDialog *ui;
		bool fieldsFilled();
		void updateButtonBox(const bool status);
		ApplicationConfig *appConfig;
		bool validElbe(const QString &elbe);

		//used as parameter in updateInformation()
		enum Icon {
			ACCEPT,
			DECLINE
		};

		void updateInformation(Icon value, const QString &text);

		QString oldInitVM;

		QPixmap yesIcon;
		QPixmap errorIcon;
};

#endif // ELBESETTINGSDIALOG_H
