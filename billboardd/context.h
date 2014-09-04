
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

#ifndef BILLBOARDD_CONTEXT_H
#define BILLBOARDD_CONTEXT_H

#include <QtCore>

#include <contextproperty.h>

class Context : public QObject {
    Q_OBJECT

    public:
        Context(QObject *parent=NULL)
            : QObject(parent),
              nowPlaying(new ContextProperty("Media.NowPlaying")),
              ctxProps()
        {
            QObject::connect(nowPlaying, SIGNAL(valueChanged()),
                    this, SLOT(nowPlayingChanged()));

            ctxProps["cell-network"] = new ContextProperty("Cellular.NetworkName");
            ctxProps["cell-signal-bars"] = new ContextProperty("Cellular.SignalBars");
            ctxProps["cell-signal-strength"] = new ContextProperty("Cellular.SignalStrength");
            ctxProps["network-name"] = new ContextProperty("Internet.NetworkName");
            ctxProps["battery"] = new ContextProperty("Battery.ChargePercentage");
            ctxProps["profile"] = new ContextProperty("Profile.Name");
            ctxProps["presence"] = new ContextProperty("Presence.State");

            /* from MeeCast - meecast/meegotouchplugin/data/meecast.context */
            ctxProps["meecast-station"] = new ContextProperty("/Weather/Station");
            ctxProps["meecast-temperature"] = new ContextProperty("/Weather/Temperature");
            ctxProps["meecast-description"] = new ContextProperty("/Weather/Description");
            ctxProps["meecast-icon-src"] = new ContextProperty("/Weather/IconPath");

            /* Wazapp online status - string property (offline, connecting, online) */
            ctxProps["wazapp-status"] = new ContextProperty("Wazapp.Online");

            ctxProps["alarm-present"] = new ContextProperty("Alarm.Present");

            /* Connect to all property change signals */
            QMap<QString,ContextProperty*>::const_iterator it;
            for (it=ctxProps.constBegin(); it != ctxProps.constEnd(); ++it) {
                QObject::connect(it.value(), SIGNAL(valueChanged()),
                        this, SIGNAL(changed()));
            }
        }

        QMap<QString,QString> properties() {
            QMap<QString,QString> tmpl;
            tmpl["song-title"] = songTitle;
            tmpl["song-artist"] = songArtist;

            if (songTitle == "" && songArtist == "") {
                tmpl["song"] = "";
            } else {
                tmpl["song"] = songArtist + " - " + songTitle;
            }

            QMap<QString,ContextProperty*>::const_iterator it;
            for (it=ctxProps.constBegin(); it != ctxProps.constEnd(); ++it) {
                tmpl[it.key()] = it.value()->value().toString();
            }
            return tmpl;
        }

        void subscribeTo(QList<QString> properties) {
            /* Subscribe to all interesting properties */
            QMap<QString,ContextProperty*>::const_iterator it;
            for (it=ctxProps.constBegin(); it != ctxProps.constEnd(); ++it) {
                if (properties.contains(it.key())) {
                    it.value()->subscribe();
                } else {
                    it.value()->unsubscribe();
                }
            }

            if (properties.contains("song") ||
                    properties.contains("song-title") ||
                    properties.contains("song-artist")) {
                nowPlaying->subscribe();
            } else {
                nowPlaying->unsubscribe();
            }
        }

    private slots:
        void nowPlayingChanged() {
            QMap<QString, QVariant> metadata = nowPlaying->value().toMap();

            if (metadata.contains("artist") && metadata.contains("title")) {
                songTitle = metadata["title"].toString();
                songArtist = metadata["artist"].toString();
            } else if (metadata.contains("resource")) {
                QString title = QUrl::fromPercentEncoding(
                        metadata["resource"].toByteArray());

                // strip path
                title = QFileInfo(title).fileName();
                // strip extension
                title = title.mid(0, title.lastIndexOf('.'));

                if (title.contains(" - ")) {
                    QStringList items = title.split(" - ");
                    songArtist = items.takeFirst();
                    songTitle = items.join(" - ");
                } else {
                    songTitle = title;
                    songArtist = "";
                }
            } else {
                songTitle = "";
                songArtist = "";
            }

            emit changed();
        }

    private:
        ContextProperty *nowPlaying;
        QMap<QString,ContextProperty*> ctxProps;

        QString songTitle;
        QString songArtist;

    signals:
        void changed();
};

#endif
