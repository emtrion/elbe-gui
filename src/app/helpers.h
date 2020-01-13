#ifndef HELPERS_H
#define HELPERS_H

#include <QMessageBox>

class QString;
class MainWindow;


namespace helpers {

	MainWindow* getMainWindow();
	QString getHomeDirectoryFromSystem();
	int showMessageBox(const QString &text,
					   const QString &informativeText,
					   QMessageBox::StandardButtons buttons,
					   QMessageBox::Button defaultButton);
}

#endif // HELPERS_H
