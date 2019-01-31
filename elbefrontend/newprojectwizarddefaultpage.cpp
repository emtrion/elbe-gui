#include <QWizardPage>
#include <QWidget>
#include <QObjectList>
#include <QObject>
#include <QDebug>
#include <QLineEdit>
#include "newprojectwizarddefaultpage.h"
#include "newprojectwizard.h"
#include "ui_newprojectwizard.h"

NewProjectWizardDefaultpage::NewProjectWizardDefaultpage()
{
	this->ui_pointer = NULL;
	this->projectSettingInputFields.clear();
}

NewProjectWizardDefaultpage::~NewProjectWizardDefaultpage(){}


void NewProjectWizardDefaultpage::setDefaultPageUiPointer(Ui::NewProjectWizard *ui_pointer)
{
	this->ui_pointer = ui_pointer;
}



void NewProjectWizardDefaultpage::initializePage(int PageId)
{
	if ( PageId == 1) {
		projectSettingInputFields = ui_pointer->MainWidget_2->findChildren<QLineEdit*>();
	}

	if ( PageId == 2 ) {
		projectSettingInputFields = ui_pointer->MainWidget_3->findChildren<QLineEdit*>();
	}

	connectFields();
	displayDefaultSettings();
}

void NewProjectWizardDefaultpage::displayDefaultSettings()
{
	for (int i = 0; i < projectSettingInputFields.size(); i++) {
		projectSettingInputFields.at(i)->setText(defaultSettings.at(i));
		projectSettingInputFields.at(i)->setEnabled(false);
	}
}

void NewProjectWizardDefaultpage::connectFields()
{
	QMutableListIterator<QLineEdit*> i(projectSettingInputFields);
	while (i.hasNext()) {
		connect(i.next(), &QLineEdit::textChanged, this, &NewProjectWizardDefaultpage::completeChanged);
	}

}

bool NewProjectWizardDefaultpage::isComplete() const
{
	bool retVal;

	foreach (QLineEdit* var, projectSettingInputFields) {
		if ( var->text().isEmpty() ) {
			retVal = false;
			break;
		} else {
			retVal = true;
		}
	}
	return retVal;
}
