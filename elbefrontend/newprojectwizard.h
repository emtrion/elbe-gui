#ifndef NEWPROJECTWIZARD_H
#define NEWPROJECTWIZARD_H

#include <QWizard>


namespace Ui {
	class NewProjectWizard;
}

class NewProjectWizard : public QWizard
{
		Q_OBJECT

	public:
		explicit NewProjectWizard(QWidget *parent = 0);
		~NewProjectWizard();

		void displayDefaultPath();
		bool validation();
		void initSecondPageDefault();
		void initThirdPageDefault();

	private slots:
		void on_DefaultPathButton_toggled(bool checked);
		void on_ProjectNameEntry_textChanged(const QString &arg1);
		void on_ProjectPathEntry_editingFinished();
		void on_ProjectPathBrowsButton_clicked();
		void on_NewProjectWizard_accepted();
		void on_ProjectPathEntry_textChanged(const QString &arg1);
		void on_DefaultSetButton_Page3_toggled(bool checked);
		void on_DefaultSetButton_Page2_toggled(bool checked);
		void on_NewProjectWizard_currentIdChanged(int id);

	private:
		Ui::NewProjectWizard *ui;
		const QString defaultPath;

};

#endif // NEWPROJECTWIZARD_H
