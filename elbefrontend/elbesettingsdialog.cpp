#include "applicationconfig.h"
#include "elbehandler.h"
#include "elbesettingsdialog.h"
#include "ui_elbesettingsdialog.h"
#include "buildmanager.h"

#include <QPushButton>
#include <QDebug>


ElbeSettingsDialog::ElbeSettingsDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ElbeSettingsDialog)
{
	ui->setupUi(this);

	yesIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	QSizePolicy sp = ui->ErrorIcon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->ErrorIcon->setSizePolicy(sp);


	appConfig = new ApplicationConfig();

	qDebug() << appConfig->elbeExe();

	if ( appConfig->elbeExe().isEmpty() || appConfig->elbeExe().compare("default") == 0 ) {
		on_defaultCheckbox_toggled(true);
	} else {
		ui->defaultCheckbox->setChecked(false);
		ui->elbeEntry->setText(appConfig->elbeExe());
	}

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

	auto elbe = new ElbeHandler();
	auto buildmanager = BuildManager::getInstance();

	if ( ui->defaultCheckbox->isChecked() ) {
		appConfig->saveElbe("default");
		buildmanager->setElbeWorkingDir("default");
		buildmanager->setElbeCommandPrefix("");
	} else {
		appConfig->saveElbe( ui->elbeEntry->text() );
		appConfig->saveInitVM(ui->initVMEntry->text());
		buildmanager->setElbeWorkingDir(ui->elbeEntry->text());
		buildmanager->setElbeCommandPrefix("./");
	}


	if ( oldInitVM.compare(newInitVM) != 0 ) {
		elbe->restartInitVM(newInitVM);
	} else {
		qDebug() << "no changes on init therefore no restart necessary";
	}

	elbe->isVersionSupported();
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
	QFileInfo info(elbe);
	if ( !ui->defaultCheckbox->isChecked() ) {
		if ( info.exists() && info.isFile() && info.isExecutable() && info.fileName().compare("elbe") == 0 ) {
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
		case Icon::ACCEPT: ui->ErrorIcon->setPixmap(yesIcon);
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


