#ifndef UPDATETAB_H
#define UPDATETAB_H

#include "updates.h"

#include <QWidget>

namespace Ui {
	class UpdateTab;
}

class UpdateTab : public QWidget
{
		Q_OBJECT

	public:
		explicit UpdateTab(QWidget *parent = 0);
		~UpdateTab();

		void updateView(Updates *updates);

		void enableButtons();
	private slots:
		void on_checkUpdateButton_clicked();

		void on_versionSelectBox_currentIndexChanged(int index);

		void on_doUpdateButton_clicked();

	public slots:
		void refreshUpdateInfo(UpdateListItem item);
		void appendLine(UpdateListItem item);

	private:
		Ui::UpdateTab *ui;
		Updates *updates;

};

#endif // UPDATETAB_H
