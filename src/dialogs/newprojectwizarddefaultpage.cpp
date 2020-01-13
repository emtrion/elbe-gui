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

#include "newprojectwizarddefaultpage.h"
#include "ui_newprojectwizard.h"

#include <QDebug>

NewProjectWizardDefaultpage::NewProjectWizardDefaultpage()
{
	this->ui_pointer = NULL;
	this->m_projectSettingInputFields.clear();
}

NewProjectWizardDefaultpage::~NewProjectWizardDefaultpage(){}


void NewProjectWizardDefaultpage::setDefaultPageUiPointer(Ui::NewProjectWizard *ui_pointer)
{
	this->ui_pointer = ui_pointer;
}


void NewProjectWizardDefaultpage::initializePage(int PageId)
{
	if ( PageId == 1) {
		m_projectSettingInputFields = ui_pointer->MainWidget_2->findChildren<QLineEdit*>();
	}

	if ( PageId == 2 ) {
		m_projectSettingInputFields = ui_pointer->MainWidget_3->findChildren<QLineEdit*>();
	}

	connectFields();
	displayDefaultSettings();
}

void NewProjectWizardDefaultpage::displayDefaultSettings()
{
	for (auto i = 0; i < m_projectSettingInputFields.size(); ++i) {
		m_projectSettingInputFields.at(i)->setText(m_defaultSettings.at(i));
		m_projectSettingInputFields.at(i)->setEnabled(false);
	}
}

void NewProjectWizardDefaultpage::connectFields()
{
	QMutableListIterator<QLineEdit*> iter(m_projectSettingInputFields);
	while ( iter.hasNext() ) {
		connect(iter.next(), &QLineEdit::textChanged, this, &NewProjectWizardDefaultpage::completeChanged);
	}

}

bool NewProjectWizardDefaultpage::isComplete() const
{
	bool retVal;
	foreach (QLineEdit* line, m_projectSettingInputFields) {
		if ( line->text().isEmpty() ) {
			retVal = false;
			break;
		} else {
			retVal = true;
		}
	}
	return retVal;
}

QList<QLineEdit *> NewProjectWizardDefaultpage::projectSettingInputFields() const
{
	return m_projectSettingInputFields;
}

QList<QString> NewProjectWizardDefaultpage::defaultSettings() const
{
	return m_defaultSettings;
}

void NewProjectWizardDefaultpage::setDefaultSettings(const QList<QString> &defaultSettings)
{
	m_defaultSettings = defaultSettings;
}
