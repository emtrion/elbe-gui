#include "chooseprojectdialog.h"
#include "existingprojects.h"
#include "projecthandler.h"
#include "ui_chooseprojectdialog.h"

#include "projectlistitem.h"
#include "helpers.h"
#include <QPushButton>
#include <QDebug>

ChooseProjectDialog::ChooseProjectDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ChooseProjectDialog)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Delete");

	existingProjectsHandler = new ExistingProjects();

	//getLookupList returns a Qlist<ProjectListItem>
	addItems(existingProjectsHandler->getExistingProjects());

	QSizePolicy sp = ui->projectList->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->projectList->setSizePolicy(sp);

	ui->Information->hide();
	if ( listIsEmpty() ) {//if list is empty there are no existing projects
		ui->projectList->hide();
		ui->Information->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

ChooseProjectDialog::~ChooseProjectDialog()
{
	delete ui;
}


bool ChooseProjectDialog::listIsEmpty()
{
	return ui->projectList->count() == 0;
}

void ChooseProjectDialog::addListItem(QString item)
{
	ui->projectList->addItem(item);
}

void ChooseProjectDialog::addItems(QList<ProjectListItem*> itemList)
{
	foreach (ProjectListItem *item, itemList) {
		ui->projectList->addItem(item);
	}
}

/********************* SLOTS *************************/

void ChooseProjectDialog::on_buttonBox_accepted()
{
	QList<QListWidgetItem*> list = ui->projectList->selectedItems();

	if ( list.isEmpty() ) {
		qDebug() << "nothing selected";
		this->close();
	} else {
		ProjectListItem *listItem = dynamic_cast<ProjectListItem*>(list.first());
		ProjectHandler *handler = new ProjectHandler();
//		qDebug() << listItem->getProjectPath();
		handler->deleteProject(listItem->getProjectPath());
	}
}
