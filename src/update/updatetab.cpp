#include "updatetab.h"
#include "ui_updatetab.h"
#include "updates.h"
#include "updatelistitem.h"

#include <QDebug>

UpdateTab::UpdateTab(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpdateTab)
{
	ui->setupUi(this);

	ui->versionSelectBox->addItem("Base-Version");
}

UpdateTab::~UpdateTab()
{
	delete ui;
}

//bit of a name problem
//updateView updates the view
void UpdateTab::updateView(Updates *updates)
{
	this->updates = updates;
	/*openProject makes new instance of updates where the update history is loaded. Then openProject calls updateView where the instance of updates is set and
	 *the view gets updated so that the new project is shown
	*/
	connect(this->updates, SIGNAL(notifyViewAfterCheckUpdates(UpdateListItem)), this, SLOT(refreshUpdateInfo(UpdateListItem)));
	connect(this->updates, SIGNAL(notifyViewAfterBuildUpdates(UpdateListItem)), this, SLOT(appendLine(UpdateListItem)));
}

void UpdateTab::appendLine(UpdateListItem item)
{
	if ( !ui->checkUpdateButton->isEnabled() && !ui->doUpdateButton->isEnabled()) {
		enableButtons();
	}

	ui->updateList->addItem(item.toString());
}

void UpdateTab::on_checkUpdateButton_clicked()
{
	if ( ui->updateList->count() <= 0) {
		return;
	}

	updates->checkForUpdates();
}

void UpdateTab::on_versionSelectBox_currentIndexChanged(int index)
{

}

void UpdateTab::on_doUpdateButton_clicked()
{
	updates->makeUpdate();
}

void UpdateTab::refreshUpdateInfo(UpdateListItem item)
{
	ui->updateList->takeItem(ui->updateList->count()-1);
	ui->updateList->addItem(item.toString());
}

void UpdateTab::enableButtons()
{
	ui->checkUpdateButton->setEnabled(true);
	ui->doUpdateButton->setEnabled(true);
}
