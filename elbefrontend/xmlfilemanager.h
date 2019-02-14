#ifndef XMLFILEMANAGER_H
#define XMLFILEMANAGER_H

#include <QString>

class XmlFileManager
{
	private:
		XmlFileManager();
		static XmlFileManager *instance;

		QString currentFilePath;
		bool isSaved;

	public:
		/* Static access method. */
		static XmlFileManager *getInstance();

		QString getCurrentFilePath() const;
		void setCurrentFilePath(const QString &value);
		bool getIsSaved() const;
		void setIsSaved(bool value);
};

#endif // XMLFILEMANAGER_H
