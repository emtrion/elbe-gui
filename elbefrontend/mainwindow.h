#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "codeeditor.h"


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void displayFileInEditor(QString content);

	CodeEditor *getEditor() const;
	QTextEdit *getMessageLog() const;
	void updateProjectStructure();

	private slots:
	void on_actionNew_triggered();

	void on_ProjektStructure_customContextMenuRequested(const QPoint &pos);

	void on_actionNew_Project_triggered();

	void on_actionNew_XML_triggered();

	void on_ProjektStructure_doubleClicked(const QModelIndex &index);

	void on_actionOpen_triggered();

	void on_actionImport_triggered();

	void on_actionValidate_triggered();

	void on_Editor_textChanged();

	void on_actionClose_triggered();

	private:
    Ui::MainWindow *ui;
//	CodeEditor *editor;
};

#endif // MAINWINDOW_H
