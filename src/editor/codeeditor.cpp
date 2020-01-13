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

#include "codeeditor.h"

#include <QtWidgets>
#include <QFontMetrics>

#include "highlighter.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

//	this->font.setFixedPitch(true);
	const int tabStop = 4;
	QFontMetrics metrics(this->font());
	this->setTabStopDistance(tabStop * metrics.width(' '));

	this->setMinimumSize(5,5);

	highlighter = new Highlighter(this->document());

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
//    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
//    highlightCurrentLine();

}



int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
	while ( max >= 10 ) {
		max /= 10;
        ++digits;
    }

	int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
	if ( dy ) {
		lineNumberArea->scroll(0, dy);
	} else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
	}
	if ( rect.contains(viewport()->rect()) ) {
		updateLineNumberAreaWidth(0);
	}
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

	if ( !isReadOnly() ) {
		QTextEdit::ExtraSelection selection;
		QColor lineColor = QColor(Qt::gray).lighter(50);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

    QColor color;
	color.setNamedColor("#404244");
    painter.fillRect(event->rect(), QColor(color));

    QTextBlock block = firstVisibleBlock();
    QTextCursor cursor = this->textCursor();

    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();


	while ( block.isValid() && top <= event->rect().bottom() ) {
		if ( block.isVisible() && bottom >= event->rect().top() ) {
           QString number = QString::number(blockNumber + 1);

           //lineNumberHighlighting
           int selectedBlock = cursor.blockNumber();
		   if ( selectedBlock == blockNumber ) {
			   color.setNamedColor("#C3B541"); //yellowish
			   painter.setPen(color);
           } else {
               painter.setPen(Qt::gray);
           }

           painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::setLineNumberAreaVisible(bool visible)
{
	if ( visible ) {
		lineNumberArea->show();
	} else {
		lineNumberArea->hide();
	}
}



