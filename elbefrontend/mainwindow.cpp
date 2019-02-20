#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QWidget>
#include <QPlainTextEdit>
#include <QColor>
#include <QDebug>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QErrorMessage>
#include <QFileDialog>
#include <QCheckBox>

#include "codeeditor.h"
#include "qtermwidget5/qtermwidget.h"
#include "newxmldialog.h"
#include "helpers.h"
#include "newprojectwizard.h"
#include "importfiledialog.h"
#include "projectmanager.h"
#include "schemavalidation.h"
#include "projecthandler.h"
#include "xmlfilehandler.h"
#include "openprojectfiledialog.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add terminal to window
    QTermWidget *console = new QTermWidget(ui->Terminal_Tab);
    console->setColorScheme("DarkPastels");
    console->setContentsMargins(0, 0, 0, 0);
    ui->Terminal_Tab->layout()->addWidget(console);

    //set starting size for upperSection
	ui->central_Splitter->setSizes(QList<int>()<<500<<50);
	ui->UpperSection_Splitter->setSizes(QList<int>()<<120<<500<<1);

	ui->Editor->setLineNumberAreaVisible(false);
	setEditorTabVisible(false);

	helpers::initSystemWatcher();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
	/*for testing will be removed later*/

//
//	 projectmanager->update("/home/hico/elbefrontFilehandlingTestFolder/bsp1/.project");
//	 projectmanager->setProjectOpened(true);
}



void MainWindow::on_ProjektStructure_customContextMenuRequested(const QPoint &pos)
{
	Q_UNUSED(pos)
//	QMenu *menu = new QMenu;
//	closeAction = new QAction("&Close", menu);
//	connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(on_ProjektStructure_ContextMenu_closeAction_triggered()));


//	closeAction->setEnabled(false);


//	menu->addAction(closeAction);
//	menu->exec(QCursor::pos());
}




void MainWindow::on_actionNew_Project_triggered()
{
	NewProjectWizard *wiz = new NewProjectWizard();
	wiz->show();
}

void MainWindow::on_actionNew_XML_triggered()
{

	if ( projectmanager->isProjectOpened()) {
		NewXMLDialog *xml = new NewXMLDialog();
		xml->show();
	} else {
		qDebug() << "Project has to be \"opend\". ErrorMessage will be implemented later";
		//show error message TODO
		QMessageBox *msgBox = new QMessageBox(this);
		msgBox->setIcon(QMessageBox::Critical);
		msgBox->setText("A project has to be opened!");
		msgBox->exec();
	}

}

void MainWindow::displayFileInEditor(QString content)
{
	ui->Editor->setPlainText(content);
}

void MainWindow::on_ProjektStructure_doubleClicked(const QModelIndex &index)
{
	QString path = model->getItemPath(index);
	QFileInfo item(path);
	if ( item.isFile() ) {
		XmlFileHandler *handler = new XmlFileHandler(model->getItemPath(index));
		handler->openFile();
	} else {
		return;
	}

}


void MainWindow::on_actionOpen_triggered()
{
	OpenProjectFileDialog *fileChooser = new OpenProjectFileDialog(this, "/home/hico");
	if ( fileChooser->exec() == QDialog::Accepted ) {
		ProjectHandler *handler = new ProjectHandler();
		handler->openProject(fileChooser->selectedUrls().value(0).toLocalFile()+"/.project");

//		qDebug() << fileChooser->selectedUrls().value(0).toLocalFile()+"/.project";
	}
}

void MainWindow::on_actionImport_triggered()
{

	if ( projectmanager->isProjectOpened()) {
		ImportFileDialog *dialog = new ImportFileDialog();
		dialog->show();
	} else {
		qDebug() << "Project has to be \"opend\". ErrorMessage will be implemented later";
		QMessageBox *msgBox = new QMessageBox(this);
		msgBox->setIcon(QMessageBox::Critical);
		msgBox->setText("A project has to be opened!");
		msgBox->exec();
	}
}

void MainWindow::on_actionValidate_triggered()
{
	SchemaValidation *sv = new SchemaValidation(ui->Editor->toPlainText());
	sv->validate();
}



CodeEditor *MainWindow::getEditor() const
{
	return ui->Editor;
}

QTextEdit *MainWindow::getMessageLog() const
{
	return ui->MessageLog;
}

QAction *MainWindow::getActionClose() const
{
	return ui->actionClose;
}



void MainWindow::on_Editor_textChanged()
{
	filemanager->setIsSaved(false);
	ui->Editor->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void MainWindow::on_actionClose_triggered()
{
	ProjectHandler *handler = new ProjectHandler();
	handler->closeProject();
}

void MainWindow::clearProjectStructure()
{
	model = new ProjectItemModel();
	model->clear();
	ui->ProjektStructure->setModel(model);

}

void MainWindow::renewProjectStructure()
{
	model = new ProjectItemModel();
	model->setProjectDetails( projectmanager->getProjectDirectory(),  projectmanager->getProjectName());
	ui->ProjektStructure->setModel(model);
	ui->ProjektStructure->header()->hide();
}

void MainWindow::updateProjectStructure()
{
	if ( projectmanager->isProjectOpened() ) {
		clearProjectStructure();
	} else {
		renewProjectStructure();
	}
}

void MainWindow::setEditorTabVisible(bool visible)
{
	if ( visible ) {
		ui->OpenFileNameLabel->show();
		ui->EditorClosButton->show();
	} else {
		ui->OpenFileNameLabel->hide();
		ui->EditorClosButton->hide();
	}
}

void MainWindow::updateEditorTabSaveStatus(bool saved)
{
	if ( saved ) {
		ui->OpenFileNameLabel->setText(filemanager->getCurrentFileName());
	} else {
		ui->OpenFileNameLabel->setText(filemanager->getCurrentFileName()+"*");
	}
}

void MainWindow::setOpenFileNameLabelText(QString text)
{
	ui->OpenFileNameLabel->setText(text);
}

void MainWindow::enableActionsOnProjectOpen(bool isOpen)
{
	ui->actionClose->setEnabled(isOpen);
	ui->actionImport->setEnabled(isOpen);
	ui->actionNew_XML->setEnabled(isOpen);
}

void MainWindow::enableActionsOnXMLOpen(bool isOpen)
{
	ui->actionSave->setEnabled(isOpen);
	ui->actionSave_As->setEnabled(isOpen);
	ui->actionValidate->setEnabled(isOpen);
}



void MainWindow::on_EditorClosButton_clicked()
{
	XmlFileHandler *handler = new XmlFileHandler();
	handler->closeFile();
}

void MainWindow::on_actionSave_triggered()
{
	qDebug() << __func__;
	XmlFileHandler *handler = new XmlFileHandler();
	handler->saveFile();
}

void MainWindow::updateItemModel(QString dir)
{
	Q_UNUSED(dir);
	qDebug() << "signal received in "<<__func__;

	ProjectManager *pm = ProjectManager::getInstance();
	pm->setProjectModified(true); //tell the projectmanager that current project has been modified

	MainWindow *mw = helpers::getMainWindow();
	mw->renewProjectStructure();
}

void MainWindow::updateCurrentFile(QString path)
{
	qDebug() << "signal received in "<<__func__;
	XmlFileHandler *handler = new XmlFileHandler();
	handler->handleFileModification(path);
}
