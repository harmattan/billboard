
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

#include "organizer.h"

#include <QOrganizerItem>
#include <QOrganizerEventTime>

#define CACHE_TIME_MS (60*1000)
#define ORGANIZER_MAX_EVENTS_SHOWN (3)

QString
Organizer::getUpcomingEvents(int days) {
    int found = 0;
    QString result = "";

    if (QDateTime::currentMSecsSinceEpoch() - last_update < CACHE_TIME_MS && cached_result.contains(days)) {
        return cached_result[days];
    }

    QDateTime now = QDateTime::currentDateTime();
    QDateTime then = now.addDays(days);
    if (days == 0) {
        /* Today's events (including "past" events of today) */
        now = QDateTime(QDate::currentDate());
        then = QDateTime(QDate::currentDate(), QTime(23, 59));
    }
    QList<QOrganizerItem> items = manager.items(now, then);

    QOrganizerItem item;
    foreach (item, items) {
        if (item.type() == "Event") {
            QOrganizerEventTime eventTime = item.detail<QOrganizerEventTime>();
            QDateTime start = eventTime.startDateTime().toLocalTime();
            QDateTime end = eventTime.endDateTime().toLocalTime();

            if (start.date() != now.date()) {
                result += "+" + QString::number(
                        now.date().daysTo(start.date()));
                //result += start.toString("d. MMM");
                result += " ";
            }

            if (start.time() != QTime(0, 0)) {
                result += start.toString("HH:mm");
                result += ' ';
            }

            result += item.displayLabel();
            result += '\n';
            found++;
        }

        if (found >= ORGANIZER_MAX_EVENTS_SHOWN && days != 0) {
            /**
             * Limit maximum events if we show upcoming
             * (don't limit if days == 0, i.e. "Today's events")
             **/
            break;
        }
    }

    last_update = QDateTime::currentMSecsSinceEpoch();

    cached_result[days] = result.trimmed();

    return cached_result[days];
}

