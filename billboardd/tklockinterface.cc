
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

#include "tklockinterface.h"

#include <QDBusConnection>

TklockInterface::TklockInterface() :
    QObject(NULL),
    m_lock_mode("")
{
    connect();
}

TklockInterface::~TklockInterface ()
{
}

void TklockInterface::connect()
{
    QDBusConnection::systemBus().connect(
      "",
      "/com/nokia/mce/signal",
      "com.nokia.mce.signal",
      "tklock_mode_ind",
      this,
      SLOT(setLockMode(QString)));
}
