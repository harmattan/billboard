
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

#include <QtCore>
#include <QtGui>

#include "glue.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Billboard Renderer Tester");
    QHBoxLayout layout;

    QWidget leftPane;
    QVBoxLayout leftPaneLayout;

    QWidget settings;
    QHBoxLayout settingsLayout;

    QComboBox alignmentComboBox;
    alignmentComboBox.addItem("Left");
    alignmentComboBox.addItem("Center");
    alignmentComboBox.addItem("Right");
    settingsLayout.addWidget(&alignmentComboBox);

    settings.setLayout(&settingsLayout);
    leftPaneLayout.addWidget(&settings);

    QTextEdit edit;
    edit.setText("Hello world!\n{{=0.7}}\n{{green}}Das ist ein Test\n{{red}}Mehr lange Zeilen sind sehr, sehr, sehr, sehr, sehr, sehr, sehr, sehr, sehr, sehr, sehr, sehr, gut\nEins Zwei Drei\n{{yellow}}Hey ho <</usr/share/themes/blanco/meegotouch/icons/icon-s-status-bluetooth.png>>");
    leftPaneLayout.addWidget(&edit);

    leftPane.setLayout(&leftPaneLayout);
    layout.addWidget(&leftPane);

    QLabel label;
    layout.addWidget(&label);

    Glue glue(&edit, &label, &alignmentComboBox);

    window.setLayout(&layout);
    window.show();

    return app.exec();
}

