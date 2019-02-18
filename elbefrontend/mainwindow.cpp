#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
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


void MainWindow::on_ProjektStructure_ContextMenu_closeAction_triggered()
{
//	QModelIndex index = ui->ProjektStructure->currentIndex();
//	QString itemPath = model->getItemPath(index);
//	QFileInfo item(itemPath);
//	if (item.isFile()) {
//		XmlFileHandler *handler = new XmlFileHandler(itemPath);
//		handler->closeFile();
//	} else {
//		return;
//	}

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
		//show error message TODO
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
	ui->Editor->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void MainWindow::on_actionClose_triggered()
{
	ProjectHandler *handler = new ProjectHandler();
	handler->closeProject();
}

void MainWindow::updateProjectStructure()
{
	model = new ProjectItemModel();

	if (  projectmanager->isProjectOpened() ) {
		model->clear();
		ui->ProjektStructure->setModel(model);
	} else {
		model->setProjectDetails( projectmanager->getProjectDirectory(),  projectmanager->getProjectName());

		ui->ProjektStructure->setModel(model);
		ui->ProjektStructure->header()->hide();
	}
}

void MainWindow::setEditorTabVisible(bool visible)
{
	if ( visible) {
		ui->OpenFileNameLabel->show();
		ui->EditorClosButton->show();
	} else {
		ui->OpenFileNameLabel->hide();
		ui->EditorClosButton->hide();
	}
}


void MainWindow::setOpenFileNameLabelText(QString text)
{
	ui->OpenFileNameLabel->setText(text);
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
