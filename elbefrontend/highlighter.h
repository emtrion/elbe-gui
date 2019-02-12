#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
		Q_OBJECT

	public:
		Highlighter(QTextDocument *parent = 0);
	protected:
		virtual void highlightBlock(const QString &text);

	private:
		void highlightByRegex(const QTextCharFormat & format,
	   const QRegExp & regex, const QString & text);

	   void setRegexes();
	   void setFormats();

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
};

#endif // HIGHLIGHTER_H
