#ifndef IMPORTFILEDIALOG_H
#define IMPORTFILEDIALOG_H

#include <QDialog>

namespace Ui {
	class ImportFileDialog;
}

class ImportFileDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ImportFileDialog(QWidget *parent = 0);
		~ImportFileDialog();

	private slots:
		void on_buttonBox_accepted();

		void on_FileNameEntry_textChanged(const QString &arg1);

		void on_BrowseButton_clicked();

	private:
		Ui::ImportFileDialog *ui;
		QString filePath;
		QString fileName;
		QString srcFolder;
		void updateUI(bool valid);
		bool isValid();
		void chopNameFromPath();
		QPixmap yesIcon;
		QPixmap errorIcon;
};

#endif // IMPORTFILEDIALOG_H
