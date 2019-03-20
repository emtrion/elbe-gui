#ifndef BUILDPROCESSSTATUSBARUPDATE_H
#define BUILDPROCESSSTATUSBARUPDATE_H

#include <QObject>

class BuildProcessStatusBarUpdate : public QObject
{
		Q_OBJECT
	public:
		explicit BuildProcessStatusBarUpdate(QObject *parent = 0);

	private:
		QStringList list;
		void changeStatusBar();

	signals:
		void statusBarHasChanged(const QString &str);

	public slots:
		void statusBarLoadingFile();
		void statusBarBuildRunning();
	private slots:
		void updateStatusBar(const QString &str);
};

#endif // BUILDPROCESSSTATUSBARUPDATE_H
