#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

class QString;

namespace filesystemWatcher{

	void init();
	void addPath(QString path);
	void removePath(QString path);
}

#endif // FILESYSTEMWATCHER_H
