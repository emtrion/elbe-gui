#ifndef NEWPROJECTWIZARDDEFAULTPAGE_H
#define NEWPROJECTWIZARDDEFAULTPAGE_H

#include <QWizardPage>

class QLineEdit;
class NewProjectWizard;

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
		virtual bool isComplete() const override;
		QList<QLineEdit *> projectSettingInputFields() const;
		QList<QString> defaultSettings() const;
		void setDefaultSettings(const QList<QString> &defaultSettings);

	private:
		Ui::NewProjectWizard *ui_pointer;
		QList<QLineEdit*> m_projectSettingInputFields;
		QList<QString> m_defaultSettings;
		QString m_name;
};

#endif // NEWPROJECTWIZARDDEFAULTPAGE_H
