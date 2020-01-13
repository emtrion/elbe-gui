#include "importfiledialog.h"
#include "ui_importfiledialog.h"

#include <QFileDialog>
#include <QDebug>

#include "src/projects/project.h"
#include "src/xml/xmlfilehandler.h"
#include "src/app/helpers.h"

ImportFileDialog::ImportFileDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ImportFileDialog)
{
	ui->setupUi(this);

	acceptIcon = QPixmap(":/qss_icons/rc/YesIcon.png");
	errorIcon = QPixmap(":/qss_icons/rc/ErrorIcon.png");

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Import");

	QSizePolicy sp = ui->SpacingButton->sizePolicy();
	sp.setRetainSizeWhenHidden(true);
	ui->SpacingButton->setSizePolicy(sp);
	ui->SpacingButton->hide();

	Project *project = Project::getInstance();
	ui->ProjectEntry->setText(project->projectName());
	srcFolder = project->srcPath();

	updateUI(isValid());
}

ImportFileDialog::~ImportFileDialog()
{
	delete ui;
}


void ImportFileDialog::on_FilePathEntry_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1)
	updateUI(isValid());
}

void ImportFileDialog::updateUI(bool valid)
{
	if ( valid ) {
		ui->Icon->hide();
		ui->Information->setText("Import file");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui->Icon->setPixmap(acceptIcon);
		ui->Icon->show();
	} else {
		ui->Icon->setPixmap(errorIcon);
		ui->Icon->show();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
}

bool ImportFileDialog::isValid()
{
	QFileInfo file(ui->FilePathEntry->text());
	if ( ui->FilePathEntry->text().isEmpty() ) {
		ui->Information->setText("Select a file");
		return false;
	} else if ( !file.fileName().endsWith(".xml") ) {
		ui->Information->setText("Selected file is not a XML");
		return false;
	} else {
		filePath = ui->FilePathEntry->text();
		return true;
	}
}

void ImportFileDialog::on_buttonBox_accepted()
{
	//chop the filename to name the new file
	QString fileName = filePath.section('/', -1);
	QString target = srcFolder+fileName;
	if ( !QFile().copy(filePath, target) ) {
		qDebug() << "ERROR from "<<__func__<<"Copy failed";
	}
	XmlFileHandler::openFile(target);
}

void ImportFileDialog::on_BrowseButton_clicked()
{
	QFileDialog *fileChooser = new QFileDialog();
	fileChooser->setDirectory(helpers::getHomeDirectoryFromSystem());
	fileChooser->setNameFilter("XML files (*.xml)");
	fileChooser->setModal(true);
	filePath = fileChooser->getOpenFileName();
	ui->FilePathEntry->setText(filePath);
}
