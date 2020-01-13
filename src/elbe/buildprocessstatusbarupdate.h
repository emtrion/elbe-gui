#ifndef BUILDPROCESSSTATUSBARUPDATE_H
#define BUILDPROCESSSTATUSBARUPDATE_H

#include <QObject>

class BuildProcessStatusBarUpdate : public QObject
{
		Q_OBJECT

	public:
		explicit BuildProcessStatusBarUpdate(QObject *parent = 0);



	signals:
		void statusBarHasChanged(const QString &str);

	public slots:
		void statusBarLoadingFile();
		void statusBarBuildRunning();
		void statusBarUpdateProject();
		void statusBarCheckUpdate();

	private slots:
		void updateStatusBar(const QString &str);

	private:
		QStringList list;

	private:
		void changeStatusBar();
};

#endif // BUILDPROCESSSTATUSBARUPDATE_H
