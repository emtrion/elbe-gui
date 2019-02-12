#include "highlighter.h"

#include <QSyntaxHighlighter>
#include <QTextDocument>

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
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
	m_xmlElementRegex.setPattern("<[\\s]*[/]?[\\s]*([^\\n]\\w*)(?=[\\s/>])");
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
	color.setNamedColor("#45c6d6"); //blue
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
