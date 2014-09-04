
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
    id: settingsHeader
    property alias text: headerCaption.text

    width: parent.width
    height: 60

    Rectangle {
        id: horizontalLine

        anchors {
            leftMargin: 18
            rightMargin: 18
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        height: 1
        color: '#666'
    }

    Label {
        id: headerCaption
        text: ''
        color: 'white'
        font.pixelSize: 32
        font.weight: Font.Light

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 7
            leftMargin: 18
            rightMargin: 18
        }
    }
}

