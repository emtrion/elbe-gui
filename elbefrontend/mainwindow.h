#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMessageBox>

#include "codeeditor.h"
#include "projectitemmodel.h"
#include "projectmanager.h"
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

	void setEditorTabVisible(bool visible);
	void setOpenFileNameLabelText(QString text);
	void enableActionsOnProjectOpen(bool isOpen);
	void enableActionsOnXMLOpen(bool isOpen);
	void updateEditorTabSaveStatus(bool saved);

	void clearProjectStructure();
	void renewProjectStructure();
	void showElbeVersion(QString version);

	void setElbeVersion(const QString &version);
	void showTempStatusOnStatusBar(QString status);
	void changeNewXmlButtonEnabledStatus(bool status);
	void setStatusBarTextColor(QColor color);
	void showNormalStatusOnStatusBar(QString status);

	void changeImportButtonEnabledStatus(bool status);
	public slots:
	void updateCurrentFile(QString path);
	void updateItemModel(QString dir);
	void messageLogAppendText(const QString &str, const QString &colorHexVal);

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

	void on_EditorCloseButton_clicked();

	void on_actionSave_triggered();

	void on_actionAbout_triggered();

	void on_actionAbout_elbe_triggered();

	void on_actionDelete_triggered();

	void on_actionBuild_triggered();

	void on_ProjektStructure_ContextMenu_deleteAction_triggered();

	void on_MessageLog_textChanged();

	void on_actionDownload_files_triggered();

	private:
    Ui::MainWindow *ui;
//	CodeEditor *editor;
	ProjectItemModel *model;
	ProjectManager *projectmanager = ProjectManager::getInstance();
	XmlFileManager *filemanager = XmlFileManager::getInstance();
	void closeEvent(QCloseEvent *event);

	void showPermStatusOnStatusBar(QString status);

	QString elbeVersion;
	QMessageBox *aboutElbeMessageBox;
	void initAboutElbeMessageBox();
	bool saveOnClose();
//	int showCloseError(const QString &text, const QString &informativeText, QMessageBox::StandardButtons buttons, QMessageBox::Button defaultButton);
	void handleCloseDuringBuild();
};

#endif // MAINWINDOW_H
