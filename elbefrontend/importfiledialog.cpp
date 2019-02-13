#include "importfiledialog.h"
#include "projectmanager.h"
#include "ui_importfiledialog.h"

#include <QFileDialog>
#include <QDebug>
#include "xmlfilehandler.h"
#include "helpers.h"

ImportFileDialog::ImportFileDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ImportFileDialog)
{
	ui->setupUi(this);

	yesIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Import");

	QSizePolicy sp = ui->SpacingButton->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->SpacingButton->setSizePolicy(sp);

	ui->SpacingButton->hide();

	ProjectManager *pm = ProjectManager::getInstance();
	ui->ProjectEntry->setText(pm->getProjectName());
	srcFolder = pm->getSrcPath();

	updateUI(isValid());

}

ImportFileDialog::~ImportFileDialog()
{
	delete ui;
}


void ImportFileDialog::on_FileNameEntry_textChanged(const QString &arg1)
{
	updateUI(isValid());
}

void ImportFileDialog::updateUI(bool valid)
{
	if ( valid ) {
		ui->Icon->hide();
		ui->Information->setText("Import file");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui->Icon->setPixmap(yesIcon);
		ui->Icon->show();
	} else {
		ui->Icon->setPixmap(errorIcon);
		ui->Icon->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

bool ImportFileDialog::isValid()
{
	if ( ui->FileNameEntry->text().isEmpty() ) {
		ui->Information->setText("Select a file");
		return false;
	} else {
		fileName = ui->FileNameEntry->text();
		return true;
	}
}

void ImportFileDialog::on_buttonBox_accepted()
{
	QString target = srcFolder+fileName;
//	qDebug() << target;
	if ( !QFile().copy(filePath, target) ) {
		qDebug() << "ERROR from "<<__func__<<"Copy failed";
	}
//	qDebug() << "Name: "<<fileName << "Path: "<<filePath << "Target: " << target;
	XmlFileHandler *handler = new XmlFileHandler(target);
	handler->openFile();
}

void ImportFileDialog::chopNameFromPath()
{
	fileName = filePath.section('/', -1);
//	qDebug() << fileName;
}



void ImportFileDialog::on_BrowseButton_clicked()
{
	QFileDialog *fileChooser = new QFileDialog();
	fileChooser->setDirectory("/home/hico/");
	fileChooser->setNameFilter("XML files (*.xml)");
	fileChooser->setModal(true);
	filePath = fileChooser->getOpenFileName();
	ui->FileNameEntry->setText(filePath);
	chopNameFromPath();
}
