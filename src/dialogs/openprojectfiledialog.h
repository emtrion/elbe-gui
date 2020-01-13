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

	protected:
		bool checkIfProject(const QString &file);
		bool eventFilter(QObject *obj, QEvent *event) override;

	protected slots:
		void fileSelectionChanged(const QString &file);
		void backOrForClicked();

	private:
		QPushButton *myOpenButton;
		QToolButton *backButton;
		QToolButton *forwardButton;
		QFileInfo currentSelection;

	private:
		void updateCurrentSelection(QString file);
		bool isSelectionValid();

};

#endif // OPENPROJECTFILEDIALOG_H
