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

#ifndef NEWXMLDIALOG_H
#define NEWXMLDIALOG_H

#include <QDialog>

namespace Ui {
	class NewXMLDialog;
}

class NewXMLDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit NewXMLDialog(QWidget *parent = 0);
		~NewXMLDialog();

		bool isValid();
		void updateUI(bool valid);

	private slots:
		void on_FileNameEntry_textChanged(const QString &arg1);
		void on_buttonBox_accepted();

	private:
		Ui::NewXMLDialog *ui;
		QString fileName;
		QString projectPath;
		QPixmap acceptIcon;
		QPixmap errorIcon;
};

#endif // NEWXMLDIALOG_H
