#include "ui_mainwindow.h"
#include "mainwindow.h"
//#include <QWidget>
//#include <QPlainTextEdit>
//#include <QColor>
#include <QDebug>
//#include <QFileSystemModel>
//#include <QStandardItemModel>
//#include <QTreeWidgetItem>
//#include <QMessageBox>
//#include <QErrorMessage>
//#include <QFileDialog>
//#include <QCheckBox>
//#include <QThread>
//#include <QString>

#include <QDesktopServices>

//#include "codeeditor.h"
//#include "qtermwidget5/qtermwidget.h"
#include "newxmldialog.h"
//#include "helpers.h"
#include "newprojectwizard.h"
#include "importfiledialog.h"
//#include "projectmanager.h"
#include "schemavalidation.h"
#include "projecthandler.h"
#include "openprojectfiledialog.h"
#include "deletedialog.h"
#include "buildprocessstartdialog.h"
#include "filedownloaddialog.h"
#include "buildmanager.h"
#include "buildprocessworker.h"
#include "existingprojects.h"
#include "xmlfilehandler.h"
#include "projectitemmodel.h"
#include "elbehandler.h"
#include "elbesettingsdialog.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //add terminal to window
//    QTermWidget *console = new QTermWidget(ui->Terminal_Tab);
//    console->setColorScheme("DarkPastels");
////    console->setContentsMargins(0, 0, 0, 0);
//    ui->Terminal_Tab->layout()->addWidget(console);

	ui->Terminal_Tab->hide();
	ui->DisplaView->removeTab(1);

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


/******************************** toolbar actions ***********************************/

void MainWindow::on_actionNew_triggered()
{
	/*for testing will be removed later*/

//
//	 projectmanager->update("/home/hico/elbefrontFilehandlingTestFolder/bsp1/.project");
//	 projectmanager->setProjectOpened(true);
}

/************************************************************************************/

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
		QMessageBox *msgBox = new QMessageBox(this);
		msgBox->setIcon(QMessageBox::Critical);
		msgBox->setText("A project has to be opened!");
		msgBox->exec();
	}
}

void MainWindow::on_actionOpen_triggered()
{
	OpenProjectFileDialog *fileChooser = new OpenProjectFileDialog(this, "/home/hico");
	if ( fileChooser->exec() == QDialog::Accepted ) {
		ProjectHandler *handler = new ProjectHandler();
		handler->openProject(fileChooser->selectedUrls().value(0).toLocalFile()+"/.project");
	}
}

void MainWindow::on_actionImport_triggered()
{
	if ( projectmanager->isProjectOpened()) {
		ImportFileDialog *dialog = new ImportFileDialog();
		dialog->show();
	} else {
		QMessageBox *msgBox = new QMessageBox(this);
		msgBox->setIcon(QMessageBox::Critical);
		msgBox->setText("A project has to be opened!");
		msgBox->exec();
	}
}


void MainWindow::on_actionSave_triggered()
{
//	qDebug() << __func__;
	XmlFileHandler *handler = new XmlFileHandler();
	handler->saveFile();
}

void MainWindow::on_actionClose_triggered()
{
	ProjectHandler *handler = new ProjectHandler();
	handler->closeProject();
}

void MainWindow::on_actionDelete_triggered()
{
	//open project selection
	DeleteDialog *projectChooser = new DeleteDialog();
	projectChooser->show();
}

void MainWindow::on_actionOpen_in_Explorer_triggered()
{
	QDesktopServices::openUrl(QUrl(projectmanager->getProjectDirectory()));
}

/******************************************************/

/******************** elbe menu ***********************/

void MainWindow::on_actionBuild_triggered()
{
//	ElbeHandler *elbeHandler = new ElbeHandler();
//	elbeHandler->startBuildProcess();

	BuildProcessStartDialog *dialog = new BuildProcessStartDialog();
	dialog->show();
}

void MainWindow::on_actionValidate_triggered()
{
	SchemaValidation *sv = new SchemaValidation(ui->Editor->toPlainText());
	sv->validate();
}

