#include "filedownloaddialog.h"
#include "ui_filedownloaddialog.h"

#include <QPushButton>
#include <QThread>

#include "buildprocessworker.h"
#include "elbehandler.h"

FileDownloadDialog::FileDownloadDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FileDownloadDialog)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Load");
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	checkboxes = this->findChildren<QCheckBox*>();
}

FileDownloadDialog::~FileDownloadDialog()
{
	delete ui;
}

void FileDownloadDialog::on_buttonBox_accepted()
{
	foreach (QCheckBox *box, checkboxes) {
		if ( box->isChecked() ) {
			selectedFiles.append(box->text());
		}
	}

	QThread *loadingThread = new QThread();

	//use the downloadFiles method from the buildProcessWorker
	BuildProcessWorker *fileLoader = new BuildProcessWorker(selectedFiles);

	connect(loadingThread, SIGNAL(started()), fileLoader, SLOT(downloadFiles()));
	connect(loadingThread, SIGNAL(finished()), fileLoader, SLOT(deleteLater()));
	fileLoader->moveToThread(loadingThread);

	loadingThread->start();
}

void FileDownloadDialog::anythingSelected()
{
	bool status = false;
	foreach (QCheckBox *box, checkboxes) {
		if ( box->isChecked() ) {
			status = true;
			break;
		}
	}
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(status);
}

//everytime a checkbox is clicked we need to ensure if the Ok button can be enabled

void FileDownloadDialog::on_LogCheck_clicked()
{
	anythingSelected();
}

void FileDownloadDialog::on_ReportCheck_clicked()
{
	anythingSelected();
}

void FileDownloadDialog::on_ValidationCheck_clicked()
{
	anythingSelected();
}

void FileDownloadDialog::on_LicenseXmlCheck_clicked()
{
	anythingSelected();
}

void FileDownloadDialog::on_LicenseCheck_clicked()
{
	anythingSelected();
}

void FileDownloadDialog::on_ImageCheck_clicked()
{
	anythingSelected();
}

void FileDownloadDialog::on_sourceCheck_clicked()
{
	anythingSelected();
}


