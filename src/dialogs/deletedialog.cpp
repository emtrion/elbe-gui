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

#include "deletedialog.h"
#include "ui_deletedialog.h"

#include <QPushButton>
#include <QDebug>

#include "src/projects/existingprojects.h"
#include "src/projects/projecthandler.h"
#include "src/projects/projectlistitem.h"
#include "src/app/helpers.h"


DeleteDialog::DeleteDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DeleteDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Delete");

	existingProjectsHandler = new ExistingProjects();
	addItems(existingProjectsHandler->existingProjects());

	QSizePolicy sp = ui->projectList->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->projectList->setSizePolicy(sp);

	ui->Information->hide();

	if ( listIsEmpty() ) {//if list is empty, there are no existing projects
		ui->projectList->hide();
		ui->Information->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

DeleteDialog::~DeleteDialog()
{
	delete ui;
}


bool DeleteDialog::listIsEmpty()
{
	return ui->projectList->count() == 0;
}

void DeleteDialog::addListItem(QString item)
{
	ui->projectList->addItem(item);
}

void DeleteDialog::addItems(QList<ProjectListItem*> itemList)
{
	foreach (ProjectListItem *item, itemList) {
		ui->projectList->addItem(item);
	}
}

/********************* SLOTS *************************/

void DeleteDialog::on_buttonBox_accepted()
{
	QList<QListWidgetItem*> list = ui->projectList->selectedItems();

	if ( list.isEmpty() ) {
		this->close();
	} else {
		//list contains QListWidgetItems but we need a ProjectListItem to get the projectPath
		ProjectListItem *listItem = dynamic_cast<ProjectListItem*>(list.first());
		ProjectHandler::deleteProject(listItem->projectPath());
	}
}
