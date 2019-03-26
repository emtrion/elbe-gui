#include "deletedialog.h"
#include "existingprojects.h"
#include "projecthandler.h"
#include "ui_chooseprojecttodeletedialog.h"

#include "projectlistitem.h"
#include "helpers.h"
#include <QPushButton>
#include <QDebug>

DeleteDialog::DeleteDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::DeleteDialog)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Delete");

	existingProjectsHandler = new ExistingsProjects();

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
		qDebug() << "nothing selected";
		this->close();
	} else {
		ProjectListItem *listItem = dynamic_cast<ProjectListItem*>(list.first());
		ProjectHandler *handler = new ProjectHandler();
//		qDebug() << listItem->getProjectPath();
		handler->deleteProject(listItem->getProjectPath());
	}
}
