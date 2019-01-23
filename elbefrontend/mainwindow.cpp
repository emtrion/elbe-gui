#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QPlainTextEdit>
#include <QColor>
#include <QDebug>
#include <QFileSystemModel>
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





	/*----------------------------- add some dummy objects to tree view -------------------------------*/

	QFileSystemModel *model = new QFileSystemModel;
	model->setRootPath("/home/hico/elbefrontFilehandlingTestFolder");
	ui->ProjektStructur->setModel(model);

	for (int i = 1; i < model->columnCount(); ++i) {
		ui->ProjektStructur->hideColumn(i);
	}

	ui->ProjektStructur->setRootIndex(model->index("/home/hico/elbefrontFilehandlingTestFolder"));

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



void MainWindow::on_ProjektStructur_customContextMenuRequested(const QPoint &pos)
{
	QMenu *menu = new QMenu;
	QModelIndex index = ui->ProjektStructur->currentIndex();
//	QString itemToDelete = QFileSystemModel().filePath(index);
//	qDebug() << "Delete: "+itemToDelete;

//	menu->addAction(QString("Delete"), this, SLOT(helpers::deleteFile(itemToDelete)));
	menu->exec(QCursor::pos());
}



