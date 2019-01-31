#include "newprojectwizard.h"
#include "ui_newprojectwizard.h"
#include <iostream>
#include "helpers.h"

#include <QString>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include "newprojectwizardfirstpage.h"

NewProjectWizard::NewProjectWizard(QWidget *parent) :
QWizard(parent),
ui(new Ui::NewProjectWizard)
{
	ui->setupUi(this);

	displayDefaultPath();
	ui->ProjectPathBrowsButton->hide();
	ui->Information->setText("Projectname must be specified");

	//since it's not an option in Qt Designer, the "retainSize"-option must be set here
	QSizePolicy sp = ui->ErrorIcon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->ErrorIcon->setSizePolicy(sp);
	ui->ErrorIcon->show();

	/*pass the ui to wizardpages where the userinput can be validated*/

	/*first page*/
	ui->newProjectFirstPage->setFirstPageUiPointer(ui);
	ui->newProjectFirstPage->connectSignals();

	/*second page*/

	ui->newProjectSecondPage->setDefaultPageUiPointer(ui);

	/*third page*/

	ui->newProjectThirdPage->setDefaultPageUiPointer(ui);


}


NewProjectWizard::~NewProjectWizard()
{
	delete ui;
}



void NewProjectWizard::displayDefaultPath()
{

	//defaultpath should only be altered here
//    homeDirectory = helpers::getHomeDirectoryFromSystem();
//    const_cast <QString&> (defaultPath) = homeDirectory.append("/elbeProjects/");
//    ui->ProjectPathEntry->setText(defaultPath);

	//this is the default path for testing only. The correct default path for this application is handled above
	const_cast <QString&> (defaultPath) = "/home/hico/elbefrontFilehandlingTestFolder/";
	ui->ProjectPathEntry->setText(defaultPath);
}


void NewProjectWizard::on_DefaultPathButton_toggled(bool checked)
{//checkbox if the defautlpath is to be used or not
	QString tmp = defaultPath;
	if ( checked ) {
		ui->ProjectPathBrowsButton->hide();
		ui->ProjectPathEntry->setEnabled(false);
		tmp.append(ui->ProjectNameEntry->text());
		ui->ProjectPathEntry->setText(tmp);

	} else {
		ui->ProjectPathEntry->clear();
		ui->ProjectPathBrowsButton->show();
		ui->ProjectPathEntry->setEnabled(true);
	}
}



void NewProjectWizard::on_ProjectNameEntry_textChanged(const QString &arg1)
{//invoked when the content was changed from user OR program
	QString tmp;
	if ( ui->DefaultPathButton->isChecked() ) {
		tmp = defaultPath;
		tmp.append(arg1);
		ui->ProjectPathEntry->setText(tmp);
	}
}



void NewProjectWizard::on_ProjectPathEntry_editingFinished()
{//invoked when the user (and only the user) changes the focus after editing

	QString path = ui->ProjectPathEntry->text();

	if ( !path.isEmpty() && path.right(1).compare("/") != 0 ) {
		//check if the first rightmost character is a "/". If not it'll be added to ensure it's a valid directory
		path.append("/");
		ui->ProjectPathEntry->setText(path);
	}
}

void NewProjectWizard::on_ProjectPathBrowsButton_clicked()
{//open a file chooser to select directoy

	QFileDialog *fileChooser = new QFileDialog();
	fileChooser->setDirectory("/home/hico/");
	fileChooser->setOptions(QFileDialog::ShowDirsOnly);
	fileChooser->setModal(true);

	QString dir = fileChooser->getExistingDirectory();
	ui->ProjectPathEntry->setText(dir+"/");

}


void NewProjectWizard::on_NewProjectWizard_accepted()
{
	newProjectSettings.name = ui->nameEntry->text();
	newProjectSettings.version = ui->versionEntry->text();
	newProjectSettings.description = ui->descriptionEntry->text();
	newProjectSettings.buildtype = ui->buildtypeDropDown->currentText();
	newProjectSettings.suite = ui->suiteDropDown->currentText();
	newProjectSettings.host = ui->hostEntry->text();
	newProjectSettings.path = ui->pathEntry->text();
	newProjectSettings.proto = ui->protoEntry->text();

	createProject();
}



