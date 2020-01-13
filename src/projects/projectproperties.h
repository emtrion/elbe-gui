#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include <QString>

class ProjectProperties
{
	public:
		ProjectProperties();

		QString name() const;
		void setName(const QString &value);
		QString version() const;
		void setVersion(const QString &version);
		QString description() const;
		void setDescription(const QString &description);
		QString buildtype() const;
		void setBuildtype(const QString &buildtype);
		QString host() const;
		void setHost(const QString &host);
		QString path() const;
		void setPath(const QString &path);
		QString proto() const;
		void setProto(const QString &proto);
		QString suite() const;
		void setSuite(const QString &suite);
		void reset();

	private:
		QString m_name;
		QString m_version;
		QString m_description;
		QString m_buildtype;
		QString m_host;
		QString m_path;
		QString m_proto;
		QString m_suite;
};

#endif // PROJECTPROPERTIES_H
