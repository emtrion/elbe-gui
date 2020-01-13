/* * ELBE-GUI - Graphical Frontend for the Embedded Linux Build Environmend (ELBE)
 * Copyright (C) 2019-2020 emtrion GmbH
 *
 * This file is part of ELBE-GUI.
 *
 * ELBE-GUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ELBE-GUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ELBE-GUI.  If not, see <http://www.gnu.org/licenses/>.
 */

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
