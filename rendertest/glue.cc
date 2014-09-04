
/**
 * Billboard - Low Power Mode Standby Screen for the N9
 * Webpage: http://thp.io/2012/billboard/
 * Copyright (C) 2012, 2013, 2014 Thomas Perl <thp.io/about>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "renderer.h"
#include "formatter.h"

#include "glue.h"

Glue::Glue(QTextEdit *editor, QLabel *label, QComboBox *alignmentComboBox)
    : QObject(NULL),
      m_editor(editor),
      m_label(label),
      m_alignmentComboBox(alignmentComboBox)
{
    connect(editor, SIGNAL(textChanged()),
            this, SLOT(onTextChanged()));
    connect(alignmentComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onTextChanged()));
    onTextChanged();
}

void
Glue::onTextChanged()
{
    int flags = 0;
    if (m_alignmentComboBox->currentText() == "Center") {
        flags |= Qt::AlignCenter;
    } else if (m_alignmentComboBox->currentText() == "Right") {
        flags |= Qt::AlignRight;
    }

    QMap<QString,QString> tmpl;
    tmpl["a"] = "Hello World";
    tmpl["b"] = "Alpha";

    QString text = m_editor->toPlainText();
    QSet<QString> props;
    QImage image = Renderer::render(Formatter::format(text, tmpl, &props), flags, QImage(), Qt::white, QFont("Ubuntu", 20));
    qDebug() << "Interesting properties:" << props;
    QPixmap pixmap = QPixmap::fromImage(image);
    m_label->setPixmap(pixmap);
}