bool NewProjectWizard::createProject()
{ //creates the directory
	QString newProjectPath = ui->newProjectFirstPage->getNewProjectPath();
	QString newProjectName = ui->newProjectFirstPage->getNewProjectName();

	if ( QDir().exists(newProjectPath) ) {
		//no need to create an existing project
	} else {
		if ( !QDir().mkpath(newProjectPath) ) {//create directory and all parentdirectories necessary
			//returns false when it couldn't create the directory
			qDebug() << "problem while creating project directory";
			return false;
		}
	}

	QFile::copy(":/projectconfig.xml", newProjectPath+"/.project"); //copy conf-file template to directory
	QFile confFile(newProjectPath+"/.project");
	if ( !helpers::setProjectMetadata(newProjectName, newProjectPath, newProjectSettings) ) {//add project specific data to
		confFile.remove(); //if an error occurs while creating ".project", it will be removed from the directory
		qDebug() << "problem while creating config file";
		return false;
	}

	if ( !QDir().mkpath(newProjectPath+"/src/") || !QDir().mkpath(newProjectPath+"/out/") ) {
		//create directory for the elbe-XML file and the output from elbe
		confFile.remove();//if an error occurs while creating "src" and "out", ".project" has to be removed
		qDebug() << "problem while creating source and output directories";
		return false;
	}

	return true;
}

void NewProjectWizard::on_ProjectPathEntry_textChanged(const QString &arg1)
{

	QString path = ui->ProjectPathEntry->text();

	if ( !path.isEmpty() && path.right(1).compare("/") != 0 ) {
		//check if the first rightmost character is a "/". If not it'll be added to ensure it's a valid directory
		path.append("/");
		ui->ProjectPathEntry->setText(path);
		ui->ProjectPathEntry->cursorBackward(false, 1);
	}
}

void NewProjectWizard::on_DefaultSetButton_1_toggled(bool checked)
{
	if (checked) {
		ui->newProjectSecondPage->displayDefaultSettings();
		ui->buildtypeDropDown->setEnabled(false);
		ui->suiteDropDown->setEnabled(false);
	} else {
		foreach (QLineEdit* le, ui->newProjectSecondPage->projectSettingInputFields) {
			le->setEnabled(true);
			le->clear();
		}
		ui->buildtypeDropDown->setEnabled(true);
		ui->suiteDropDown->setEnabled(true);
	}
}

void NewProjectWizard::on_DefaultSetButton_2_toggled(bool checked)
{
	if (checked) {
		ui->newProjectThirdPage->displayDefaultSettings();
	} else {
		foreach (QLineEdit* le, ui->newProjectThirdPage->projectSettingInputFields) {
			le->setEnabled(true);
			le->clear();
		}
	}
}


void NewProjectWizard::initSecondPageDefault()
{//initialize default entries for second page
	QList<QString> tmp = ui->newProjectSecondPage->defaultSettings;
	tmp.clear();
	tmp.append(ui->newProjectFirstPage->getNewProjectName());
	tmp.append("1.0");
	tmp.append("creates an nfsroot for "+ui->newProjectFirstPage->getNewProjectName());
	ui->buildtypeDropDown->setCurrentIndex(0);
	ui->suiteDropDown->setCurrentIndex(1);
	ui->newProjectSecondPage->defaultSettings = tmp;
}

void NewProjectWizard::initThirdPageDefault()
{//initialize default entries for third page
	QList<QString> tmp = ui->newProjectThirdPage->defaultSettings;
	tmp.clear();
	tmp.append("ftp.de.debian.org");
	tmp.append("/debian");
	tmp.append("http");
	ui->newProjectThirdPage->defaultSettings = tmp;

}

void NewProjectWizard::on_NewProjectWizard_currentIdChanged(int id)
{//invoked when the page in the wizard changes
//	qDebug() << "PageID: "<<id;

	if (id == 1) {
		ui->DefaultSetButton_1->setChecked(true);
		initSecondPageDefault();
		ui->newProjectSecondPage->initializePage(id);

	} else if (id == 2) {
		ui->DefaultSetButton_2->setChecked(true);
		initThirdPageDefault();
		ui->newProjectThirdPage->initializePage(id);
	}
}

void NewProjectWizard::on_buildtypeDropDown_currentIndexChanged(const QString &arg1)
{//invoked when the selection in the dropdown is changed
	this->newProjectSettings.buildtype = arg1;
}


void NewProjectWizard::on_suiteDropDown_currentIndexChanged(const QString &arg1)
{//invoked when the selection in the dropdown is changed
	this->newProjectSettings.suite = arg1;
}
