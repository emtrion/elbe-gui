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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QRegularExpression;

class Highlighter : public QSyntaxHighlighter
{
		Q_OBJECT

	public:
		Highlighter(QTextDocument *parent = 0);

	protected:
		virtual void highlightBlock(const QString &text);

	private:
	   QTextCharFormat     m_xmlKeywordFormat;
	   QTextCharFormat     m_xmlElementFormat;
	   QTextCharFormat     m_xmlAttributeFormat;
	   QTextCharFormat     m_xmlValueFormat;
	   QTextCharFormat     m_xmlCommentFormat;

	   QList<QRegExp>      m_xmlKeywordRegexes;
	   QRegExp             m_xmlElementRegex;
	   QRegExp             m_xmlAttributeRegex;
	   QRegExp             m_xmlValueRegex;
	   QRegExp             m_xmlCommentRegex;

	private:
		void highlightByRegex(const QTextCharFormat & format,
							  const QRegExp & regex, const QString & text);

	   void setRegexes();
	   void setFormats();
};

#endif // HIGHLIGHTER_H
