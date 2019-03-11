#ifndef BUILDPROCESSSTARTDIALOG_H
#define BUILDPROCESSSTARTDIALOG_H

#include <QCheckBox>
#include <QDialog>

namespace Ui {
	class BuildProcessStartDialog;
}

class BuildProcessStartDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit BuildProcessStartDialog(QWidget *parent = 0);
		~BuildProcessStartDialog();

	private slots:
		void on_buttonBox_accepted();

	private:
		Ui::BuildProcessStartDialog *ui;
		QList<QCheckBox *> checkBoxList;
		QStringList checkedBoxes;
};

#endif // BUILDPROCESSSTARTDIALOG_H
