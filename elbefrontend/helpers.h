#ifndef HELPERS_H
#define HELPERS_H

#include <QString>

namespace helpers {
    QString getHomeDirectoryFromSystem();
	bool setProjectMetadata(QString projectName, QString projectPath);
}

#endif // HELPERS_H
