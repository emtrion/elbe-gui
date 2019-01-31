#ifndef NEWPROJECTWIZARDDEFAULTPAGE_H
#define NEWPROJECTWIZARDDEFAULTPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QObjectList>
#include "newprojectwizard.h"

namespace Ui {
	class NewProjectWizard;
}

class NewProjectWizardDefaultpage : public QWizardPage
{
		Q_OBJECT

	public:
		NewProjectWizardDefaultpage();
		~NewProjectWizardDefaultpage();
		void setDefaultPageUiPointer(Ui::NewProjectWizard *ui_pointer);
		void initializePage(int pageId);
		void connectFields();
		void displayDefaultSettings();
		QList<QLineEdit*> projectSettingInputFields;
//		QList<QString> projectSettingInputStrings;
		QList<QString> defaultSettings;

		QString name;


		// QWizardPage interface
	public:
		virtual bool isComplete() const override;


	private:
		Ui::NewProjectWizard *ui_pointer;

};

#endif // NEWPROJECTWIZARDDEFAULTPAGE_H
