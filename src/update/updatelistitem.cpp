#include "updatelistitem.h"

UpdateListItem::UpdateListItem(	QString buildNo, QString version, QString date, QString info)
{
	this->buildNo = buildNo;
	this->version = version;
	this->date = date;
	this->info = info;
}

QString UpdateListItem::toString()
{
	return buildNo+"\t\t"+version+"\t\t"+date+"\t\t"+info;
}

void UpdateListItem::setInfo(const QString &value)
{
	info = value;
}

QString UpdateListItem::getVersion() const
{

	return version.section(" ", -1);
}

