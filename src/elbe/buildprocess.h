#ifndef BUILDPROCESS_H
#define BUILDPROCESS_H


#include <QObject>

class QThread;
class BuildManager;
class BuildProcessWorker;


class BuildProcess : public QObject
{
		Q_OBJECT

	public:
		explicit BuildProcess(QObject *parent = 0);
		~BuildProcess();

		void startBuild(bool sourceOptionChecked, bool binOptionChecked);
		void waitBusyWithoutStartingBuild(QString id);

	private slots:
		void cleanup();

	private:
		void buildThreadInit();

	private:
		QThread *buildThread;
		BuildManager *buildmanager;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
		BuildProcessWorker *buildWorker;
};

#endif // BUILDPROCESS_H
