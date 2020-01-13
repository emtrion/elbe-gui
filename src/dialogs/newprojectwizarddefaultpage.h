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

#ifndef NEWPROJECTWIZARDDEFAULTPAGE_H
#define NEWPROJECTWIZARDDEFAULTPAGE_H

#include <QWizardPage>

class QLineEdit;
class NewProjectWizard;

namespace Ui {
	class NewProjectWizard;
}

class NewProjectWizardDefaultpage : public QWizardPage
{
		Q_OBJECT

	public:
		NewProjectWizardDefaultpage();
		~NewProjectWizardDefaultpage();

		void setDefaultPageUiPointer(Ui::NewProjectWizard *ui_pointer);
		void initializePage(int pageId);
		void connectFields();
		void displayDefaultSettings();
		virtual bool isComplete() const override;
		QList<QLineEdit *> projectSettingInputFields() const;
		QList<QString> defaultSettings() const;
		void setDefaultSettings(const QList<QString> &defaultSettings);

	private:
		Ui::NewProjectWizard *ui_pointer;
		QList<QLineEdit*> m_projectSettingInputFields;
		QList<QString> m_defaultSettings;
		QString m_name;
};

#endif // NEWPROJECTWIZARDDEFAULTPAGE_H
