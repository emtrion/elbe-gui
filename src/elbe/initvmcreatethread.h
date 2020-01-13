#ifndef INITVMCREATETHREAD_H
#define INITVMCREATETHREAD_H

#include <QThread>



class InitvmCreateThread : public QThread
{
	Q_OBJECT

	public:
		InitvmCreateThread();


		void setNewInitvmPath(const QString &value);

	private:
		QString newInitvmPath;

	signals:
		void ready();

		// QThread interface
	protected:
		virtual void run() override;
};

#endif // INITVMCREATETHREAD_H
