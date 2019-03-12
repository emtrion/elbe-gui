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


		QStringList getOutputFiles() const;
		void setOutputFiles(const QStringList &value);

		void startBuild(bool sourceOptionChecked, bool binOptionChecked);

	signals:


	public slots:

	private:
		QStringList outputFiles;
		void buildThreadInit();
		ElbeHandler *handler;
		QString buildingElbeID;
		QString buildingXmlPath;
		QString buildingOutPath;
};

#endif // BUILDPROCESS_H
