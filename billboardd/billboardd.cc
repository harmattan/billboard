
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

#include <QtGui>
#include <signal.h>

#include "daemon.h"

#define DATA_DIR "io.thp.billboardd-cache/"

static QApplication *app = NULL;
static Daemon *billboardd = NULL;

void terminate_gracefully(int) {
    delete billboardd;
    app->quit();
}

int main(int argc, char *argv[])
{
    setenv("DISPLAY", ":0", 1);
    app = new QApplication(argc, argv);
    QDir temp("/tmp");

    if (!temp.exists(DATA_DIR)) {
        if (!temp.mkpath(DATA_DIR)) {
            return EXIT_FAILURE;
        }
    }

    billboardd = new Daemon(temp.filePath(DATA_DIR));

    signal(SIGINT, terminate_gracefully);
    signal(SIGTERM, terminate_gracefully);

    return app->exec();
}

