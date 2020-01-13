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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

#include "highlighter.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QFontMetrics;
class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

	public:
		CodeEditor(QWidget *parent = 0);

		void lineNumberAreaPaintEvent(QPaintEvent *event);
		int lineNumberAreaWidth();
		void setLineNumberAreaVisible(bool visible);

	protected:
		void resizeEvent(QResizeEvent *event) override;

	private slots:
		void updateLineNumberAreaWidth(int newBlockCount);
		void highlightCurrentLine();
		void updateLineNumberArea(const QRect &, int);

	private:
		QWidget *lineNumberArea;
		Highlighter *highlighter;
	};


class LineNumberArea : public QWidget
{
	public:
		LineNumberArea(CodeEditor *editor) : QWidget(editor) {
			codeEditor = editor;
		}

		QSize sizeHint() const override {
			return QSize(codeEditor->lineNumberAreaWidth(), 0);
		}

	protected:
		void paintEvent(QPaintEvent *event) override {
			codeEditor->lineNumberAreaPaintEvent(event);
		}

	private:
		CodeEditor *codeEditor;
};
#endif // CODEEDITOR_H
