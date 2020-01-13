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

#include "projectproperties.h"

ProjectProperties::ProjectProperties()
{

}

QString ProjectProperties::name() const
{
	return m_name;
}

void ProjectProperties::setName(const QString &name)
{
	m_name = name;
}

QString ProjectProperties::version() const
{
	return m_version;
}

void ProjectProperties::setVersion(const QString &version)
{
	m_version = version;
}

QString ProjectProperties::description() const
{
	return m_description;
}

void ProjectProperties::setDescription(const QString &description)
{
	m_description = description;
}

QString ProjectProperties::buildtype() const
{
	return m_buildtype;
}

void ProjectProperties::setBuildtype(const QString &buildtype)
{
	m_buildtype = buildtype;
}

QString ProjectProperties::host() const
{
	return m_host;
}

void ProjectProperties::setHost(const QString &host)
{
	m_host = host;
}

QString ProjectProperties::path() const
{
	return m_path;
}

void ProjectProperties::setPath(const QString &path)
{
	m_path = path;
}

QString ProjectProperties::proto() const
{
	return m_proto;
}

void ProjectProperties::setProto(const QString &proto)
{
	m_proto = proto;
}

QString ProjectProperties::suite() const
{
	return m_suite;
}

void ProjectProperties::setSuite(const QString &suite)
{
	m_suite = suite;
}


void ProjectProperties::reset()
{
	m_name = "";
	m_version = "";
	m_description = "";
	m_buildtype = "";
	m_host = "";
	m_path = "";
	m_proto = "";
	m_suite = "";
}
