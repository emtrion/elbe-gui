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

#include "codeeditor.h"
#include "qtermwidget5/qtermwidget.h"
#include "newxmldialog.h"
#include "helpers.h"
#include "newprojectwizard.h"
#include "importfiledialog.h"
#include "projectmanager.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add terminal to window
    QTermWidget *console = new QTermWidget(ui->Terminal_Tab);
    console->setColorScheme("DarkPastels");
    console->setContentsMargins(0, 0, 0, 0);
    ui->Terminal_Tab->layout()->addWidget(console);

    //add custom code editor to window
	editor = new CodeEditor();
    ui->Editor->layout()->addWidget(editor);

    //set starting size for upperSection
    ui->central_Splitter->setSizes(QList<int>()<<300<<50);
    ui->UpperSection_Splitter->setSizes(QList<int>()<<50<<200<<40);





	/*----------------------------- add some dummy objects to project structure -------------------------------*/
	/*For testing and further implemtentations of other features related to the project structure.
	 * The Structure represents the current default directory for testing: "/home/hico/elbefrontFilehandlingTestFolder"
	 * It's going to be a project representation only in the future.
	 * Something like this:  #projectname   TODO
	 *							#src
	 *								<xml>
	 *							#out
	 */

	QFileSystemModel *model = new QFileSystemModel;
	model->setRootPath("/home/hico/elbefrontFilehandlingTestFolder");

	ui->ProjektStructure->setModel(model);


	for (int i = 1; i < model->columnCount(); ++i) {
		ui->ProjektStructure->hideColumn(i);
	}

	ui->ProjektStructure->setRootIndex(model->index("/home/hico/elbefrontFilehandlingTestFolder"));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionNew_triggered()
{
	/*for testing will be removed later*/

	ProjectManager *pm = ProjectManager::getInstance();
	pm->update("/home/hico/elbefrontFilehandlingTestFolder/bsp1/.project");
	pm->setProjectOpened(true);
}



void MainWindow::on_ProjektStructure_customContextMenuRequested(const QPoint &pos)
{
	QMenu *menu = new QMenu;
	QModelIndex index = ui->ProjektStructure->currentIndex();
//	QString itemToDelete = QFileSystemModel().filePath(index);
//	qDebug() << "Delete: "+itemToDelete;

//	menu->addAction(QString("Delete"), this, SLOT(helpers::deleteFile(itemToDelete)));
	menu->exec(QCursor::pos());
}




void MainWindow::on_actionNew_Project_triggered()
{
	NewProjectWizard *wiz = new NewProjectWizard();
	wiz->show();
}

void MainWindow::on_actionNew_XML_triggered()
{
	ProjectManager *pm = ProjectManager::getInstance();
	if (pm->getProjectOpened()) {
		NewXMLDialog *xml = new NewXMLDialog();
		xml->show();
	} else {
		qDebug() << "Project has to be \"opend\". ErrorMessage will be implemented later";
		//show error message TODO
	}

}

void MainWindow::displayFileInEditor(QString content)
{
	editor->setPlainText(content);
}

void MainWindow::on_ProjektStructure_doubleClicked(const QModelIndex &index)
{}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionImport_triggered()
{
	ProjectManager *pm = ProjectManager::getInstance();
	if (pm->getProjectOpened()) {
		ImportFileDialog *dialog = new ImportFileDialog();
		dialog->show();
	} else {
		qDebug() << "Project has to be \"opend\". ErrorMessage will be implemented later";
		//show error message TODO
	}
}
