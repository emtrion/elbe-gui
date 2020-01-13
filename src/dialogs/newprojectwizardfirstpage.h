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

#ifndef NEWPROJECTWIZARDFIRSTPAGE_H
#define NEWPROJECTWIZARDFIRSTPAGE_H

#include <QWizardPage>

namespace Ui {
	class NewProjectWizard;
}
/*NewProjectWizardFirstPage is needed to represent the first page of the NewProjectWizard.
 *validation through the next/finish-button is offered by the basic QWizardPage-class in form of mandatory fields.
 *To get a more comlex validation one needs to re-implement the isComplete()-method*/
class NewProjectWizardFirstPage : public QWizardPage
{
	Q_OBJECT

	public:
		NewProjectWizardFirstPage();
		~NewProjectWizardFirstPage();

		void setFirstPageUiPointer(Ui::NewProjectWizard *ui_pointer);
		void connectSignals();
		bool isComplete() const;
		QString newProjectName() const;
		QString newProjectPath() const;

	private:
		 Ui::NewProjectWizard *ui_pointer;
		 QString m_newProjectPath;
		 QString m_newProjectName;
		 QPixmap acceptIcon;
		 QPixmap errorIcon;
};

#endif // NEWPROJECTWIZARDFIRSTPAGE_H
