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

#include "highlighter.h"

#include <QSyntaxHighlighter>
#include <QTextDocument>

Highlighter::Highlighter(QTextDocument *parent) :
	QSyntaxHighlighter(parent)
{
	setRegexes();
	setFormats();
}

void Highlighter::highlightBlock(const QString &text)
{
	int xmlElementIndex = m_xmlElementRegex.indexIn(text);

	while (xmlElementIndex >= 0) {
		int matchedPos = m_xmlElementRegex.pos(1);
		int matchedLength = m_xmlElementRegex.cap(1).length();
		setFormat(matchedPos, matchedLength, m_xmlElementFormat);

		xmlElementIndex = m_xmlElementRegex.indexIn(text, matchedPos + matchedLength);
	}

	typedef QList<QRegExp>::const_iterator Iter;
	Iter xmlKeywordRegexesEnd = m_xmlKeywordRegexes.end();
	for(Iter it = m_xmlKeywordRegexes.begin(); it != xmlKeywordRegexesEnd; ++it) {
		const QRegExp &regex = *it;
		highlightByRegex(m_xmlKeywordFormat, regex, text);
	}

	highlightByRegex(m_xmlAttributeFormat, m_xmlAttributeRegex, text);
	highlightByRegex(m_xmlCommentFormat, m_xmlCommentRegex, text);
	highlightByRegex(m_xmlValueFormat, m_xmlValueRegex, text);

}

void Highlighter::highlightByRegex(const QTextCharFormat &format,
const QRegExp &regex, const QString &text)
{
	int index = regex.indexIn(text);

	while(index >= 0)
	{
		int matchedLength = regex.matchedLength();
		setFormat(index, matchedLength, format);

		index = regex.indexIn(text, index + matchedLength);
	}
}

void Highlighter::setRegexes()
{
	m_xmlElementRegex.setPattern("<[\\s]*[/]?[\\s]*([^\\n][\\w\:\-]*)(?=[\\s/>])");
	m_xmlAttributeRegex.setPattern("\\w+(?=\\=)");
	m_xmlValueRegex.setPattern("\"[^\\n\"]+\"(?=[\\s/>])");
	m_xmlCommentRegex.setPattern("<!--[^\\n]*-->");

	m_xmlKeywordRegexes = QList<QRegExp>() << QRegExp("<\\?") << QRegExp("/>")
	<< QRegExp(">") << QRegExp("<") << QRegExp("</")
	<< QRegExp("\\?>");
}

void Highlighter::setFormats()
{
	QColor color;
	color.setNamedColor("#45c6d6"); //blue for dark
//	color.setNamedColor();
	m_xmlKeywordFormat.setForeground(color);//blue
//	m_xmlKeywordFormat.setFontWeight(QFont::Bold);

	m_xmlElementFormat.setForeground(color);//blue
//	m_xmlElementFormat.setFontWeight(QFont::Bold);

	color.setNamedColor("#d6cf9a"); //normal text color
	m_xmlAttributeFormat.setForeground(color);
//	m_xmlAttributeFormat.setFontWeight(QFont::Bold);
	m_xmlAttributeFormat.setFontItalic(true);

	color.setNamedColor("#CB4B16");//red
	m_xmlValueFormat.setForeground(color); //red

	m_xmlCommentFormat.setForeground(Qt::gray);
}
