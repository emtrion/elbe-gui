#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QMessageBox>

#include "src/dialogs/newxmldialog.h"
#include "src/dialogs/newprojectwizard.h"
#include "src/dialogs/importfiledialog.h"
#include "src/xml-validation/schemavalidation.h"
#include "src/projects/projecthandler.h"
#include "src/dialogs/openprojectfiledialog.h"
#include "src/dialogs/deletedialog.h"
#include "src/dialogs/buildprocessstartdialog.h"
#include "src/dialogs/filedownloaddialog.h"
#include "src/elbe/buildmanager.h"
#include "src/elbe/buildprocessworker.h"
#include "src/projects/existingprojects.h"
#include "src/xml/xmlfilehandler.h"
#include "src/projects/projectitemmodel.h"
#include "src/elbe/elbehandler.h"
#include "src/dialogs/elbesettingsdialog.h"
#include "src/update/updatetab.h"

#include <QDebug>

#include <src/update/updates.h>

#include <src/dialogs/changeworkspacedialog.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	ui->Updates_Tab->hide();

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



/******************************** MenuBar actions ***********************************/

/********************** file menu *********************/
void MainWindow::on_actionNew_Project_triggered()
{
	NewProjectWizard *wiz = new NewProjectWizard();
	wiz->show();
}

void MainWindow::on_actionNew_XML_triggered()
{
	if ( projectmanager->isProjectOpened() ) {
		NewXMLDialog *xml = new NewXMLDialog();
		xml->show();
	} else {
		//should never be reached as the button is disabled when if project is open
		helpers::showMessageBox("Warning",
								"A project has to be open",
								QMessageBox::StandardButtons(QMessageBox::Ok),
								QMessageBox::Ok);
	}
}

void MainWindow::on_actionOpen_triggered()
{
	OpenProjectFileDialog *fileChooser = new OpenProjectFileDialog(this, helpers::getHomeDirectoryFromSystem());
	if ( fileChooser->exec() == QDialog::Accepted ) {
		ProjectHandler::openProject(fileChooser->selectedUrls().value(0).toLocalFile()+"/.project");
	}
}

void MainWindow::on_actionImport_triggered()
{
	if ( projectmanager->isProjectOpened()) {
		ImportFileDialog *dialog = new ImportFileDialog();
		dialog->show();
	} else {
		//same as above: should not be reaches as button is disabled if no project is open
		helpers::showMessageBox("Warning",
								"A project has to be open",
								QMessageBox::StandardButtons(QMessageBox::Ok),
								QMessageBox::Ok);
	}
}


void MainWindow::on_actionSave_triggered()
{
	XmlFileHandler::saveFile();
}

void MainWindow::on_actionClose_triggered()
{
	ProjectHandler::closeProject();
}

void MainWindow::on_actionDelete_triggered()
{
	DeleteDialog *deletedialog = new DeleteDialog();
	deletedialog->show();
}

void MainWindow::on_actionOpen_in_Explorer_triggered()
{
	QDesktopServices::openUrl(QUrl(projectmanager->projectDirectory()));
}



void MainWindow::on_actionChange_workspace_triggered()
{
	ChangeWorkspaceDialog *dialog = new ChangeWorkspaceDialog();
	dialog->show();
}

UpdateTab *MainWindow::getUpdatetab() const
{
	return updatetab;
}

/******************************************************/

/******************** elbe menu ***********************/

void MainWindow::on_actionBuild_triggered()
{
	BuildProcessStartDialog *dialog = new BuildProcessStartDialog();
	dialog->show();
}

void MainWindow::on_actionValidate_triggered()
{
	SchemaValidation *validation = new SchemaValidation(ui->Editor->toPlainText());
	validation->validate();
}

void MainWindow::on_actionDownload_files_triggered()
{
	FileDownloadDialog *dialog = new FileDownloadDialog();
	dialog->show();
}

/******************************************************/

