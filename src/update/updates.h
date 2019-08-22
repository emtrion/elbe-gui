#ifndef UPDATES_H
#define UPDATES_H

#include "updatelistitem.h"

#include <QFile>
#include <QObject>
#include "updatethread.h"
#include "checkupdatethread.h"

class Updates : public QObject
{
		Q_OBJECT

	public:
		Updates(QString);

		void checkForUpdates();
		void makeUpdate();
		void updatesAvailable(int avialablePkgs);

		QList<UpdateListItem> getUpdateList() const;


		void makeInitialEntry();
		int nextUpdateNumber();

		void initUpdateList();
	signals:
		void notifyViewAfterCheckUpdates(UpdateListItem);
		void notifyViewAfterBuildUpdates(UpdateListItem);

	public slots:
		void handleCheckResult(QString result);

		void handleUpdateResult();
		void showErrorMessage(QStringList result);
	private:

		UpdateListItem makeListEntry(QStringList list);
		QList<UpdateListItem> updateList;
		UpdateListItem modifyLastEntry(QString availableUpdates);

		UpdateThread *uthread;
		CheckUpdateThread *cuthread;


		QFile *updateHistoryFile;
		void saveListEntryToHistory();
};

#endif // UPDATES_H

