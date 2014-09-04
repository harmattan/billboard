
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

PageStackWindow {
    initialPage: Page {
        id: mainPage
        orientationLock: PageOrientation.LockPortrait

        tools: ToolBarLayout {
            ToolIcon {
                anchors.right: parent.right
                iconId: "icon-m-toolbar-view-menu-white"
                onClicked: contextMenu.open()
            }
        }

        Flickable {
            anchors.fill: parent
            contentHeight: column.height
            contentWidth: column.width

            Column {
                id: column
                spacing: 10
                width: mainPage.width

                SettingsHeader { text: qsTr('General') }

                LabelAndSwitch {
                    id: enableSwitch
                    text: qsTr('Enable Billboard')
                    onCheckedChanged: controller.enabled = checked
                }

                SettingsHeader { text: qsTr('Content'); visible: controller.enabled }

                Row {
                    id: buttons
                    visible: controller.enabled
                    anchors {
                        leftMargin: 18
                        rightMargin: 18
                        left: parent.left
                        right: parent.right
                    }

                    spacing: 10

                    height: 80

                    Button {
                        id: first
                        text: qsTr('Insert')
                        width: (mainPage.width - 10 - 18*2) / 2
                        anchors.verticalCenter: parent.verticalCenter

                        onClicked: {
                            insertDialog.selectedIndex = -1
                            insertDialog.open()
                        }
                    }

                    Button {
                        text: qsTr('Presets')
                        width: first.width
                        anchors.verticalCenter: parent.verticalCenter

                        onClicked: {
                            presetDialog.selectedIndex = -1
                            presetDialog.open()
                        }
                    }
                }

                TextArea {
                    id: textArea
                    visible: controller.enabled

                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.imNoAutoUppercase

                    height: 200

                    anchors {
                        leftMargin: 18
                        rightMargin: 18
                        left: parent.left
                        right: parent.right
                    }

                    onTextChanged: controller.text = text
                }

                SettingsHeader { text: qsTr('Preview'); visible: controller.enabled }

                Image {
                    id: preview
                    source: 'file://' + controller.logo + '#' + Qt.formatDateTime(new Date(), 'yyMMddhhmmsszzz')
                    visible: controller.enabled

                    anchors {
                        margins: 10
                        horizontalCenter: parent.horizontalCenter
                    }
                }

                SettingsHeader { text: qsTr('Appearance'); visible: controller.enabled }

                LabelAndSwitch {
                    id: otherEnableSwitch
                    text: qsTr('Show other logos')
                    onCheckedChanged: controller.otherEnabled = checked
                    visible: controller.enabled
                }

                LooksLikeAComboBox {
                    label: qsTr('Text color')
                    value: colorDialog.model.get(colorDialog.selectedIndex).name
                    onClicked: colorDialog.open()
                    visible: controller.enabled
                }

                LooksLikeAComboBox {
                    label: qsTr('Text size')
                    value: sizeDialog.model.get(sizeDialog.selectedIndex).name
                    onClicked: sizeDialog.open()
                    visible: controller.enabled
                }

                LooksLikeAComboBox {
                    label: qsTr('Font family')
                    value: fontDialog.model.get(fontDialog.selectedIndex).name
                    onClicked: fontDialog.open()
                    visible: controller.enabled
                }

                LooksLikeAComboBox {
                    label: qsTr('Text alignment')
                    value: alignmentDialog.model.get(alignmentDialog.selectedIndex).name
                    onClicked: alignmentDialog.open()
                    visible: controller.enabled
                }

                Item { width: 1; height: 10 }
            }
        }

        SelectionDialog {
            id: insertDialog
            titleText: qsTr('Insert new field')

            model: ListModel { id: insertDialogModel }

            Component.onCompleted: {
                insertDialogModel.append({ name: qsTr('Compact date'), value: '{date}' });
                insertDialogModel.append({ name: qsTr('Full date'), value: '{full-date}' });

                insertDialogModel.append({ name: qsTr('Weekday name'), value: '{weekday}' });
                insertDialogModel.append({ name: qsTr('Day of month'), value: '{day}' });
                insertDialogModel.append({ name: qsTr('Month name'), value: '{month}' });
                insertDialogModel.append({ name: qsTr('Short month name'), value: '{short-month}' });
                insertDialogModel.append({ name: qsTr('Month'), value: '{month-number}' });
                insertDialogModel.append({ name: qsTr('Year'), value: '{year}' });
                insertDialogModel.append({ name: qsTr('Week number'), value: '{week}' });

                insertDialogModel.append({ name: qsTr('Song artist'), value: '{song-artist}' });
                insertDialogModel.append({ name: qsTr('Song title'), value: '{song-title}' });
                insertDialogModel.append({ name: qsTr('Song "Artist - Title"'), value: '{song}' });

                insertDialogModel.append({ name: qsTr('Media volume (text)'), value: '{volume}' });
                insertDialogModel.append({ name: qsTr('Media volume (bar)'), value: '{volume-bar}' });
                insertDialogModel.append({ name: qsTr('Playback volume bar'), value: '{song-volume-bar}' });

                insertDialogModel.append({ name: qsTr('Cell network name'), value: '{cell-network}' });
                insertDialogModel.append({ name: qsTr('Internet connection'), value: '{network-name}' });
                insertDialogModel.append({ name: qsTr('IP address'), value: '{ip}' });

                insertDialogModel.append({ name: qsTr('Signal strength'), value: '{cell-signal-strength}' });
                insertDialogModel.append({ name: qsTr('Signal strength (bars)'), value: '{cell-signal-bars}' });

                insertDialogModel.append({ name: qsTr('Battery charge (text)'), value: '{battery}' });
                insertDialogModel.append({ name: qsTr('Battery charge (bar)'), value: '{battery-bar}' });
                insertDialogModel.append({ name: qsTr('Battery charge (icon)'), value: '{battery-icon}' });
                insertDialogModel.append({ name: qsTr('Colored battery bar'), value: '{color-battery-bar}' });
                insertDialogModel.append({ name: qsTr('Active profile'), value: '{profile}' });

                insertDialogModel.append({ name: qsTr('Upcoming events'), value: '{events}' });
                insertDialogModel.append({ name: qsTr('Today\'s events'), value: '{events-today}' });

                insertDialogModel.append({ name: qsTr('MeeCast station'), value: '{meecast-station}' });
                insertDialogModel.append({ name: qsTr('MeeCast temperature'), value: '{meecast-temperature}' });
                insertDialogModel.append({ name: qsTr('MeeCast description'), value: '{meecast-description}' });
                insertDialogModel.append({ name: qsTr('MeeCast icon'), value: '<<{meecast-icon-src}>>' });

                insertDialogModel.append({ name: qsTr('IM presence status'), value: '{presence}' });
                insertDialogModel.append({ name: qsTr('Wazapp online status'), value: '{wazapp-status}' });

                insertDialogModel.append({ name: qsTr('USSD Balance'), value: '{ussd-balance}' });
                insertDialogModel.append({ name: qsTr('USSD Balance (2nd line)'), value: '{ussd-balance-2}' });
                insertDialogModel.append({ name: qsTr('USSD Balance (3rd line)'), value: '{ussd-balance-3}' });

                insertDialogModel.append({ name: qsTr('Brackets: {}'), value: '{}' });
                insertDialogModel.append({ name: qsTr('Brackets: {{}}'), value: '{{}}' });
                insertDialogModel.append({ name: qsTr('Embed image: <<>>'), value: '<</path/to/image.png>>' });
                insertDialogModel.append({ name: qsTr('Embed image (dithered): <<#>>'), value: '<</path/to/image.png#>>' });
            }

            onAccepted: {
                var txt = insertDialog.model.get(insertDialog.selectedIndex).value
                var cursorPosition = textArea.cursorPosition + txt.length
                textArea.text = textArea.text.substring(0, textArea.cursorPosition) + txt + textArea.text.substring(textArea.cursorPosition)
                textArea.cursorPosition = cursorPosition
                textArea.forceActiveFocus()
            }
        }

        SelectionDialog {
            id: presetDialog
            titleText: qsTr('Select preset')

            model: ListModel { id: presetDialogModel }

            Component.onCompleted: {
                presetDialogModel.append({ name: qsTr('Default'), value: "{full-date}" });
                presetDialogModel.append({ name: qsTr('Empty'), value: " " });
                presetDialogModel.append({ name: qsTr('Default with song'), value: "{full-date}\n{song?Now Playing: }{song-title}" });
                presetDialogModel.append({ name: qsTr('Full device status'), value: "{full-date}\nBattery: {battery}%\n{song!No media}{song?Now playing: }{song-artist}{song?\n}{song-title}\n{cell-network}{cell-network!No network} // {network-name?AP: }{network-name}{network-name!Offline}" });
                presetDialogModel.append({ name: qsTr('Date, battery and song (icons)'), value: '{weekday} {day} {month}, ⚡ {battery}%\n\n{song? ★ }{song-artist}{song?\n☊ }{song-title}' });
            }

            onAccepted: {
                textArea.text = presetDialog.model.get(presetDialog.selectedIndex).value
            }
        }

        SelectionDialog {
            id: colorDialog
            titleText: qsTr('Text color')

            model: ListModel { id: colorDialogModel }

            Component.onCompleted: {
                colorDialogModel.append({ name: qsTr('White'), value: "white" });
                colorDialogModel.append({ name: qsTr('Red'), value: "red" });
                colorDialogModel.append({ name: qsTr('Green'), value: "green" });
                colorDialogModel.append({ name: qsTr('Blue'), value: "blue" });
                colorDialogModel.append({ name: qsTr('Yellow'), value: "yellow" });
                colorDialogModel.append({ name: qsTr('Cyan'), value: "cyan" });
                colorDialogModel.append({ name: qsTr('Magenta'), value: "magenta" });
            }

            onAccepted: {
                controller.color = colorDialog.model.get(colorDialog.selectedIndex).value
            }
        }

        SelectionDialog {
            id: sizeDialog
            titleText: qsTr('Text size')

            model: ListModel { id: sizeDialogModel }

            Component.onCompleted: {
                sizeDialogModel.append({ name: qsTr('Smaller'), value: 14 });
                sizeDialogModel.append({ name: qsTr('Small'), value: 18 });
                sizeDialogModel.append({ name: qsTr('Normal'), value: 22 });
                sizeDialogModel.append({ name: qsTr('Big'), value: 28 });
                sizeDialogModel.append({ name: qsTr('Bigger'), value: 32 });
                sizeDialogModel.append({ name: qsTr('Very big'), value: 50 });
                sizeDialogModel.append({ name: qsTr('Crazy big'), value: 60 });
            }

            onAccepted: {
                controller.size = sizeDialog.model.get(sizeDialog.selectedIndex).value
            }
        }

        SelectionDialog {
            id: fontDialog
            titleText: qsTr('Font family')

            model: ListModel { id: fontDialogModel }

            Component.onCompleted: {
                fontDialogModel.append({ name: qsTr('Pure'), value: "Nokia Pure" });
                fontDialogModel.append({ name: qsTr('Sans'), value: "Nokia Sans" });
                fontDialogModel.append({ name: qsTr('Monospace'), value: "Andale Mono" });
            }

            onAccepted: {
                controller.font = fontDialog.model.get(fontDialog.selectedIndex).value
            }
        }

        SelectionDialog {
            id: alignmentDialog
            titleText: qsTr('Text alignment')

            model: ListModel { id: alignmentDialogModel }

            Component.onCompleted: {
                alignmentDialogModel.append({ name: qsTr('Left'), value: "left" });
                alignmentDialogModel.append({ name: qsTr('Right'), value: "right" });
                alignmentDialogModel.append({ name: qsTr('Center'), value: "center" });
            }

            onAccepted: {
                controller.alignment = alignmentDialog.model.get(alignmentDialog.selectedIndex).value
            }
        }

        ContextMenu {
            id: contextMenu
            MenuLayout {
                MenuItem {
                    text: qsTr('Get more apps')
                    onClicked: {
                        Qt.openUrlExternally('http://store.ovi.com/publisher/Thomas+Perl/')
                        contextMenu.close()
                    }
                }

                MenuItem {
                    text: qsTr('Help / About')
                    onClicked: {
                        contextMenu.close()
                        pageStack.push(aboutPage)
                    }
                }
            }
        }
    }

    Page {
        id: aboutPage
        orientationLock: PageOrientation.LockPortrait

        Flickable {
            id: aboutFlickable

            width: parent.width
            contentHeight: aboutColumn.height
            anchors.fill: parent
            anchors.margins: 20

            Column {
                id: aboutColumn
                width: parent.width
                spacing: 5

                Image {
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: 'logo.png'
                }

                Label {
                    text: 'Billboard 2.0.0'
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 30
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: 'http://thp.io/2012/billboard/'
                    MouseArea {
                        anchors.fill: parent
                        anchors.margins: -10
                        onClicked: Qt.openUrlExternally(parent.text)
                    }
                }

                Label {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: {
                      [
                        '<br><br>',
                        qsTr('Show useful information in the low power mode screen.'),
                        '<br><br>',
                        qsTr('Syntax information:'),
                        '<br><b>{property}</b> - ', qsTr('Insert property'),
                        '<br><b>{property?text}</b> - ', qsTr('Insert "text" when property is not empty'),
                        '<br><b>{property!text}</b> - ', qsTr('Insert "text" when property is empty'),
                        '<br><br>',
                        qsTr('Examples:'),
                        '<br><b>{song!', qsTr('No song'), '}</b> - ', qsTr('Insert "No song" when no song is selected'),
                        '<br><b>{date}</b> - ', qsTr('Insert the current date (short format)'),
                        '<br><br>',
                        qsTr('Advanced features:'),
                        '<br><b>{script:uptime}</b> - ', qsTr('Insert output of "uptime" command'),
                        '<br><b>{script:/home/user/test.py}</b> - ', qsTr('Insert output of "/home/user/test.py" script'),
                        '<br><b>{{red}}</b> - ', qsTr('Use a color name (%1) to set the inline color').arg('red, green, blue, cyan, magenta, yellow'),
                        '<br><b>{{}}</b> - ', qsTr('Reset the inline color to the default color'),
                        '<br><b>&lt;&lt;/path/to/image.png&gt;&gt;</b> - ', qsTr('Insert image (normal)'),
                        '<br><b>&lt;&lt;/path/to/image.png#&gt;&gt;</b> - ', qsTr('Insert image (with dithering)'),
                        '<br><br>',
                        qsTr('Author: %1').arg(' Thomas Perl (thp.io)'),
                        '<br><br>',
                        'Released under the terms of the GNU General Public License, version 3 or later.'
                      ].join('')
                    }
                }

            }
        }

        ScrollDecorator {
            flickableItem: aboutFlickable
        }

        tools: ToolBarLayout {
            ToolIcon {
                iconId: "icon-m-toolbar-back-white"
                onClicked: pageStack.pop()
            }
        }

    }

    Component.onCompleted: {
        theme.inverted = true;
        textArea.text = controller.text;
        enableSwitch.checked = controller.enabled;
        otherEnableSwitch.checked = controller.otherEnabled;

        var colorFound = false;
        for (var i=0; i<colorDialog.model.count; i++) {
            if (colorDialog.model.get(i).value === controller.color) {
                colorDialog.selectedIndex = i;
                colorFound = true;
                break;
            }
        }
        if (!colorFound) {
            colorDialog.selectedIndex = 0;
            colorDialog.accepted();
        }

        var sizeFound = false;
        for (var i=0; i<sizeDialog.model.count; i++) {
            if (parseInt(sizeDialog.model.get(i).value) === controller.size) {
                sizeDialog.selectedIndex = i;
                sizeFound = true;
                break;
            }
        }
        if (!sizeFound) {
            sizeDialog.selectedIndex = 2;
            sizeDialog.accepted();
        }

        var fontFound = false;
        for (var i=0; i<fontDialog.model.count; i++) {
            if (fontDialog.model.get(i).value === controller.font) {
                fontDialog.selectedIndex = i;
                fontFound = true;
                break;
            }
        }
        if (!fontFound) {
            fontDialog.selectedIndex = 0;
            fontDialog.accepted();
        }

        var alignmentFound = false;
        for (var i=0; i<alignmentDialog.model.count; i++) {
            if (alignmentDialog.model.get(i).value === controller.alignment) {
                alignmentDialog.selectedIndex = i;
                alignmentFound = true;
                break;
            }
        }
        if (!alignmentFound) {
            alignmentDialog.selectedIndex = 1; /* right */
            alignmentDialog.accepted();
        }
    }
}