void MainWindow::on_actionDownload_files_triggered()
{
	FileDownloadDialog *dialog = new FileDownloadDialog();
	dialog->show();
}

/******************************************************/

/******************** help menu ***********************/

void MainWindow::on_actionAbout_triggered()
{}

void MainWindow::on_actionAbout_elbe_triggered()
{
	aboutElbeMessageBox->show();
}

void MainWindow::on_actionSettings_triggered()
{
	auto settings = new ElbeSettingsDialog();
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
		ui->OpenFileNameLabel->setText(filemanager->getCurrentFileName());
	} else {
		ui->OpenFileNameLabel->setText(filemanager->getCurrentFileName()+"*");
	}
}

void MainWindow::setOpenFileNameLabelText(QString text)
{
	ui->OpenFileNameLabel->setText(text);
}

void MainWindow::on_EditorCloseButton_clicked()
{
	XmlFileHandler *handler = new XmlFileHandler();
	handler->closeFile();
}

/************************************************************************************/

/*************************** project structure utils ********************************/

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
	ElbeHandler *elbe = new ElbeHandler();
	if ( !elbe->projectIsInElbe(projectmanager->getProjectPath()) ) {
		projectName = projectmanager->getProjectName() + " (does not exist in initVM)";
	} else {
		projectName = projectmanager->getProjectName();
	}

	model = new ProjectItemModel();
	model->setProjectDetails( projectmanager->getProjectDirectory(),  projectName);
	ui->ProjektStructure->setModel(model);
	ui->ProjektStructure->header()->hide();
}

void MainWindow::updateItemModel(QString dir)
{
	Q_UNUSED(dir);
//	qDebug() << "signal received in "<<__func__;

	ProjectManager *pm = ProjectManager::getInstance();
	pm->setProjectModified(true); //tell the projectmanager that current project has been modified

	MainWindow *mw = helpers::getMainWindow();
	mw->renewProjectStructure();
}

void MainWindow::updateCurrentFile(QString path)
{
//	qDebug() << "signal received in "<<__func__;
	XmlFileHandler *handler = new XmlFileHandler();
	handler->handleFileModification(path);
}

void MainWindow::on_ProjektStructure_customContextMenuRequested(const QPoint &pos)
{
//	QMenu *menu = new QMenu;
//	QAction *deleteAction = new QAction("&Delete", menu);
//	connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(on_ProjektStructure_ContextMenu_deleteAction_triggered()));


//	QModelIndex idx = ui->ProjektStructure->indexAt(pos);
//	if ( !idx.isValid() ) {
//		return;
//	}
////	qDebug() << idx;
////	qDebug() << idx.column() << idx.row();
////	qDebug() << idx.internalId();
////	qDebug() << idx.parent();

//	menu->addAction(deleteAction);
//	menu->exec(QCursor::pos());


}

void MainWindow::on_ProjektStructure_ContextMenu_deleteAction_triggered()
{
 //
}

/************************************************************************************/

/***************************** messageLog utils *************************************/

void MainWindow::on_MessageLog_textChanged()
{
	ui->MessageLog->ensureCursorVisible();
}

void MainWindow::messageLogAppendText(const QString &str, const QString &colorHexVal)
{

	//qDebug() << __func__<<" is in: "<<QThread::currentThreadId();

	QColor color;
	color.setNamedColor(colorHexVal);

	QTextEdit *msgLog = this->getMessageLog();

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
}

void MainWindow::setStatusBarTextColor(QColor color)
{
//	QPalette *p = new QPalette();
//	p->setColor(QPalette::Text, color);
//	ui->statusBar->setForegroundRole(p);
}

/************************************************************************************/

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

	showPermStatusOnStatusBar(elbeVersion);
	initAboutElbeMessageBox();
}