/******************** help menu ***********************/

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox *msgBox = new QMessageBox();
	msgBox->setText("elbeFrontend");
	msgBox->setInformativeText("Version 1.0");
	msgBox->setParent(this);
	msgBox->setIcon(QMessageBox::Information);
	msgBox->setStandardButtons(QMessageBox::Close);
	msgBox->setDefaultButton(QMessageBox::Close);
	msgBox->show();
}

void MainWindow::on_actionAbout_elbe_triggered()
{
	aboutElbeMessageBox->show();
}

void MainWindow::on_actionSettings_triggered()
{
	ElbeSettingsDialog *settings = new ElbeSettingsDialog();
	settings->show();
}

/************************************************************************************/

/********************************** editor utils ************************************/

void MainWindow::displayFileInEditor(QString content)
{
	ui->Editor->setPlainText(content);
}

void MainWindow::on_Editor_textChanged()
{
	filemanager->setIsSaved(false);
	ui->Editor->setExtraSelections(QList<QTextEdit::ExtraSelection>());
}

void MainWindow::setEditorTabVisible(bool visible)
{
	if ( visible ) {
		ui->OpenFileNameLabel->show();
		ui->EditorCloseButton->show();
	} else {
		ui->OpenFileNameLabel->hide();
		ui->EditorCloseButton->hide();
	}
}

void MainWindow::updateEditorTabSaveStatus(bool saved)
{
	if ( saved ) {
		ui->OpenFileNameLabel->setText(filemanager->currentFileName());
	} else {
		ui->OpenFileNameLabel->setText(filemanager->currentFileName()+"*");
	}
}

void MainWindow::setOpenFileNameLabelText(QString text)
{
	ui->OpenFileNameLabel->setText(text);
}

void MainWindow::on_EditorCloseButton_clicked()
{
	XmlFileHandler::closeFile();
}

/************************************************************************************/

/*************************** project structure utils ********************************/

void MainWindow::on_ProjektStructure_doubleClicked(const QModelIndex &index)
{
	QString path = model->itemPath(index);
	QFileInfo item(path);
	if ( item.isFile() ) {
		XmlFileHandler::openFile(path);
	} else {
		return;
	}
}

void MainWindow::updateProjectStructure()
{
	if ( projectmanager->isProjectOpened() ) {
		clearProjectStructure();
	} else {
		renewProjectStructure();
	}
}

void MainWindow::clearProjectStructure()
{
	model = new ProjectItemModel();
	model->clear();
	ui->ProjektStructure->setModel(model);
}

void MainWindow::renewProjectStructure()
{
	QString projectName;
	if ( !ElbeHandler::projectIsInElbe(projectmanager->projectPath()) ) {
		projectName = projectmanager->projectName() + " (does not exist in initVM)";
	} else {
		projectName = projectmanager->projectName();
	}

	model = new ProjectItemModel();
	model->setProjectDetails( projectmanager->projectDirectory(),  projectName);
	ui->ProjektStructure->setModel(model);
	ui->ProjektStructure->header()->hide();
}

void MainWindow::updateItemModel(QString dir)
{
	Q_UNUSED(dir);

	Project *projectmanager = Project::getInstance();
	projectmanager->setProjectModified(true);
	renewProjectStructure();
}

void MainWindow::updateCurrentFile(QString path)
{
	//QFileSystemWatcher identifies removing a file as a modification of the file
	//so if the "modified" file was actually removed we can return here
	if ( !QFile(path).exists() ) {
		XmlFileHandler::handleRemotelyDeletedFile();
		return;
	}

	XmlFileHandler::handleFileModification(path);
}



/************************************************************************************/

/***************************** messageLog utils *************************************/

void MainWindow::on_MessageLog_textChanged()
{
	//scrollbar always at the bottom
	ui->MessageLog->ensureCursorVisible();
}

void MainWindow::messageLogAppendText(const QString &str, const QString &colorHexVal)
{
	QColor color;
	color.setNamedColor(colorHexVal);

	QTextEdit *msgLog = this->messageLog();
	msgLog->setTextColor(color);
	msgLog->append(str);
}

