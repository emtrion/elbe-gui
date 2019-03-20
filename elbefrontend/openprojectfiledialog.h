#ifndef OPENPROJECTFILEDIALOG_H
#define OPENPROJECTFILEDIALOG_H



#include <QFileDialog>

class QToolButton;

class OpenProjectFileDialog : public QFileDialog
{
	Q_OBJECT

	public:
		OpenProjectFileDialog(QWidget* parent, QString directory);

		virtual ~OpenProjectFileDialog(){}

	protected slots:
		void fileSelectionChanged(const QString &file);
		void backOrForClicked();

	protected:
		bool checkIfProject(const QString &file);
		bool eventFilter(QObject *obj, QEvent *event) override;


	private:
		QPushButton *myOpenButton;
		QToolButton *backButton;
		QToolButton *forwardButton;
		void updateCurrentSelection(QString file);
		QFileInfo currentSelection;
		bool isSelectionValid();

};

#endif // OPENPROJECTFILEDIALOG_H
