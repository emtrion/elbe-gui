#ifndef STATUSBARTHREAD_H
#define STATUSBARTHREAD_H

#include <QObject>

class StatusBarThread : public QObject
{
		Q_OBJECT
	public:
		explicit StatusBarThread(QObject *parent = 0);

	private:
		QStringList list;
		void changeStatusBar();

	signals:
		void statusBarHasChanged(const QString &str);

	public slots:
		void statusBarLoadingFile();
		void statusBarBuildRunning();
	private slots:
		void updateStatusBar(const QString &str);
};

#endif // STATUSBARTHREAD_H
