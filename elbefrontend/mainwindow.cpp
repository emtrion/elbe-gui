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

#include "codeeditor.h"
#include "qtermwidget5/qtermwidget.h"
#include "newprojectdialog.h"
#include "helpers.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add terminal to window
    QTermWidget *console = new QTermWidget(ui->Terminal_Tab);
    console->setColorScheme("DarkPastels");
    console->setContentsMargins(0, 0, 0, 0);
    ui->Terminal_Tab->layout()->addWidget(console);

    //add custom code editor to window
    CodeEditor *editor = new CodeEditor();
    ui->Editor->layout()->addWidget(editor);

    //set starting size for upperSection
    ui->central_Splitter->setSizes(QList<int>()<<300<<50);
    ui->UpperSection_Splitter->setSizes(QList<int>()<<50<<200<<40);





	/*----------------------------- add some dummy objects to project structure -------------------------------*/
	/*For testing and further implemtentations of other features related to the project structure.
	 * The Structure represents the current default directory for testing: "/home/hico/elbefrontFilehandlingTestFolder"
	 * It's going to be a project representation only in the future.
	 * Something like this:  #projectname
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
    //openDialog
    NewProjectDialog *dialog = new NewProjectDialog();
    dialog->show();
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



