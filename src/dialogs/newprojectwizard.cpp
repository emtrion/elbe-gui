#include "newprojectwizard.h"
#include "ui_newprojectwizard.h"

#include <QFileDialog>

#include "src/projects/project.h"
#include "src/projects/projecthandler.h"
#include "src/app/helpers.h"

NewProjectWizard::NewProjectWizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::NewProjectWizard)
{
	ui->setupUi(this);

	displayDefaultPath();
	ui->ProjectPathBrowsButton->hide();
	ui->Information->setText("Projectname must be specified");

	//since it's not an option in Qt Designer, the "retainSize"-option must be set here
	QSizePolicy sp = ui->Icon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->Icon->setSizePolicy(sp);
	ui->Icon->show();

	//pass the ui to wizardpages where the userinput can be validated
	//first page
	ui->newProjectFirstPage->setFirstPageUiPointer(ui);
	ui->newProjectFirstPage->connectSignals();
	//second page
	ui->newProjectSecondPage->setDefaultPageUiPointer(ui);
	//third page
	ui->newProjectThirdPage->setDefaultPageUiPointer(ui);
}

NewProjectWizard::~NewProjectWizard()
{
	delete ui;
}

void NewProjectWizard::displayDefaultPath()
{
	//defaultpath should only be altered here
	QString homeDirectory = helpers::getHomeDirectoryFromSystem();
	const_cast <QString&> (defaultPath) = homeDirectory.append("/elbeProjects/");
	ui->ProjectPathEntry->setText(defaultPath);

	//this is the default path for testing only. The correct default path for this application is handled above
//	const_cast <QString&> (defaultPath) = "/home/hico/elbefrontFilehandlingTestFolder/";
//	ui->ProjectPathEntry->setText(defaultPath);
}

void NewProjectWizard::on_DefaultPathButton_toggled(bool checked)
{
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
{
	QString tmp;
	if ( ui->DefaultPathButton->isChecked() ) {
		tmp = defaultPath;
		tmp.append(arg1);
		ui->ProjectPathEntry->setText(tmp);
	}
}


void NewProjectWizard::on_ProjectPathEntry_editingFinished()
{
	QString path = ui->ProjectPathEntry->text();
	if ( !path.isEmpty() && path.right(1).compare("/") != 0 ) {
		//check if the first rightmost character is a "/". If not it'll be added to ensure it's a valid directory
		path.append("/");
		ui->ProjectPathEntry->setText(path);
	}
}

void NewProjectWizard::on_ProjectPathEntry_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1)

	QString path = ui->ProjectPathEntry->text();
	if ( !path.isEmpty() && path.right(1).compare("/") != 0 ) {
		//check if the first rightmost character is a "/". If not it'll be added to ensure it's a valid directory
		path.append("/");
		ui->ProjectPathEntry->setText(path);
		ui->ProjectPathEntry->cursorBackward(false, 1);
	}
}

void NewProjectWizard::on_ProjectPathBrowsButton_clicked()
{
	QFileDialog *fileChooser = new QFileDialog();
	fileChooser->setDirectory(helpers::getHomeDirectoryFromSystem());
	fileChooser->setOptions(QFileDialog::ShowDirsOnly);
	fileChooser->setModal(true);

	QString dir = fileChooser->getExistingDirectory();
	ui->ProjectPathEntry->setText(dir+"/");

}

void NewProjectWizard::on_NewProjectWizard_accepted()
{
	Project *project = Project::getInstance();
	ProjectProperties *properties = project->newProjectProperties();

	properties->setName(ui->nameEntry->text());
	properties->setVersion(ui->versionEntry->text());
	properties->setDescription(ui->descriptionEntry->text());
	properties->setBuildtype(ui->buildtypeDropDown->currentText());
	properties->setSuite(ui->suiteDropDown->currentText());
	properties->setHost(ui->hostEntry->text());
	properties->setPath(ui->pathEntry->text());
	properties->setProto(ui->protoEntry->text());

	ProjectHandler::createProject(ui->newProjectFirstPage->newProjectPath(), ui->newProjectFirstPage->newProjectName());
}

void NewProjectWizard::on_DefaultSetButton_Page2_toggled(bool checked)
{
	if ( checked ) {
		ui->newProjectSecondPage->displayDefaultSettings();
		ui->buildtypeDropDown->setEnabled(false);
		ui->suiteDropDown->setEnabled(false);
	} else {
		foreach (QLineEdit* le, ui->newProjectSecondPage->projectSettingInputFields()) {
			le->setEnabled(true);
			le->clear();
		}
		ui->buildtypeDropDown->setEnabled(true);
		ui->suiteDropDown->setEnabled(true);
	}
}

void NewProjectWizard::on_DefaultSetButton_Page3_toggled(bool checked)
{
	if ( checked ) {
		ui->newProjectThirdPage->displayDefaultSettings();
	} else {
		foreach (QLineEdit* le, ui->newProjectThirdPage->projectSettingInputFields()) {
			le->setEnabled(true);
			le->clear();
		}
	}
}

void NewProjectWizard::initSecondPageDefault()
{
	QList<QString> list = ui->newProjectSecondPage->defaultSettings();
	list.clear();
	list.append(ui->newProjectFirstPage->newProjectName());
	list.append("1.0");
	list.append("creates an rfs for "+ui->newProjectFirstPage->newProjectName());
	ui->buildtypeDropDown->setCurrentIndex(0);
	ui->suiteDropDown->setCurrentIndex(1);
	ui->newProjectSecondPage->setDefaultSettings(list);
}

void NewProjectWizard::initThirdPageDefault()
{
	QList<QString> list = ui->newProjectThirdPage->defaultSettings();
	list.clear();
	list.append("ftp.de.debian.org");
	list.append("/debian");
	list.append("http");
	ui->newProjectThirdPage->setDefaultSettings(list);
}

void NewProjectWizard::on_NewProjectWizard_currentIdChanged(int id)
{
	if ( id == 1 ) {
		ui->DefaultSetButton_Page2->setChecked(true);
		initSecondPageDefault();
		ui->newProjectSecondPage->initializePage(id);

	} else if ( id == 2 ) {
		ui->DefaultSetButton_Page3->setChecked(true);
		initThirdPageDefault();
		ui->newProjectThirdPage->initializePage(id);
	}
}
