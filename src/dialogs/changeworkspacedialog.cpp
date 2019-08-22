#include "changeworkspacedialog.h"
#include "ui_changeworkspacedialog.h"

#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>

#include "src/app/helpers.h"
#include <QDebug>

ChangeWorkspaceDialog::ChangeWorkspaceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ChangeWorkspaceDialog)
{
	ui->setupUi(this);

	acceptIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	QSizePolicy sp = ui->ErrorIcon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->ErrorIcon->setSizePolicy(sp);


	defaultEntry = helpers::getHomeDirectoryFromSystem()+"/elbeProjects/";

	appConfig = new ApplicationConfig();
	if  ( appConfig->workspace().compare(helpers::getHomeDirectoryFromSystem()+"/elbeProjects/") == 0) {
		ui->defaultCheckbox->setChecked(true);
		ui->directoryEntry->setEnabled(false);

	} else {
		ui->defaultCheckbox->setChecked(false);
		ui->directoryEntry->setEnabled(true);
	}

	ui->directoryEntry->setText(appConfig->workspace());
}

ChangeWorkspaceDialog::~ChangeWorkspaceDialog()
{
	delete ui;
}



void ChangeWorkspaceDialog::on_buttonBox_accepted()
{
	QDir dir(ui->directoryEntry->text());

	if ( !dir.exists() ) {
		QDir().mkdir(ui->directoryEntry->text());
	}

	appConfig->saveWorkspace(dir.absolutePath());
}

void ChangeWorkspaceDialog::checkIfValid(QString input)
{
	QDir dir(input);
	if ( input.isEmpty() ) {
		ui->ErrorIcon->setPixmap(errorIcon);
		ui->Information->setText("Please enter a valid path");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}

	if ( dir.exists() ) {
		ui->ErrorIcon->setPixmap(acceptIcon);
		ui->Information->setText("Change workspace");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		return;
	}

}


void ChangeWorkspaceDialog::on_directoryEntry_textChanged(const QString &arg1)
{
	checkIfValid(arg1);
}

void ChangeWorkspaceDialog::on_defaultCheckbox_toggled(bool checked)
{
	if ( checked ) {
		ui->directoryEntry->setText(defaultEntry);
		ui->directoryEntry->setEnabled(false);
	} else {
//		ui->directoryEntry->setText("");
		ui->directoryEntry->setEnabled(true);
	}
}


void ChangeWorkspaceDialog::on_browseButton_clicked()
{
	QFileDialog *fileChooser = new QFileDialog();
	fileChooser->setDirectory(helpers::getHomeDirectoryFromSystem());
	fileChooser->setOptions(QFileDialog::ShowDirsOnly);
	fileChooser->setModal(true);

	QString dir = fileChooser->getExistingDirectory();
	ui->directoryEntry->setText(dir+"/");
}
