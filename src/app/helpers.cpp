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

#include "helpers.h"

#include <QProcessEnvironment>
#include <QApplication>

#include "src/mainwindow/mainwindow.h"


namespace helpers {

	QString getHomeDirectoryFromSystem()
	{
        QProcessEnvironment proEnv = QProcessEnvironment::systemEnvironment();
        QString homeDir;
        homeDir = proEnv.value("HOME", NULL);
        return homeDir;
    }



	MainWindow* getMainWindow()
	{
		foreach( QWidget *widget, qApp->topLevelWidgets()){
			if ( MainWindow *mainWindow = qobject_cast<MainWindow*>(widget)){
				return mainWindow;
			}
		}
		return NULL;
	}

	int showMessageBox(const QString &text, const QString &informativeText, QMessageBox::StandardButtons buttons, QMessageBox::Button defaultButton)
	{
		QMessageBox msgBox;
		msgBox.setText(text);
		msgBox.setInformativeText(informativeText);
		msgBox.setStandardButtons(buttons);
		msgBox.setDefaultButton(defaultButton);

		msgBox.setModal(true);
		msgBox.resize(400, 300);
		return msgBox.exec();
	}
}



