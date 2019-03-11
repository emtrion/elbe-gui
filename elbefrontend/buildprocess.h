#ifndef BUILDPROCESS_H
#define BUILDPROCESS_H

#include "elbehandler.h"

#include <QCheckBox>
#include <QObject>
#include <QThread>



class BuildProcess : public QObject
{
		Q_OBJECT
	public:
		explicit BuildProcess(QObject *parent = 0);
		~BuildProcess();
		QThread *buildThread;
		QThread *statusBarThread;

		QStringList getOutputFiles() const;
		void setOutputFiles(const QStringList &value);

		void startBuild();

	signals:


	public slots:
		void handleResults(const QString &result);
		void updateMessageLog(const QString &str);
		void updateStatusBar(const QString &str);
		void downloadFiles();
	private:
		QStringList outputFiles;
		void buildThreadInit();
		ElbeHandler *handler;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
};

#endif // BUILDPROCESS_H
