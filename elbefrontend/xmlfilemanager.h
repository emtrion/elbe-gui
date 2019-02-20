#ifndef XMLFILEMANAGER_H
#define XMLFILEMANAGER_H


#include <QString>

class XmlFileManager
{
	private:
		XmlFileManager();
		static XmlFileManager *instance;

		QString currentFilePath;
		QString currentFileName;
		bool isSaved;

		bool saving;

		void saveStatusChanged(bool saveStatus);

	public:
		/* Static access method. */
		static XmlFileManager *getInstance();

		QString getCurrentFilePath() const;
		void setCurrentFilePath(const QString &value);
		bool getIsSaved() const;
		void setIsSaved(bool value);
		QString getCurrentFileName() const;
		void setToDefault();
		bool getSaving() const;
		void setSaving(bool value);
};

#endif // XMLFILEMANAGER_H
