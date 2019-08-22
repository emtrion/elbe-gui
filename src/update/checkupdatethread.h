#ifndef CHECKUPDATETHREAD_H
#define CHECKUPDATETHREAD_H

#include <QThread>

class CheckUpdateThread : public QThread
{
		Q_OBJECT


	public:
		CheckUpdateThread();

	signals:
		void resultReady(const QString &s);


	private:
		void showCheckUpdateInStatusBar();
		QThread *statusBarCheckUpdateThread;

	// QThread interface
	protected:
		virtual void run() override;
};

#endif // CHECKUPDATETHREAD_H
