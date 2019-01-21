#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QPlainTextEdit>
#include <QColor>
#include "codeeditor.h"
#include "qtermwidget5/qtermwidget.h"
#include "newprojectdialog.h"

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
