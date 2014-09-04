
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

#ifndef RENDERTEST_GLUE_H
#define RENDERTEST_GLUE_H

#include <QtCore>
#include <QtGui>

class Glue : public QObject
{
    Q_OBJECT

    public:
        Glue(QTextEdit *editor, QLabel *label, QComboBox *alignmentComboBox);

    public slots:
        void onTextChanged();

    private:
        QTextEdit *m_editor;
        QLabel *m_label;
        QComboBox *m_alignmentComboBox;
};

#endif
