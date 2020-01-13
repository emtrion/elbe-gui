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

#include "newprojectwizardfirstpage.h"

#include <QFile>

#include "ui_newprojectwizard.h"

NewProjectWizardFirstPage::NewProjectWizardFirstPage(){
	this->ui_pointer = NULL;
	acceptIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");
}

NewProjectWizardFirstPage::~NewProjectWizardFirstPage()
{

}

void NewProjectWizardFirstPage::setFirstPageUiPointer(Ui::NewProjectWizard *ui_pointer)
{
	this->ui_pointer = ui_pointer;
}

void NewProjectWizardFirstPage::connectSignals()
{
	connect(ui_pointer->DefaultPathButton, &QCheckBox::toggled, this, &NewProjectWizardFirstPage::completeChanged);
	connect(ui_pointer->ProjectNameEntry, &QLineEdit::textChanged, this, &NewProjectWizardFirstPage::completeChanged);
	connect(ui_pointer->ProjectPathEntry, &QLineEdit::editingFinished, this, &NewProjectWizardFirstPage::completeChanged);
	connect(ui_pointer->ProjectPathEntry, &QLineEdit::textEdited, this, &NewProjectWizardFirstPage::completeChanged);
	connect(ui_pointer->ProjectPathEntry, &QLineEdit::textChanged, this, &NewProjectWizardFirstPage::completeChanged);
}

bool NewProjectWizardFirstPage::isComplete() const
{
	QString path = ui_pointer->ProjectPathEntry->text();
	QString name = ui_pointer->ProjectNameEntry->text();

	if ( name.isEmpty() ) {
		ui_pointer->Information->setText("Projectname must be specified.");
		ui_pointer->Icon->setPixmap(errorIcon);
		return false;
	} else if ( path.isEmpty() ) {
		ui_pointer->Information->setText("Path must be specified");
		ui_pointer->Icon->setPixmap(errorIcon);
		return false;
	} else if ( QFile().exists(path+"/src") || QFile().exists(path+"/out") || QFile().exists(path+"/.project") ) {
		//check if theres already an existing project in the directory
		ui_pointer->Information->setText("There is already a project in this directory.");
		ui_pointer->Icon->setPixmap(errorIcon);
		return false;
	} else {
		//if nothing of the above occured we are good to go
		ui_pointer->Information->setText("Create project at selected directory");
		ui_pointer->Icon->setPixmap(acceptIcon);
	}

	//the only time these are changed
	const_cast <QString&> (m_newProjectPath) = path;
	const_cast <QString&> (m_newProjectName) = name;

	return true;
}

QString NewProjectWizardFirstPage::newProjectName() const
{
	return m_newProjectName;
}

QString NewProjectWizardFirstPage::newProjectPath() const
{
	return m_newProjectPath;
}

