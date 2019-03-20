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

		// QWizardPage interface
	public:
		 bool isComplete() const; //re-implemented from QWizardPage
		 /*decides weather to enable or disable the Next/Finish-Button*/

		 QString getNewProjectName() const;

		 QString getNewProjectPath() const;

	private:
		 Ui::NewProjectWizard *ui_pointer;
		 QString newProjectPath;
		 QString newProjectName;
		 QPixmap yesIcon;
		 QPixmap errorIcon;

};

#endif // NEWPROJECTWIZARDFIRSTPAGE_H
