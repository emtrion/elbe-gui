#ifndef BUILDPROCESS_H
#define BUILDPROCESS_H


#include <QObject>


class QThread;
class BuildManager;
class ElbeHandler;
class BuildProcessWorker;


class BuildProcess : public QObject
{
		Q_OBJECT
	public:
		explicit BuildProcess(QObject *parent = 0);
		~BuildProcess();
		QThread *buildThread;


		QStringList getOutputFiles() const;
		void setOutputFiles(const QStringList &value);

		void startBuild(bool sourceOptionChecked, bool binOptionChecked);

		void waitBusyWithoutStartingBuild(QString id);

	private slots:
		void cleanup();
	private:
		QStringList outputFiles;
		void buildThreadInit();
		ElbeHandler *elbehandler;
		BuildManager *buildmanager;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;

		BuildProcessWorker *buildWorker;
};

#endif // BUILDPROCESS_H
