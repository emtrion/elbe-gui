#ifndef NEWPROJECTWIZARDFIRSTPAGE_H
#define NEWPROJECTWIZARDFIRSTPAGE_H

#include <QWizardPage>

namespace Ui {
	class NewProjectWizard;
}
/*NewProjectWizardFirstPage is needed to represent the first page of the NewProjectWizard.
 *validation through the next/finish-button is offered by the basic QWizardPage-class in form of mandatory fields.
 *To get a more comlex validation one needs to re-implement the isComplete()-method*/
class NewProjectWizardFirstPage : public QWizardPage
{
	Q_OBJECT

	public:
		NewProjectWizardFirstPage();
		~NewProjectWizardFirstPage();

		void setFirstPageUiPointer(Ui::NewProjectWizard *ui_pointer);
		void connectSignals();
		bool isComplete() const;
		QString newProjectName() const;
		QString newProjectPath() const;

	private:
		 Ui::NewProjectWizard *ui_pointer;
		 QString m_newProjectPath;
		 QString m_newProjectName;
		 QPixmap acceptIcon;
		 QPixmap errorIcon;
};

#endif // NEWPROJECTWIZARDFIRSTPAGE_H
