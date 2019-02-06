#include "openprojectdialog.h"
#include "ui_openprojectdialog.h"

#include <QFileDialog>
#include <QDebug>

OpenProjectDialog::OpenProjectDialog(QWidget *parent) :
QWidget(parent),
ui(new Ui::OpenProjectDialog)
{
	ui->setupUi(this);

	QFileDialog *fileChooser = new QFileDialog();
	//	fileChooser->setFileMode(QFileDialog::ExistingFile);
	fileChooser->setDirectory("/home/hico/");
	//	fileChooser->setOption(QFileDialog::DontUseNativeDialog, true);
	//	fileChooser->setFilter(QDir::Hidden);

	QDir::Filters dirFilters = fileChooser->filter();
	dirFilters.setFlag(QDir::Hidden, true);
	fileChooser->setFilter(dirFilters);

	fileChooser->setModal(true);

		QStringList urls;
		if ( fileChooser->exec() ) {
			urls = fileChooser->selectedFiles();
		}
	//	QString url = fileChooser->getOpenFileName(this, "Choose Project");
		qDebug() << urls;
}

OpenProjectDialog::~OpenProjectDialog()
{
	delete ui;
}