/************************************************************************************/

/***************************** statusBar utils **************************************/

void MainWindow::showTempStatusOnStatusBar(QString status)
{
	statusBar()->showMessage(status);
}

void MainWindow::showNormalStatusOnStatusBar(QString status)
{
	QWidget *widget = new QWidget();
	QLayout *layout = new QHBoxLayout();
	QLabel *label = new QLabel(widget);
	label->setText(status);
	layout->addWidget(label);
	layout->setMargin(0);

	widget->setLayout(layout);
	statusBar()->addWidget(widget);
}

void MainWindow::showPermStatusOnStatusBar(QString status)
{
	QWidget *widget = new QWidget();
	QLayout *layout = new QHBoxLayout();
	QLabel *label = new QLabel(widget);
	label->setText(status);
	layout->addWidget(label);
	layout->setMargin(0);

	widget->setLayout(layout);
	statusBar()->addPermanentWidget(widget);

	//store the widget to be able to change it later
	permStatus = label;
}

/************************************************************************************/

void MainWindow::showUpdateTab(UpdateTab *utab)
{
	updatetab = utab;
	ui->Updates_Tab->layout()->addWidget(utab);
	ui->Updates_Tab->show();
}

void MainWindow::hideUpdateTab()
{
	ui->Updates_Tab->layout()->removeWidget(updatetab);
	ui->Updates_Tab->hide();
}


/********************************* misc *********************************************/

void MainWindow::enableActionsOnProjectOpen(bool isOpen)
{
	ui->actionClose->setEnabled(isOpen);
	ui->actionImport->setEnabled(isOpen);
	ui->actionNew_XML->setEnabled(isOpen);
	ui->actionOpen_in_Explorer->setEnabled(isOpen);

	BuildManager *buildmanager = BuildManager::getInstance();
	//only if no build is running it's safe to enable the actions
	if ( !(buildmanager->isBuildRunning() || buildmanager->isLoadingFiles()) ) {
		changeElbeActionsEnabledStatus(isOpen);
	}
}

void MainWindow::enableActionsOnXMLOpen(bool isOpen)
{
	ui->actionSave->setEnabled(isOpen);
	ui->actionSave_As->setEnabled(isOpen);
	ui->actionValidate->setEnabled(isOpen);
}

void MainWindow::setElbeVersion(const QString &version)
{
	elbeVersion = version;

	if ( elbeVersion.isEmpty() ) {
		elbeVersion = "No elbe found";
	}
	showPermStatusOnStatusBar(elbeVersion);
	initAboutElbeMessageBox();
}

void MainWindow::changeElbeVersion(const QString &version)
{
	elbeVersion = version;

	if ( elbeVersion.isEmpty() ) {
		elbeVersion = "No elbe found";
	}
	permStatus->setText(elbeVersion);
	initAboutElbeMessageBox();
}

void MainWindow::initAboutElbeMessageBox()
{
	aboutElbeMessageBox = new QMessageBox();
	aboutElbeMessageBox->setText("Elbe-Version");
	aboutElbeMessageBox->setInformativeText(elbeVersion);
	aboutElbeMessageBox->setParent(this);
	aboutElbeMessageBox->setIcon(QMessageBox::Information);
	aboutElbeMessageBox->setStandardButtons(QMessageBox::Close);
	aboutElbeMessageBox->setDefaultButton(QMessageBox::Close);
}

void MainWindow::changeNewXmlButtonEnabledStatus(bool status)
{
	ui->actionNew_XML->setEnabled(status);
}

void MainWindow::changeImportButtonEnabledStatus(bool status)
{
	ui->actionImport->setEnabled(status);
}

void MainWindow::changeElbeActionsEnabledStatus(bool status)
{
	ui->actionBuild->setEnabled(status);
	ui->actionDownload_files->setEnabled(status);
}

