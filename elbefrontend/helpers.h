#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include "newprojectwizard.h"

namespace helpers {
    QString getHomeDirectoryFromSystem();
	bool setProjectMetadata(QString projectName, QString projectPath, NewProjectWizard::projectSettings settings);
//	void deleteFile(QString path);
}

#endif // HELPERS_H
