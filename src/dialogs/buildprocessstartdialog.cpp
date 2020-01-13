#include "buildprocessstartdialog.h"
#include "ui_buildprocessstartdialog.h"

#include <QPushButton>
#include <QDebug>

#include "src/elbe/buildmanager.h"
#include "src/elbe/buildprocess.h"

BuildProcessStartDialog::BuildProcessStartDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BuildProcessStartDialog)
{
	ui->setupUi(this);

	//change accept button text
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Start build");

	ui->ImageCheck->setChecked(true);

	//store all the checkboxes in a list
	checkBoxList = this->findChildren<QCheckBox*>();
}

BuildProcessStartDialog::~BuildProcessStartDialog()
{
	delete ui;
}

void BuildProcessStartDialog::on_buttonBox_accepted()
{
	//store all checked boxes
	foreach (QCheckBox *box, checkBoxList) {
		if ( box->isChecked() ) {
			checkedBoxes.append(box->text());
		}
	}
	//pass the selected output files
	BuildManager *buildmanager = BuildManager::getInstance();
	buildmanager->setOutputFiles(checkedBoxes);
	//start build
	BuildProcess *buildProcess = new BuildProcess();
	buildProcess->startBuild(ui->sourceCdCheck->isChecked(), ui->binCdCheck->isChecked());
}
