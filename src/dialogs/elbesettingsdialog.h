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

#ifndef ELBESETTINGSDIALOG_H
#define ELBESETTINGSDIALOG_H

#include <QDialog>

class ApplicationConfig;

namespace Ui {
	class ElbeSettingsDialog;
}

class ElbeSettingsDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ElbeSettingsDialog(QWidget *parent = 0);
		~ElbeSettingsDialog();

	private slots:
		void on_defaultCheckbox_toggled(bool checked);
		void on_buttonBox_accepted();
		void on_elbeEntry_textChanged(const QString &arg1);
		void on_initVMEntry_textChanged(const QString &arg1);

	private:
		Ui::ElbeSettingsDialog *ui;
		ApplicationConfig *appConfig;
		QString oldInitVM;
		QPixmap acceptIcon;
		QPixmap errorIcon;

		//used as parameter in updateInformation()
		enum Icon {
			ACCEPT,
			DECLINE
		};

	private:
		bool fieldsFilled();
		void updateButtonBox(const bool status);
		bool validElbe(const QString &elbe);
		void updateInformation(Icon value, const QString &text);
};

#endif // ELBESETTINGSDIALOG_H
