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

#include "newxmldialog.h"
#include "ui_newxmldialog.h"

#include <QPushButton>
#include <QDir>

#include "src/xml/xmlfilehandler.h"
#include "src/projects/project.h"

#include <QDebug>

NewXMLDialog::NewXMLDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NewXMLDialog)
{
	ui->setupUi(this);

	acceptIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Finish");
	QSizePolicy sp = ui->Icon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->Icon->setSizePolicy(sp);

	Project *projectmanager = Project::getInstance();
	projectPath = projectmanager->srcPath();

	ui->ProjectEntry->setText(projectmanager->projectName());

	updateUI(isValid());
}

NewXMLDialog::~NewXMLDialog()
{
	delete ui;
}

void NewXMLDialog::updateUI(bool valid)
{
	if ( valid ) {
		ui->Icon->hide();
		ui->Information->setText("Create XML file");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui->Icon->setPixmap(acceptIcon);
		ui->Icon->show();
	} else {
		ui->Icon->setPixmap(errorIcon);
		ui->Icon->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

bool NewXMLDialog::isValid()
{
	QFileInfo file(projectPath +"/"+ ui->FileNameEntry->text());
	if ( ui->FileNameEntry->text().isEmpty() ) {
		ui->Information->setText("Select a file name");
		return false;
	} else if ( file.exists() ) {
		//should not happen because there is only one xml allowed.
		//usefull for later when we can handle more than one file in one project
		ui->Information->setText("File does already exist");
		return false;
	} else {
		fileName = ui->FileNameEntry->text();
		return true;
	}
}

void NewXMLDialog::on_FileNameEntry_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1)
	updateUI(isValid());
}

void NewXMLDialog::on_buttonBox_accepted()
{
	XmlFileHandler::createFile(projectPath, fileName);
}
