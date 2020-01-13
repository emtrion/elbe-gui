#ifndef UPDATELISTITEM_H
#define UPDATELISTITEM_H

#include <QListWidgetItem>
#include <QString>



class UpdateListItem
{
	public:
		UpdateListItem(	QString buildNo, QString version, QString date, QString info);

		QString toString();

		void setInfo(const QString &value);

		QString getVersion() const;

	private:
		QString buildNo;
		QString version;
		QString date;
		QString info;


};

#endif // UPDATELISTITEM_Hs
