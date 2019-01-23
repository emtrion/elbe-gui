#include <iostream>
#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "helpers.h"

NewProjectDialog::NewProjectDialog() : QDialog(), ui(new Ui::NewProjectDialog)
{

    ui->setupUi(this);
    ui->dialogButtonBox->button(QDialogButtonBox::Ok)->setText("Create");
	displayDefaultPath();
	ui->ProjectPathBrowsButton->hide();
	ui->Information->setText("Projectname must be specified");
	ui->dialogButtonBox->button(QDialogButtonBox::Ok)->hide();

	//since it's not an option in Qt Designer the retain size must be speciefied here
	QSizePolicy sp = ui->ErrorIcon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->ErrorIcon->setSizePolicy(sp);

	ui->ErrorIcon->show();
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}


void NewProjectDialog::on_DefaultPathButton_toggled(bool checked)
{//checkbox if the defautlpath is to be used or not
    QString tmp = defaultPath;
	if ( checked ) {
        ui->ProjectPathBrowsButton->hide();
		ui->ProjectPathEntry->setEnabled(false);
        tmp.append(ui->ProjectNameEntry->text());
        ui->ProjectPathEntry->setText(tmp);

		if ( !ui->ProjectNameEntry->text().isEmpty() ) {
			ui->dialogButtonBox->button(QDialogButtonBox::Ok)->show();
		}

    } else {
        ui->ProjectPathEntry->clear();
        ui->ProjectPathBrowsButton->show();
		ui->ProjectPathEntry->setEnabled(true);
		ui->dialogButtonBox->button(QDialogButtonBox::Ok)->hide();
    }
}



void NewProjectDialog::on_ProjectNameEntry_textChanged(const QString &arg1)
{//invoked when the content was changed from user OR program
    QString tmp;
	if ( ui->DefaultPathButton->isChecked() ) {
		tmp = defaultPath;
		tmp.append(arg1);
		ui->ProjectPathEntry->setText(tmp);
	}
	validation();
}


void NewProjectDialog::displayDefaultPath()
{

	//defaultpath should only be altered here
//    homeDirectory = helpers::getHomeDirectoryFromSystem();
//    const_cast <QString&> (defaultPath) = homeDirectory.append("/elbeProjects/");
//    ui->ProjectPathEntry->setText(defaultPath);

	//this is the default path for testing only. The correct default path for this application is handled above
	const_cast <QString&> (defaultPath) = "/home/hico/elbefrontFilehandlingTestFolder/";
	ui->ProjectPathEntry->setText(defaultPath);
}

void NewProjectDialog::on_ProjectPathEntry_editingFinished()
{//invoked when the user (and only the user) changes the focus after editing
	QString path = ui->ProjectPathEntry->text();

	if ( path.right(1).compare("/") != 0 ) {
		//check if the first rightmost character is a "/". If not it'll be added to ensure it's a valid directory
		path.append("/");
		ui->ProjectPathEntry->setText(path);
	}
	validation();
}


void NewProjectDialog::on_dialogButtonBox_accepted()
{
	if ( validation() ) {
		newProjectPath = ui->ProjectPathEntry->text();
	}

	if ( !createProject() ) { /*if createProject returns false the project was not created.
								All ressources created prior to the error are removed,
								so the attempt fails completely not leaving any fragments of a project on the filesystem.*/
		QMessageBox messageBox;
		messageBox.critical(0, "Error", "Could not create Project!");
		messageBox.setFixedSize(500, 300);
	}

	std::cout << newProjectPath.toStdString() << std::endl;
}

void NewProjectDialog::on_ProjectPathEntry_textEdited(const QString &arg1)
{ //invoked when the user (and only the user) changes the path
	validation();
}


bool NewProjectDialog::validation()
{
	QString path = ui->ProjectPathEntry->text();
	QString name = ui->ProjectNameEntry->text();

	if ( name.isEmpty() ) {//check if projectname was sepcified
		ui->Information->setText("Projectname must be specified.");
		ui->ErrorIcon->show();
		ui->dialogButtonBox->button(QDialogButtonBox::Ok)->hide();
		return false;
	} else if ( QFile().exists(path+"/src") || QFile().exists(path+"/out") || QFile().exists(path+".project") ) {
		//check if theres already an existing project in the directory
		ui->Information->setText("There is already a project in this directory.");
		ui->dialogButtonBox->button(QDialogButtonBox::Ok)->hide();
		ui->ErrorIcon->show();
		return false;
	} else {//if nothing of the above occured we are good to go
		ui->Information->setText("Create project at selected directory");
		ui->dialogButtonBox->button(QDialogButtonBox::Ok)->show();
		ui->ErrorIcon->hide();
	}

	newProjectPath = path;
	newProjectName = name;

	return true;
}


void NewProjectDialog::on_ProjectPathBrowsButton_clicked()
{//open a file chooser to select directoy

	QFileDialog *fileChooser = new QFileDialog();
	fileChooser->setDirectory("/home/hico/");
	fileChooser->setOptions(QFileDialog::ShowDirsOnly);
	fileChooser->setModal(true);

	QString dir = fileChooser->getExistingDirectory();
	ui->ProjectPathEntry->setText(dir+"/");
}


bool NewProjectDialog::createProject()
{ //creates the directory
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
	if ( !helpers::setProjectMetadata(newProjectName, newProjectPath) ) {//add project specific data to
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
