/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filedownloaddialog.h"
#include "ui_filedownloaddialog.h"

#include <QPushButton>
#include <QThread>

#include "src/elbe/buildprocessworker.h"
#include "src/elbe/elbehandler.h"

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


