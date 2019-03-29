#include "deletedialog.h"
#include "ui_chooseprojecttodeletedialog.h"

#include <QPushButton>
#include <QDebug>

#include "existingprojects.h"
#include "projecthandler.h"
#include "projectlistitem.h"
#include "helpers.h"


DeleteDialog::DeleteDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DeleteDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Delete");

	existingProjectsHandler = new ExistingProjects();
	addItems(existingProjectsHandler->existingProjects());

	QSizePolicy sp = ui->projectList->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->projectList->setSizePolicy(sp);

	ui->Information->hide();

	if ( listIsEmpty() ) {//if list is empty, there are no existing projects
		ui->projectList->hide();
		ui->Information->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

DeleteDialog::~DeleteDialog()
{
	delete ui;
}


bool DeleteDialog::listIsEmpty()
{
	return ui->projectList->count() == 0;
}

void DeleteDialog::addListItem(QString item)
{
	ui->projectList->addItem(item);
}

void DeleteDialog::addItems(QList<ProjectListItem*> itemList)
{
	foreach (ProjectListItem *item, itemList) {
		ui->projectList->addItem(item);
	}
}

/********************* SLOTS *************************/

void DeleteDialog::on_buttonBox_accepted()
{
	QList<QListWidgetItem*> list = ui->projectList->selectedItems();

	if ( list.isEmpty() ) {
		this->close();
	} else {
		//list contains QListWidgetItems but we need a ProjectListItem to get the projectPath
		ProjectListItem *listItem = dynamic_cast<ProjectListItem*>(list.first());
		ProjectHandler::deleteProject(listItem->projectPath());
	}
}
