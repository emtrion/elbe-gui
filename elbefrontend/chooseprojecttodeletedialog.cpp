#include "chooseprojecttodeletedialog.h"
#include "existingprojects.h"
#include "projecthandler.h"
#include "ui_chooseprojecttodeletedialog.h"

#include "projectlistitem.h"
#include "helpers.h"
#include <QPushButton>
#include <QDebug>

ChooseProjectToDeleteDialog::ChooseProjectToDeleteDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ChooseProjectToDeleteDialog)
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

ChooseProjectToDeleteDialog::~ChooseProjectToDeleteDialog()
{
	delete ui;
}


bool ChooseProjectToDeleteDialog::listIsEmpty()
{
	return ui->projectList->count() == 0;
}

void ChooseProjectToDeleteDialog::addListItem(QString item)
{
	ui->projectList->addItem(item);
}

void ChooseProjectToDeleteDialog::addItems(QList<ProjectListItem*> itemList)
{
	foreach (ProjectListItem *item, itemList) {
		ui->projectList->addItem(item);
	}
}

/********************* SLOTS *************************/

void ChooseProjectToDeleteDialog::on_buttonBox_accepted()
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
