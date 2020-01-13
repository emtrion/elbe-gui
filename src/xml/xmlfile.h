#ifndef XMLFILE_H
#define XMLFILE_H


#include <QString>

class XmlFile
{	
	public:
		/* Static access method. */
		static XmlFile *getInstance();

		QString currentFilePath() const;
		void setCurrentFilePath(const QString &value);
		bool isSaved() const;
		void setIsSaved(bool value);
		QString currentFileName() const;
		void setCurrentFileName(const QString &currentFileName);
		bool saving() const;
		void setSaving(bool value);
		bool isOpen() const;
		void setIsOpen(bool value);
		void setToDefault();

	private:
		XmlFile();
		static XmlFile *instance;

		QString m_currentFilePath;
		QString	m_currentFileName;
		bool m_isSaved;
		bool m_saving;
		bool m_isOpen;

	private:
		void saveStatusChanged(bool saveStatus);



};

#endif // XMLFILEMANAGER_H
