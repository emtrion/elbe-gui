#include "elbesettingsdialog.h"
#include "ui_elbesettingsdialog.h"

#include <QPushButton>
#include <QDebug>

#include "src/app/applicationconfig.h"
#include "src/elbe/elbehandler.h"
#include "src/elbe/buildmanager.h"
#include "src/mainwindow/mainwindow.h"
#include "src/app/helpers.h"

ElbeSettingsDialog::ElbeSettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ElbeSettingsDialog)
{
	ui->setupUi(this);

	acceptIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	QSizePolicy sp = ui->ErrorIcon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->ErrorIcon->setSizePolicy(sp);

	appConfig = new ApplicationConfig();
	//the current settings are shown in the entryfiels
	if ( appConfig->elbeExe().isEmpty() || appConfig->elbeExe().compare("default") == 0 ) {
		on_defaultCheckbox_toggled(true);
	} else {
		ui->defaultCheckbox->setChecked(false);
		ui->elbeEntry->setText(appConfig->elbeExe());
	}
	//remember the current initVM to compare it later
	oldInitVM = appConfig->initVM();
	ui->initVMEntry->setText(oldInitVM);
}
ElbeSettingsDialog::~ElbeSettingsDialog()
{
	delete ui;
}

void ElbeSettingsDialog::on_defaultCheckbox_toggled(bool checked)
{
	if ( checked ) {
		ui->elbeEntry->setEnabled(false);
		ui->elbeEntry->setText("installed on system");
	} else {
		ui->elbeEntry->setEnabled(true);
		ui->elbeEntry->setText("");
	}
}

void ElbeSettingsDialog::on_buttonBox_accepted()
{
	QString newInitVM = ui->initVMEntry->text();
	auto buildmanager = BuildManager::getInstance();

	if ( ui->defaultCheckbox->isChecked() ) {
		appConfig->saveElbe("default");
		//setElbeWorkingDir() changes "default" to system home directory
		buildmanager->setElbeWorkingDir("default");
		buildmanager->setElbeCommandPrefix("");
	} else {
		appConfig->saveElbe( ui->elbeEntry->text() );
		appConfig->saveInitVM(ui->initVMEntry->text());
		buildmanager->setElbeWorkingDir(ui->elbeEntry->text());
		buildmanager->setElbeCommandPrefix("./");
	}

	if ( oldInitVM.compare(newInitVM) != 0 ) {
		ElbeHandler::restartInitVM(newInitVM);
	} else {
		//no changes, therefor no action necessary
	}

	ElbeHandler::isVersionSupported();
	MainWindow *mainwindow = helpers::getMainWindow();
	//update the statusBar and the information window "about elbe"
	mainwindow->changeElbeVersion(ElbeHandler::checkElbeVersion());
}

void ElbeSettingsDialog::on_elbeEntry_textChanged(const QString &arg1)
{
	updateButtonBox(fieldsFilled() && validElbe(arg1));
}

void ElbeSettingsDialog::on_initVMEntry_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1)
	updateButtonBox(fieldsFilled());
}

bool ElbeSettingsDialog::validElbe(const QString &elbe)
{
	QFileInfo file(elbe);
	if ( !ui->defaultCheckbox->isChecked() ) {
		//checks whether the path directs to a valid elbe exectuable
		if ( file.exists() && file.isFile() && file.isExecutable() && file.fileName().compare("elbe") == 0 ) {
			updateInformation(Icon::ACCEPT, "specify elbe and initVM for all projects");
			return true;
		} else {
			updateInformation(Icon::DECLINE, "Is not a valid elbe executable");
			return false;
		}
	} else {
		updateInformation(Icon::ACCEPT, "specify elbe and initVM for all projects");
		return true;
	}
}

bool ElbeSettingsDialog::fieldsFilled()
{
	if ( ui->elbeEntry->text().isEmpty() || ui->initVMEntry->text().isEmpty() ) {
		updateInformation(Icon::DECLINE, "Both lines must be filled");
		return false;
	} else {
		updateInformation(Icon::ACCEPT, "specify elbe and initVM for all projects");
		return true;
	}
}

void ElbeSettingsDialog::updateInformation(Icon value, const QString &text)
{
	switch (value) {
		case Icon::DECLINE: ui->ErrorIcon->setPixmap(errorIcon);
			break;
		case Icon::ACCEPT: ui->ErrorIcon->setPixmap(acceptIcon);
			break;
		default:
			//should not be reached
			break;
	}
	ui->Information->setText(text);
}

void ElbeSettingsDialog::updateButtonBox(const bool status)
{
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(status);
}


