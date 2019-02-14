#include "newxmldialog.h"
#include "ui_newxmldialog.h"
#include <QPushButton>
#include <QSizePolicy>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>

#include "mainwindow.h"
#include "helpers.h"
#include "xmlfilehandler.h"
#include "projectmanager.h"

NewXMLDialog::NewXMLDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::NewXMLDialog)
{
	ui->setupUi(this);

	yesIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Finish");
	QSizePolicy sp = ui->Icon->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->Icon->setSizePolicy(sp);

	ProjectManager *pm = ProjectManager::getInstance();

	/*for testing only. Shall be removed later*/
//	pm->update("/home/hico/elbefrontFilehandlingTestFolder/bsp1/.project");

	projectPath = pm->getSrcPath();


//	qDebug() << projectPath;
//	qDebug() << pm->getProjectPath();
	ui->ProjectEntry->setText(pm->getProjectName());

	updateUI(isValid());
}

NewXMLDialog::~NewXMLDialog()
{
	delete ui;
}


void NewXMLDialog::updateUI(bool valid)
{
	if ( valid ) {
		ui->Icon->hide();
		ui->Information->setText("Create XML file");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui->Icon->setPixmap(yesIcon);
		ui->Icon->show();
	} else {

		ui->Icon->setPixmap(errorIcon);
		ui->Icon->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

bool NewXMLDialog::isValid()
{
	if ( ui->FileNameEntry->text().isEmpty() ) {
		ui->Information->setText("Select a file name");
		return false;
	} else if ( false/*QDir().exists(src folder path/filename.xml )*/) {//same as below: getOpenProject and check if xml already existsl
		ui->Information->setText("File does already exist");
		return false;
	} else {
		fileName = ui->FileNameEntry->text();
		return true;
	}
}

//void NewXMLDialog::on_ProjectBrowseButton_clicked()
//{
//	QFileDialog *fileChooser = new QFileDialog();
////	fileChooser->setFileMode(QFileDialog::ExistingFile);
//	fileChooser->setDirectory("/home/hico/");
////	fileChooser->setOption(QFileDialog::DontUseNativeDialog, true);
////	fileChooser->setFilter(QDir::Hidden);

//	QDir::Filters dirFilters = fileChooser->filter();
//	dirFilters.setFlag(QDir::Hidden, true);
//	fileChooser->setFilter(dirFilters);

//	fileChooser->setModal(true);

//	QStringList urls;
//	if ( fileChooser->exec() ) {
//		urls = fileChooser->selectedFiles();
//	}
////	QString url = fileChooser->getOpenFileName(this, "Choose Project");
//	qDebug() << urls;

//}


void NewXMLDialog::on_FileNameEntry_textChanged(const QString &arg1)
{
	updateUI(isValid());
}



void NewXMLDialog::on_buttonBox_accepted()
{
	XmlFileHandler *fh = new XmlFileHandler(projectPath, fileName);
	fh->createFile();
//	fh->XMLautoGenerate();
//	fh->openFile();
}


