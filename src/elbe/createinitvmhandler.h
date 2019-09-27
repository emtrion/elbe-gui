#ifndef CREATEINITVMHANDLER_H
#define CREATEINITVMHANDLER_H


#include <QObject>

class CreateInitvmHandler : public QObject
{
		Q_OBJECT
	public:
		explicit CreateInitvmHandler(QObject *parent = 0);

		void createNewInitvm(QString newInitvm);
	signals:

	public slots:
		void handleFinishedThread();
	private:
		void prepareStart();
};

#endif // CREATEINITVMHANDLER_H
