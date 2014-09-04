
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

#ifndef VOLUMEINTERFACE_H
#define VOLUMEINTERFACE_H

#include <QObject>
#include <dbus/dbus.h>

class VolumeInterface : public QObject
{
    Q_OBJECT

    public:
        VolumeInterface();
        ~VolumeInterface();

        qreal volume() {
            connect();
            return m_volume;
        }

        void setVolume(qreal volume) {
            if (m_volume != volume) {
                m_volume = volume;
                emit changed();
            }
        }

    signals:
        void changed();

    private:
        void connect();

        DBusConnection  *m_dbus_conn;
        qreal m_volume;
};

#endif
