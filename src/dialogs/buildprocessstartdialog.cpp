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

#include "buildprocessstartdialog.h"
#include "ui_buildprocessstartdialog.h"

#include <QPushButton>
#include <QDebug>

#include "src/elbe/buildmanager.h"
#include "src/elbe/buildprocess.h"

BuildProcessStartDialog::BuildProcessStartDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BuildProcessStartDialog)
{
	ui->setupUi(this);

	//change accept button text
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Start build");

	ui->ImageCheck->setChecked(true);

	//store all the checkboxes in a list
	checkBoxList = this->findChildren<QCheckBox*>();
}

BuildProcessStartDialog::~BuildProcessStartDialog()
{
	delete ui;
}

void BuildProcessStartDialog::on_buttonBox_accepted()
{
	//store all checked boxes
	foreach (QCheckBox *box, checkBoxList) {
		if ( box->isChecked() ) {
			checkedBoxes.append(box->text());
		}
	}
	//pass the selected output files
	BuildManager *buildmanager = BuildManager::getInstance();
	buildmanager->setOutputFiles(checkedBoxes);
	//start build
	BuildProcess *buildProcess = new BuildProcess();
	buildProcess->startBuild(ui->sourceCdCheck->isChecked(), ui->binCdCheck->isChecked());
}
