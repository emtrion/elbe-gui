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

#ifndef NEWPROJECTWIZARD_H
#define NEWPROJECTWIZARD_H

#include <QWizard>


namespace Ui {
	class NewProjectWizard;
}

class NewProjectWizard : public QWizard
{
		Q_OBJECT

	public:
		explicit NewProjectWizard(QWidget *parent = 0);
		~NewProjectWizard();

		void displayDefaultPath();
		bool validation();
		void initSecondPageDefault();
		void initThirdPageDefault();

	private slots:
		void on_DefaultPathButton_toggled(bool checked);
		void on_ProjectNameEntry_textChanged(const QString &arg1);
		void on_ProjectPathEntry_editingFinished();
		void on_ProjectPathBrowsButton_clicked();
		void on_NewProjectWizard_accepted();
		void on_ProjectPathEntry_textChanged(const QString &arg1);
		void on_DefaultSetButton_Page3_toggled(bool checked);
		void on_DefaultSetButton_Page2_toggled(bool checked);
		void on_NewProjectWizard_currentIdChanged(int id);

	private:
		Ui::NewProjectWizard *ui;
		const QString defaultPath;

};

#endif // NEWPROJECTWIZARD_H
