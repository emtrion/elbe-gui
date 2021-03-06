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

#include "openprojectfiledialog.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QToolButton>

OpenProjectFileDialog::OpenProjectFileDialog(QWidget* parent, QString startdir) :
	QFileDialog(parent, "Open file", startdir),
	myOpenButton(0)
{
	this->setFileMode(QFileDialog::Directory);
	this->setAcceptMode(QFileDialog::AcceptOpen);

	if ( acceptMode() == QFileDialog::AcceptOpen ) {
		QDialogButtonBox *button_box = findChild<QDialogButtonBox*>();
		if (button_box) {
			myOpenButton = (QPushButton *)button_box->button(QDialogButtonBox::Open);
		}
	}

	backButton = findChild<QToolButton*>("backButton");
	forwardButton = findChild<QToolButton*>("forwardButton");

	this->setOption(QFileDialog::ShowDirsOnly, true);

	myOpenButton->setEnabled(false);

	connect(this, SIGNAL(currentChanged(QString)), this, SLOT(fileSelectionChanged(const QString&)));
	connect(backButton, SIGNAL(clicked()), this, SLOT(backOrForClicked()));
	connect(forwardButton, SIGNAL(clicked()), this, SLOT(backOrForClicked()));

	myOpenButton->installEventFilter(this);
}

void OpenProjectFileDialog::backOrForClicked()
{
	QFileInfo file(directory().absolutePath());
	updateCurrentSelection(file.absoluteFilePath());

	if ( !file.path().isEmpty() ) {
		if ( checkIfProject(file.absoluteFilePath()) ) {
			myOpenButton->setEnabled(true);
		} else {
			myOpenButton->setEnabled(false);
		}
	} else {
		//file is empty
	}
}

void OpenProjectFileDialog::fileSelectionChanged(const QString &file)
{
	updateCurrentSelection(file);
	if ( !file.isEmpty() ) {
		if ( checkIfProject(file) ) {
			myOpenButton->setEnabled(true);
		} else {
			myOpenButton->setEnabled(false);
		}
	} else {
		//file is empty
	}
}

bool OpenProjectFileDialog::checkIfProject(const QString &file)
{
	QFileInfo fileInfo(file+"/.project");
	if ( fileInfo.exists() ) {
		return true;
	} else {
		return false;
	}
}

void OpenProjectFileDialog::updateCurrentSelection(QString file)
{
	if ( !file.isEmpty() ) {
		currentSelection = QFileInfo(file+"/.project");
	}
}

bool OpenProjectFileDialog::isSelectionValid()
{
	QFile file(directory().absolutePath()+"/.project");
	return currentSelection.exists() && file.exists();
}

//catch events which alter the enabled state of the open button
bool OpenProjectFileDialog::eventFilter(QObject *obj, QEvent *event)
{
	if ( obj == myOpenButton ) {
		//if a signal changed the status but the selection is not valid...
		if( event->type() == QEvent::EnabledChange && !isSelectionValid() && myOpenButton->isEnabled()) {
			//...it's reset
			myOpenButton->setEnabled(false);
			return true;
		} else {
			return false;
		}
	} else {
		//if the event didn't come from myOpenButton it's passed down to the default eventfilter which will know what to do with it
		return QFileDialog::eventFilter(obj, event);
	}
}
