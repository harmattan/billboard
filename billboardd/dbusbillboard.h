
/**
 * Billboard - Low Power Mode Standby Screen for the N9
 * Webpage: http://thp.io/2012/billboard/
 * Copyright (C) 2015 Elliot Wolk <elliot.wolk@gmail.com>
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

#ifndef DBUS_BILLBOARD_H
#define DBUS_BILLBOARD_H

#include <QObject>

class DbusBillboard : public QObject
{
    Q_OBJECT

    public:
        DbusBillboard(QObject *parent=NULL)
            : QObject(parent)
        {
        }
        ~DbusBillboard() {
        }

    public slots:
        void render() {
            emit onRender();
        }

    signals:
        void onRender();
};

#endif
