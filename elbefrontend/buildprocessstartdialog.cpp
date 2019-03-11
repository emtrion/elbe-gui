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

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Start build"); //change accept button text

	ui->ImageCheck->setChecked(true);

	checkBoxList = this->findChildren<QCheckBox*>(); //store all the checkboxes in a list
}

BuildProcessStartDialog::~BuildProcessStartDialog()
{
	delete ui;
}

void BuildProcessStartDialog::on_buttonBox_accepted()
{
	foreach (QCheckBox *box, checkBoxList) {
		qDebug() << box->text();
		if ( box->isChecked() ) {
			checkedBoxes.append(box->text());
		}
	}

	BuildProcess *buildProcess = new BuildProcess();
	buildProcess->setOutputFiles(checkedBoxes);
	buildProcess->startBuild();
}
