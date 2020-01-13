/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <src/update/updatetab.h>

#include "src/projects/project.h"
#include "src/xml/xmlfile.h"

class CodeEditor;
class QTextEdit;
class QAction;
class QMessageBox;
class ProjectItemModel;
class QLabel;

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
		CodeEditor *editor() const;
		QTextEdit *messageLog() const;
		QAction *actionClose() const;
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
		void showNormalStatusOnStatusBar(QString status);
		void changeImportButtonEnabledStatus(bool status);
		void changeElbeActionsEnabledStatus(bool status);
		void changeElbeVersion(const QString &value);
		void closeEditorWindow();
		void showUpdateTab(UpdateTab *utab);
		void hideUpdateTab();
		UpdateTab *getUpdatetab() const;

	public slots:
		void updateCurrentFile(QString path);
		void updateItemModel(QString dir);
		void messageLogAppendText(const QString &str, const QString &colorHexVal);

	private slots:
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
		void on_MessageLog_textChanged();
		void on_actionDownload_files_triggered();
		void on_actionOpen_in_Explorer_triggered();
		void on_actionSettings_triggered();

		void on_actionChange_workspace_triggered();

		void on_actionCreate_InitVM_triggered();

	private:
		Ui::MainWindow *ui;
		ProjectItemModel *model;
		QString elbeVersion;
		QMessageBox *aboutElbeMessageBox;
		QLabel *permStatus;
		UpdateTab *updatetab;

	private:
		Project *projectmanager = Project::getInstance();
		XmlFile *filemanager = XmlFile::getInstance();
		void closeEvent(QCloseEvent *event);
		void showPermStatusOnStatusBar(QString status);
		void initAboutElbeMessageBox();
		bool saveOnClose();
		void rememberBusyProject();
		void rememberOpenedProject(QString project);
};

#endif // MAINWINDOW_H
