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

#ifndef CHANGEWORKSPACEDIALOG_H
#define CHANGEWORKSPACEDIALOG_H

#include <QDialog>

#include <src/app/applicationconfig.h>

namespace Ui {
	class ChangeWorkspaceDialog;
}

class ChangeWorkspaceDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ChangeWorkspaceDialog(QWidget *parent = 0);
		~ChangeWorkspaceDialog();

	private slots:
		void on_directoryEntry_textChanged(const QString &arg1);

		void on_defaultCheckbox_toggled(bool checked);

		void on_buttonBox_accepted();

		void on_browseButton_clicked();

	private:
		Ui::ChangeWorkspaceDialog *ui;

		QPixmap acceptIcon;
		QPixmap errorIcon;
		QString defaultEntry;
		void checkIfValid(QString input);
		ApplicationConfig *appConfig;



};

#endif // CHANGEWORKSPACEDIALOG_H
