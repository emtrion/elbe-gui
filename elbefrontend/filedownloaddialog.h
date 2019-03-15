#ifndef FILEDOWNLOADDIALOG_H
#define FILEDOWNLOADDIALOG_H

#include <QCheckBox>
#include <QDialog>

namespace Ui {
	class FileDownloadDialog;
}

class FileDownloadDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit FileDownloadDialog(QWidget *parent = 0);
		~FileDownloadDialog();

	private slots:
		void on_LogCheck_clicked();

		void on_ReportCheck_clicked();

		void on_ValidationCheck_clicked();

		void on_LicenseXmlCheck_clicked();

		void on_LicenseCheck_clicked();

		void on_ImageCheck_clicked();

		void on_sourceCheck_clicked();

		void on_buttonBox_accepted();

	private:
		Ui::FileDownloadDialog *ui;
		QList<QCheckBox *> checkboxes;
		QStringList selectedFiles;
		bool anythingSelected();
};

#endif // FILEDOWNLOADDIALOG_H