void MainWindow::initAboutElbeMessageBox()
{
	aboutElbeMessageBox = new QMessageBox();
	aboutElbeMessageBox->setText("Elbe-Version on this system");
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
	BuildManager *buildmanager = BuildManager::getInstance();
//	if ( (buildmanager->isBuildRunning() || buildmanager->isLoadingFiles()) ) {
	ui->actionBuild->setEnabled(status);
	ui->actionDownload_files->setEnabled(status);
//	} else {
//		ui->actionBuild->setEnabled(!status);
//		ui->actionDownload_files->setEnabled(!status);
//	}
}

/************************************************************************************/

/*********************** getter for ui elements *************************************/

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

/************************************************************************************/

/********************************* exit handling *******************************************/

//overwrite closeEvent
// there are things which must be handled before closing the application
void MainWindow::closeEvent(QCloseEvent *event)
{
	bool canAcceptCloseEvent = false;
	BuildManager *buildmanager = BuildManager::getInstance();

	//save the opened project
	QString openedProject;
	if ( projectmanager->isProjectOpened() ) {
		openedProject = projectmanager->getProjectPath();
//		qDebug() << "Open project is: "<<openedProject;
	}

	//check if there are unsafed changes and handle them
	if ( filemanager->getIsOpen() && !filemanager->getIsSaved() ) {
		canAcceptCloseEvent = saveOnClose();
	}  else {
		canAcceptCloseEvent = true;
	}


	//the application can not be closed if the filedownload after the build is still in progress
	if ( buildmanager->isLoadingFiles() ) {
		helpers::showMessageBox("Application can not be closed!", "Download in progress.", QMessageBox::StandardButtons(QMessageBox::Ok), QMessageBox::Ok);
		event->ignore();
		return;
	}


	//if the closeEvent was declined previously we can skip the following
	if ( canAcceptCloseEvent && buildmanager->isBuildRunning() ) {
		int ret = helpers::showMessageBox("Build is still running", "Are you sure you want to exit the application",
		QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::Cancel), QMessageBox::Yes);
		switch ( ret ) {
			case QMessageBox::Yes:
		//re-check in case the messagebox was left open and the situation has changed
				if ( buildmanager->isLoadingFiles() ) {
					helpers::showMessageBox("Application can not be closed!", "Download in progress.", QMessageBox::StandardButtons(QMessageBox::Ok), QMessageBox::Ok);
					event->ignore();
					return;
				}
				//handle close if build is running
				handleCloseDuringBuild();
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


//when the application is closed and there are unsafed changes the user is asked if they should be safed
bool MainWindow::saveOnClose()
{
	bool value;

	XmlFileHandler *filehandler = new XmlFileHandler(filemanager->getCurrentFilePath());
	ProjectHandler *projecthandler = new ProjectHandler();

	QMessageBox::StandardButtons stdButtons = (QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);
	int ret = helpers::showMessageBox("There are files which are not saved","Do you want to save the changes before closing?", stdButtons, QMessageBox::Yes);

	switch ( ret ) {
		case QMessageBox::Yes:
			filehandler->saveFile();
			projecthandler->closeProject();
			value = true;
			break;
		case QMessageBox::Discard:
			//pretened the project was saved
			filemanager->setIsSaved(true);
			projecthandler->closeProject();
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


void MainWindow::handleCloseDuringBuild()
{
	BuildManager *buildmanager = BuildManager::getInstance();
	ExistingsProjects *exist = new ExistingsProjects();

	//set flag in .elbefrontend
	exist->addBusyFlag(buildmanager->getProcessWorkerPointer()->getBuildingProjectPath());

	//quit threads
	QThread *buildThread = buildmanager->getProcessWorkerPointer()->thread();
	if ( buildThread->isRunning() ) {
		//stop thread
		buildmanager->getProcessWorkerPointer()->getStatusBarBuildThread()->requestInterruption();
	}
}

void MainWindow::rememberOpenedProject(QString project)
{
	ExistingsProjects *exist = new ExistingsProjects();
//	qDebug() << "adding openflag";
	exist->addOpenFlag(project);
}

/************************************************************************************/




