#include "openprojectfiledialog.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPushButton>
#include <QEvent>
#include <QDebug>

OpenProjectFileDialog::OpenProjectFileDialog(QWidget* parent, QString startdir) : QFileDialog(parent, "Load Correct Format", startdir),myOpenButton(0)
{
	if ( acceptMode() == QFileDialog::AcceptOpen ) {
		QDialogButtonBox *button_box = findChild<QDialogButtonBox*>();
		if (button_box) {
			myOpenButton = (QPushButton *)button_box->button(QDialogButtonBox::Open);

			qDebug() << myOpenButton->text();
		}
	}
//	connect(this, SIGNAL(fileSelected(QString))), this, SLOT(fileSelectionChanged(const QString&));
}



bool OpenProjectFileDialog::checkIfProject(const QString &file)
{
	QFile projectFile(file+".project");
	if ( projectFile.exists() ) {
		return true;
	} else {
		return false;
	}
}


