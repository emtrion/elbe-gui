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



