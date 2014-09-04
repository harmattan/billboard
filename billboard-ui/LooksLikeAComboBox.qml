
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

Rectangle {
    id: looksLikeAComboBox

    width: 480
    height: 80
    color: 'black'
    property alias label: captionLabel.text
    property alias value: valueLabel.text
    signal clicked

    Column {
        anchors {
            left: parent.left
            leftMargin: 18
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        Label {
            id: captionLabel
            color: mouseArea.pressed?'#888':'white'
            font.bold: true
        }

        Label {
            id: valueLabel
            color: mouseArea.pressed?'#888':'#888'
        }
    }

    Image {
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 18
        }

        source: 'image://theme/meegotouch-combobox-indicator-inverted' + (mouseArea.pressed?'-pressed':'')
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: looksLikeAComboBox.clicked()

        Rectangle {
            anchors.fill: parent
            visible: parent.pressed
            opacity: .5
            color: '#aaa'
        }
    }

    Component.onCompleted: theme.inverted = true
}
