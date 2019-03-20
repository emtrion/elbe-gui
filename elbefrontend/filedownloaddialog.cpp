#include "buildprocessworker.h"
#include "elbehandler.h"
#include "filedownloaddialog.h"
#include "ui_filedownloaddialog.h"
#include <QPushButton>
#include <QThread>

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
//	 ElbeHandler *elbeHandler = new ElbeHandler();
//	 ProjectManager *projectmanager = ProjectManager::getInstance();
//	 elbeHandler->getFiles(selectedFiles, projectmanager->getOutPath(), projectmanager->getElbeID());


	QThread *loadingThread = new QThread();
	//use downloadFile method from buildprocessworker
	BuildProcessWorker *fileLoader = new BuildProcessWorker(selectedFiles);

	connect(loadingThread, SIGNAL(started()), fileLoader, SLOT(downloadFiles()));
	connect(loadingThread, SIGNAL(finished()), fileLoader, SLOT(deleteLater()));
	fileLoader->moveToThread(loadingThread);

	loadingThread->start();
}

bool FileDownloadDialog::anythingSelected()
{
	bool selected = false;

	foreach (QCheckBox *box, checkboxes) {
		if ( box->isChecked() ) {
			selected = true;
			break;
		}
	}

	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(selected);

	return selected;
}

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


