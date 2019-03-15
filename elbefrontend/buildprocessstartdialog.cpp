#include "buildprocess.h"
#include "buildprocessstartdialog.h"
#include "ui_buildprocessstartdialog.h"

#include <QPushButton>
#include <QDebug>

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
//		qDebug() << box->text();
		if ( box->isChecked() ) {
			checkedBoxes.append(box->text());
		}
	}

	BuildProcess *buildProcess = new BuildProcess();
	//pass the selected output files
	buildProcess->setOutputFiles(checkedBoxes);
	//start the build
	buildProcess->startBuild(ui->sourceCdCheck->isChecked(), ui->binCdCheck->isChecked());
}
