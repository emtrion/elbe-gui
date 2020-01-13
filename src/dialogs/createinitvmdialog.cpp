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

#include "createinitvmdialog.h"
#include "ui_createinitvmdialog.h"

#include <QDir>
#include <QPushButton>
#include <QRegExpValidator>
#include "src/elbe/elbehandler.h"
#include <src/elbe/createinitvmhandler.h>
#include <src/elbe/initvmcreatethread.h>

CreateInitvmDialog::CreateInitvmDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateInitvmDialog)
{
	ui->setupUi(this);

	acceptIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");


	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Create");
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	QRegExpValidator *v = new QRegExpValidator(QRegExp("^[A-Za-z0-9\/]+"), ui->initVMEntry);
	ui->initVMEntry->setValidator(v);
}

CreateInitvmDialog::~CreateInitvmDialog()
{
	delete ui;
}

void CreateInitvmDialog::on_buttonBox_accepted()
{
	CreateInitvmHandler *handler = new CreateInitvmHandler();
	handler->createNewInitvm(ui->initVMEntry->text());
}

void CreateInitvmDialog::on_initVMEntry_textChanged(const QString &arg1)
{

	if ( validateEntry(arg1) ) {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	} else {
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

bool CreateInitvmDialog::validateEntry(QString str)
{
	QDir dir(str);

	QString::SectionFlags flags = QString::SectionIncludeLeadingSep | QString::SectionIncludeTrailingSep;
	if ( !QDir( str.section("/", 0, -2, flags) ).exists() ) {
		ui->Information->setText("Parentdirectoy does not exist.");
		ui->ErrorIcon->setPixmap(errorIcon);
		return false;
	}


	if ( dir.exists() ) {
		ui->Information->setText("Directory does already exist. Initvm has to be created in a new directory!");
		ui->ErrorIcon->setPixmap(errorIcon);
		return false;
	} else {
		ui->Information->setText("Create new initvm in directory.");
		ui->ErrorIcon->setPixmap(acceptIcon);
		return true;
	}
}
