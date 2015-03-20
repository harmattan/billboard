
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

#ifndef TKLOCKINTERFACE_H
#define TKLOCKINTERFACE_H

#include <QObject>

class TklockInterface : public QObject
{
    Q_OBJECT

    public:
        TklockInterface();
        ~TklockInterface();

        bool isLocked() {
            return m_lock_mode == "locked";
        }

    public slots:
        void setLockMode(QString lock_mode) {
            if (m_lock_mode != lock_mode) {
                m_lock_mode = lock_mode;
                if (isLocked()) {
                  emit locked();
                } else {
                  emit unlocked();
                }
            }
        }

    signals:
        void locked();

        void unlocked();

    private:
        void connect();

        QString m_lock_mode;
};

#endif