void MainWindow::closeEditorWindow() {
	this->editor()->clear();
	this->editor()->setEnabled(false);
	this->editor()->setLineNumberAreaVisible(false);
	this->setEditorTabVisible(false);
}



/************************************************************************************/

/*********************** getter for ui elements *************************************/

CodeEditor *MainWindow::editor() const
{
	return ui->Editor;
}

QTextEdit *MainWindow::messageLog() const
{
	return ui->MessageLog;
}

QAction *MainWindow::actionClose() const
{
	return ui->actionClose;
}

/************************************************************************************/

/********************************* exit handling *******************************************/

//overwrite closeEvent
void MainWindow::closeEvent(QCloseEvent *event)
{
	bool canAcceptCloseEvent = false;
	BuildManager *buildmanager = BuildManager::getInstance();

	QString openedProject;
	if ( projectmanager->isProjectOpened() ) {
		openedProject = projectmanager->projectPath();
	}

	//check if there are unsafed changes and handle them
	if ( filemanager->isOpen() && !filemanager->isSaved() ) {
		canAcceptCloseEvent = saveOnClose();
	}  else {
		canAcceptCloseEvent = true;
	}

	//the application can not be closed if filedownload is still in progress
	if ( buildmanager->isLoadingFiles() ) {
		helpers::showMessageBox("Application can not be closed!",
								"Download in progress.",
								QMessageBox::StandardButtons(QMessageBox::Ok),
								QMessageBox::Ok);
		event->ignore();
		return;
	}

	//if the closeEvent was declined previously we can skip the following
	if ( canAcceptCloseEvent && buildmanager->isBuildRunning() ) {
		int ret = helpers::showMessageBox("Build is still running",
										  "Are you sure you want to exit the application",
										  QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel),
										  QMessageBox::Yes);
		switch ( ret ) {
			case QMessageBox::Yes:
				//re-check in case the messagebox was left open and the situation has changed
				if ( buildmanager->isLoadingFiles() ) {
					helpers::showMessageBox("Application can not be closed!",
											"Download in progress.",
											QMessageBox::StandardButtons(QMessageBox::Ok),
											QMessageBox::Ok);
					event->ignore();
					return;
				}
				//handle close if build is running
				rememberBusyProject();
				break;
			case QMessageBox::Cancel:
				canAcceptCloseEvent = false;
				break;
			default:
				//should never be reached
				break;
		}
	}

	if ( canAcceptCloseEvent ) {
		rememberOpenedProject(openedProject);
		event->accept();
	} else {
		event->ignore();
	}
}



bool MainWindow::saveOnClose()
{
	bool value;

	int ret = helpers::showMessageBox("There are files which are not saved",
									  "Do you want to save the changes before closing?",
									  QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel),
									  QMessageBox::Yes);
	switch ( ret ) {
		case QMessageBox::Yes:
			XmlFileHandler::saveFile();
			ProjectHandler::closeProject();
			value = true;
			break;
		case QMessageBox::Discard:
			//pretened the project was saved
			filemanager->setIsSaved(true);
			ProjectHandler::closeProject();
			value = true;
			break;
		case QMessageBox::Cancel:
//				msgBox.close();
			value = false;
			break;
		default:
			//should not be reached
			break;
	}

	return value;
}


void MainWindow::rememberBusyProject()
{
	BuildManager *buildmanager = BuildManager::getInstance();
	ExistingProjects *existing = new ExistingProjects();

	//set flag in .elbefrontend
	existing->addBusyFlag(buildmanager->processWorkerPointer()->buildingProjectPath());

	QThread *buildThread = buildmanager->processWorkerPointer()->thread();
	if ( buildThread->isRunning() ) {
		buildmanager->processWorkerPointer()->statusBarBuildThread()->requestInterruption();
	}
}

void MainWindow::rememberOpenedProject(QString project)
{
	ExistingProjects *existing = new ExistingProjects();
	existing->addOpenFlag(project);
}

/************************************************************************************/





