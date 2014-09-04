
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

#ifndef BILLBOARDD_BATTERYICON_H
#define BILLBOARDD_BATTERYICON_H

#include <QtCore>

#define BASE "/usr/share/themes/blanco/meegotouch/icons/icon-s-status-battery"

class BatteryIcon {
    public:
        static QString fromPercentage(double percentage) {
            QString base(BASE);
            if (percentage > .9) {
                return base + "8.png";
            } else if (percentage > .8) {
                return base + "7.png";
            } else if (percentage > .7) {
                return base + "6.png";
            } else if (percentage > .6) {
                return base + "5.png";
            } else if (percentage > .5) {
                return base + "4.png";
            } else if (percentage > .4) {
                return base + "3.png";
            } else if (percentage > .3) {
                return base + "2.png";
            } else if (percentage > .2) {
                return base + "1.png";
            } else if (percentage > .1) {
                return base + "-low.png";
            } else {
                return base + "-verylow.png";
            }
        }
};

#endif
