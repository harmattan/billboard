
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

#ifndef BILLBOARDD_GCONF_H
#define BILLBOARDD_GCONF_H

#include <QtCore>

#include <GConfItem>

class GConfMonitor : public QObject {
    Q_OBJECT

    public:
        GConfMonitor(QObject *parent=NULL)
            : QObject(parent),
              gconfItems()
        {
            /* USSD Balance - http://talk.maemo.org/showthread.php?t=86563 */
            gconfItems["ussd-balance"] = new GConfItem("/apps/UssdBalance/currentStrBalance", this);
            gconfItems["ussd-balance-2"] = new GConfItem("/apps/UssdBalance/currentStrBalance2", this);
            gconfItems["ussd-balance-3"] = new GConfItem("/apps/UssdBalance/currentStrBalance3", this);
        }

        QMap<QString,QString> properties() {
            QMap<QString,QString> tmpl;

            QMap<QString,GConfItem*>::const_iterator it;
            for (it=gconfItems.constBegin(); it != gconfItems.constEnd(); ++it) {
                tmpl[it.key()] = it.value()->value().toString();
            }
            return tmpl;
        }

        void subscribeTo(QList<QString> properties) {
            /* Subscribe to all interesting keys */
            QMap<QString,GConfItem*>::const_iterator it;
            for (it=gconfItems.constBegin(); it != gconfItems.constEnd(); ++it) {
                if (properties.contains(it.key())) {
                    QObject::connect(it.value(), SIGNAL(valueChanged()),
                            this, SIGNAL(changed()));
                    //qDebug() << "subscribing to" << it.key();
                } else {
                    QObject::disconnect(it.value(), SIGNAL(valueChanged()),
                            this, SIGNAL(changed()));
                    //qDebug() << "unsubscribing from" << it.key();
                }
            }
        }

    private:
        QMap<QString,GConfItem*> gconfItems;

    signals:
        void changed();
};

#endif
