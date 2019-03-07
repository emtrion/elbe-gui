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

		QList<QCheckBox *> getOutputFiles() const;
		void setOutputFiles(const QList<QCheckBox *> &value);

		void startBuild();

	signals:


	public slots:
		void handleResults(const QString &result);
		void updateMessageLog(const QString &str);
		void updateStatusBar(const QString &str);
	private:
		QList<QCheckBox*> outputFiles;
		void buildThreadInit();
		ElbeHandler *handler;
};

#endif // BUILDPROCESS_H
