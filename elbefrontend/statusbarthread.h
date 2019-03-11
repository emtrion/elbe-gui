#ifndef STATUSBARTHREAD_H
#define STATUSBARTHREAD_H

#include <QObject>

class StatusBarThread : public QObject
{
		Q_OBJECT
	public:
		explicit StatusBarThread(QObject *parent = 0);

	signals:
		void statusBarHasChanged(const QString &str);

	public slots:
		void changeStatusBar();
};

#endif // STATUSBARTHREAD_H
