#ifndef BUILDPROCESSWORKER_H
#define BUILDPROCESSWORKER_H


#include <QObject>

class QProcess;
class QThread;
class BuildProcessStatusBarUpdate;
class Project;
class BuildManager;

class BuildProcessWorker : public QObject
{
		Q_OBJECT

	public:
		explicit BuildProcessWorker(QStringList outputFiles);
		~BuildProcessWorker();

		QString buildingProjectPath() const;
		void setSkipDownload(bool value);
		QThread *statusBarBuildThread() const;
		void setBuildingElbeID(const QString &value);
		QString buildingElbeID() const;

	signals:
		void resultReady();
		void outputReady(const QString &str);
		void messageLogHasUpdate(const QString &str, const QString &colorHexValue);
		void startStatusBarBuild();
		void startStatusBarLoad();
		void done();

	public slots:
		void doWork();
		void printLog();
		void updateMessageLog(const QString &str);
		void downloadFiles();

	private:
		QProcess *process;
		QString output;
		QStringList outputFiles;
		QString m_buildingProjectPath;
		QString m_buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
		Project *projectmanager;
		BuildManager *buildmanager;
		BuildProcessStatusBarUpdate *statusBarWorker;
		bool skipDownload;
		QThread *m_statusBarBuildThread;
		QThread *statusBarLoadThread;

	private:
		void showLoadingInStatusBar();
		void showBuildingInStatusBar();
		void waitBusy();





};

#endif // BUILDPROCESSWORKER_H
