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

#ifndef OPENPROJECTFILEDIALOG_H
#define OPENPROJECTFILEDIALOG_H

#include <QFileDialog>

class QToolButton;

class OpenProjectFileDialog : public QFileDialog
{
	Q_OBJECT

	public:
		OpenProjectFileDialog(QWidget* parent, QString directory);
		virtual ~OpenProjectFileDialog(){}

	protected:
		bool checkIfProject(const QString &file);
		bool eventFilter(QObject *obj, QEvent *event) override;

	protected slots:
		void fileSelectionChanged(const QString &file);
		void backOrForClicked();

	private:
		QPushButton *myOpenButton;
		QToolButton *backButton;
		QToolButton *forwardButton;
		QFileInfo currentSelection;

	private:
		void updateCurrentSelection(QString file);
		bool isSelectionValid();

};

#endif // OPENPROJECTFILEDIALOG_H
