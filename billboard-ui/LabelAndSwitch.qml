
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

import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: labelAndSwitch
    property string text: ''
    property alias checked: check.checked

    width: parent.width
    height: 80

    Label {
        id: label
        text: labelAndSwitch.text
        font.bold: true

        anchors {
            leftMargin: 18
            left: parent.left
            right: check.left
            verticalCenter: parent.verticalCenter
        }
    }

    Switch {
        id: check

        anchors {
            right: parent.right
            rightMargin: 16
            verticalCenter: parent.verticalCenter
        }
    }
}

