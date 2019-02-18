#ifndef OPENPROJECTFILEDIALOG_H
#define OPENPROJECTFILEDIALOG_H



#include <QFileDialog>

class OpenProjectFileDialog : public QFileDialog
{
	Q_OBJECT

	public:
		OpenProjectFileDialog(QWidget* parent, QString directory);

		virtual ~OpenProjectFileDialog(){}

	protected slots:
//		void fileSelectionChanged(const QString &file);

	protected:
		bool checkIfProject(const QString &file);


	private:
		QPushButton *myOpenButton;

};

#endif // OPENPROJECTFILEDIALOG_H
