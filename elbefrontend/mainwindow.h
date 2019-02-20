#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "codeeditor.h"
#include "projectitemmodel.h"
#include "projectmanager.h"
#include <QAction>
#include "xmlfilemanager.h"

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
	QAction *getActionClose() const;
	void updateProjectStructure();
	QAction *closeAction;
	void setEditorTabVisible(bool visible);
	void setOpenFileNameLabelText(QString text);
	void enableActionsOnProjectOpen(bool isOpen);
	void enableActionsOnXMLOpen(bool isOpen);
	void updateEditorTabSaveStatus(bool saved);

	void clearProjectStructure();
	void renewProjectStructure();
	public slots:
	void updateCurrentFile(QString path);
	void updateItemModel(QString dir);

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

	void on_EditorClosButton_clicked();

	void on_actionSave_triggered();

	private:
    Ui::MainWindow *ui;
//	CodeEditor *editor;
	ProjectItemModel *model;
	ProjectManager *projectmanager = ProjectManager::getInstance();
	XmlFileManager *filemanager = XmlFileManager::getInstance();
	void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
