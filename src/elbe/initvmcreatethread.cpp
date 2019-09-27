#include "initvmcreatethread.h"
#include "elbehandler.h"

InitvmCreateThread::InitvmCreateThread()
{

}

void InitvmCreateThread::setNewInitvmPath(const QString &value)
{
	newInitvmPath = value;
}

void InitvmCreateThread::run()
{
	ElbeHandler::createNewInitVM(newInitvmPath);

//	QThread::sleep(5);

	emit(ready());
}

