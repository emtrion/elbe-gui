#include "openprojectfiledialog.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QToolButton>

OpenProjectFileDialog::OpenProjectFileDialog(QWidget* parent, QString startdir) : QFileDialog(parent, "Open file", startdir),myOpenButton(0)
{
	this->setFileMode(QFileDialog::Directory);
	this->setAcceptMode(QFileDialog::AcceptOpen);

	if ( acceptMode() == QFileDialog::AcceptOpen ) {
		QDialogButtonBox *button_box = findChild<QDialogButtonBox*>();
		if (button_box) {
			myOpenButton = (QPushButton *)button_box->button(QDialogButtonBox::Open);
//			qDebug() << myOpenButton->text();
		}
	}

	backButton = findChild<QToolButton*>("backButton");
	forwardButton = findChild<QToolButton*>("forwardButton");

	myOpenButton->setText("&Open");


	this->setOption(QFileDialog::ShowDirsOnly, true);

	myOpenButton->setEnabled(false);

//	connect(this, SIGNAL(directoryEntered(QString)), this, SLOT(fileSelectionChanged(const QString&)));
	connect(this, SIGNAL(currentChanged(QString)), this, SLOT(fileSelectionChanged(const QString&)));
	connect(backButton, SIGNAL(clicked()), this, SLOT(backOrForClicked()));
	connect(forwardButton, SIGNAL(clicked()), this, SLOT(backOrForClicked()));

	myOpenButton->installEventFilter(this);


}


void OpenProjectFileDialog::backOrForClicked()
{
//	qDebug() << directory().absolutePath();

	QFileInfo file(directory().absolutePath());
	updateCurrentSelection(file.absoluteFilePath());

	if ( !file.path().isEmpty() ) {
		if ( checkIfProject(file.absoluteFilePath()) ) {
//			qDebug() << "true";
			myOpenButton->setEnabled(true);
		} else {
//			qDebug() << "false";
			myOpenButton->setEnabled(false);
		}
	} else {
//		qDebug() << "file is empty";
	}
}

void OpenProjectFileDialog::fileSelectionChanged(const QString &file)
{
	updateCurrentSelection(file);

//	qDebug() << "signal received!";

	if ( !file.isEmpty() ) {
		if ( checkIfProject(file) ) {
//			qDebug() << "true";
			myOpenButton->setEnabled(true);
		} else {
//			qDebug() << "false";
			myOpenButton->setEnabled(false);
		}
	} else {
//		qDebug() << "file is empty";
	}
	myOpenButton->setText("&Open");
}

bool OpenProjectFileDialog::checkIfProject(const QString &file)
{
	QFileInfo fileInfo(file+"/.project");
//	qDebug() << fileInfo.filePath();

	if ( fileInfo.exists() ) {
		return true;
	} else {
		return false;
	}
}


void OpenProjectFileDialog::updateCurrentSelection(QString file)
{
	if ( !file.isEmpty() ) {
//		qDebug() << "File received from "<< __func__<<": "<<file;
		currentSelection = QFileInfo(file+"/.project");
	}
}

bool OpenProjectFileDialog::isSelectionValid()
{
	QFile file(directory().absolutePath()+"/.project");
	return currentSelection.exists() && file.exists();
}

bool OpenProjectFileDialog::eventFilter(QObject *obj, QEvent *event)
{
	if ( obj == myOpenButton ) {
		if( event->type() == QEvent::EnabledChange && !isSelectionValid() && myOpenButton->isEnabled()) {
			myOpenButton->setEnabled(false);
//			qDebug() << "filtered: "<<event->type();
			return true;
		} else {
			return false;
		}
	} else {
		return QFileDialog::eventFilter(obj, event);
	}
}


