#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QThread>


class UpdateThread : public QThread
{
		Q_OBJECT

	public:
		UpdateThread();

	signals:
		void resultReady(const QStringList &s);
		void errorLog(const QStringList &s);

	private:
		QThread *statusBarUpdateThread;

		// QThread interface
		void showUpdateInStatusBar();
	protected:
		void run() override;
};

#endif // UPDATETHREAD_H
