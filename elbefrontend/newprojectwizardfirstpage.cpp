#include <QString>
#include <QFile>
#include "newprojectwizard.h"
#include "newprojectwizardfirstpage.h"
#include "ui_newprojectwizard.h"

NewProjectWizardFirstPage::NewProjectWizardFirstPage(){
	this->ui_pointer = NULL;
}


NewProjectWizardFirstPage::~NewProjectWizardFirstPage(){}

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


	if ( name.isEmpty() ) {//check if projectname was sepcified
		ui_pointer->Information->setText("Projectname must be specified.");
		ui_pointer->ErrorIcon->show();
		return false;
	} else if ( path.isEmpty() ) {
		ui_pointer->Information->setText("Path must be specified");
		ui_pointer->ErrorIcon->show();
		return false;
	} else if ( QFile().exists(path+"/src") || QFile().exists(path+"/out") || QFile().exists(path+"/.project") ) {
		//check if theres already an existing project in the directory
		ui_pointer->Information->setText("There is already a project in this directory.");
		ui_pointer->ErrorIcon->show();
		return false;
	} else {//if nothing of the above occured we are good to go
		ui_pointer->Information->setText("Create project at selected directory");
		ui_pointer->ErrorIcon->hide();
	}

	/*the only time these should be changed*/
	const_cast <QString&> (newProjectPath) = path;
	const_cast <QString&> (newProjectName) = name;

	return true;
}

QString NewProjectWizardFirstPage::getNewProjectName() const
{
	return newProjectName;
}

QString NewProjectWizardFirstPage::getNewProjectPath() const
{
	return newProjectPath;
}

