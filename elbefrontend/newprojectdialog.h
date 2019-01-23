#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>

#include "userdata.h"
#include "helpers.h"


namespace Ui {
    class NewProjectDialog;
}
class NewProjectDialog: public QDialog
{
    Q_OBJECT

    public:
        NewProjectDialog();
        ~NewProjectDialog();       
        void displayDefaultPath();
        bool validation();
        bool createProject();

private slots:
        void on_DefaultPathButton_toggled(bool checked);

        void on_ProjectNameEntry_textChanged(const QString &arg1);

        void on_ProjectPathEntry_editingFinished();

        void on_dialogButtonBox_accepted();

        void on_ProjectPathEntry_textEdited(const QString &arg1);

        void on_ProjectPathBrowsButton_clicked();

private:
        Ui::NewProjectDialog *ui;
        QString newProjectPath;
        QString newProjectName;
        QString homeDirectory;
        const QString defaultPath;




};

#endif // NEWPROJECTDIALOG_H
