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


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add terminal to window
    QTermWidget *console = new QTermWidget(ui->Terminal_Tab);
    console->setColorScheme("DarkPastels");
    console->setContentsMargins(0, 0, 0, 0);
    ui->Terminal_Tab->layout()->addWidget(console);

    //add custom code editor to window
//	editor = new CodeEditor();
//    ui->Editor->layout()->addWidget(editor);

    //set starting size for upperSection

	ui->central_Splitter->setSizes(QList<int>()<<500<<50);
	ui->UpperSection_Splitter->setSizes(QList<int>()<<120<<500<<1);





	/*----------------------------- add some dummy objects to project structure -------------------------------*/
	/*For testing and further implemtentations of other features related to the project structure.
	 * The Structure represents the current default directory for testing: "/home/hico/elbefrontFilehandlingTestFolder"
	 * It's going to be a project representation only in the future.
	 * Something like this:  #projectname   TODO
	 *							#src
	 *								<xml>
	 *							#out
	 */

//	QFileSystemModel *model = new QFileSystemModel;
//	model->setRootPath("/home/hico/elbefrontFilehandlingTestFolder");

//	ui->ProjektStructure->setModel(model);




//	ui->ProjektStructure->setRootIndex(model->index("/home/hico/elbefrontFilehandlingTestFolder"));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionNew_triggered()
{
	/*for testing will be removed later*/

//	ProjectManager *pm = ProjectManager::getInstance();
//	pm->update("/home/hico/elbefrontFilehandlingTestFolder/bsp1/.project");
//	pm->setProjectOpened(true);
}



void MainWindow::on_ProjektStructure_customContextMenuRequested(const QPoint &pos)
{
	QMenu *menu = new QMenu;
	closeAction = new QAction("&Close", menu);
	connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(on_ProjektStructure_ContextMenu_closeAction_triggered()));

	closeAction->setEnabled(false);
	menu->addAction(closeAction);
	menu->exec(QCursor::pos());
}


void MainWindow::on_ProjektStructure_ContextMenu_closeAction_triggered()
{
	qDebug() << "closed";
}



void MainWindow::on_actionNew_Project_triggered()
{
	NewProjectWizard *wiz = new NewProjectWizard();
	wiz->show();
}

void MainWindow::on_actionNew_XML_triggered()
{
	ProjectManager *pm = ProjectManager::getInstance();
	if (pm->isProjectOpened()) {
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
//	QString startURL = "/home/hico/";

	QFileDialog *fileChooser = new QFileDialog();
//	fileChooser->setOption(QFileDialog::ShowDirsOnly, true);

	fileChooser->setDirectory("/home/hico");
	fileChooser->setFilter(QDir::Dirs);

//	QList<QUrl> urls = fileChooser->getOpenFileUrls(this, "Open Project", startURL);
	QString url = fileChooser->getExistingDirectory();
	ProjectHandler *handler = new ProjectHandler();
	QString path = url+"/.project";
	handler->openProject(path);
}

void MainWindow::on_actionImport_triggered()
{
	ProjectManager *pm = ProjectManager::getInstance();
	if (pm->isProjectOpened()) {
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
	ProjectManager *pm = ProjectManager::getInstance();
	if ( pm->isProjectOpened() ) {
	model->clear();
	ui->ProjektStructure->setModel(model);
} else {
	model->setProjectDetails(pm->getProjectDirectory(), pm->getProjectName());

	ui->ProjektStructure->setModel(model);
	ui->ProjektStructure->header()->hide();
}
}
