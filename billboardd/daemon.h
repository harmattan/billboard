
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

#ifndef BILLBOARDD_DAEMON_H
#define BILLBOARDD_DAEMON_H

#include <QtCore>
#include <QtGui>
#include <GConfItem>
#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "../include/defaults.h"
#include "context.h"
#include "gconfmon.h"
#include "organizer.h"
#include "network.h"
#include "volumeinterface.h"
#include "batteryicon.h"

#include "renderer.h"
#include "formatter.h"

#define LOGO_KEY "/desktop/meego/screen_lock/low_power_mode/operator_logo"
#define CONFIG_KEY "/apps/billboard"

#define EVERY_HALF_HOUR (60*30)
#define EVERY_HOUR (60*60)

#define BATTERY_WARNING_FRAC (.3)
#define BATTERY_CRITICAL_FRAC (.15)

class Daemon : public QObject {
    Q_OBJECT

    public:
        Daemon(QString path, QObject *parent=NULL)
            : QObject(parent),
              file_path(path),
              filename_a(file_path.filePath("frame_a.png")),
              filename_b(file_path.filePath("frame_b.png")),
              quitting(false),
              operator_logo(NULL),
              config_other_logo(NULL),
              config_other_enabled(NULL),
              config_text(NULL),
              config_color(NULL),
              config_alignment(NULL),
              config_enabled(NULL),
              config_size(NULL),
              config_font(NULL),
              enabled(false),
              context(NULL),
              gconfmon(NULL),
              organizer(),
              volume(),
              updateTimer(),
              sysAlignedTimer(),
              lastRender(""),
              templateText("")
        {
            operator_logo = new GConfItem(LOGO_KEY, this);
            config_other_logo = new GConfItem(CONFIG_KEY "/other_logo", this);
            config_other_enabled = new GConfItem(CONFIG_KEY "/other_enabled", this);
            config_text = new GConfItem(CONFIG_KEY "/text", this);
            config_color = new GConfItem(CONFIG_KEY "/color", this);
            config_alignment = new GConfItem(CONFIG_KEY "/alignment", this);
            config_enabled = new GConfItem(CONFIG_KEY "/enabled", this);
            config_size = new GConfItem(CONFIG_KEY "/size", this);
            config_font = new GConfItem(CONFIG_KEY "/font", this);

            QObject::connect(operator_logo, SIGNAL(valueChanged()),
                    this, SLOT(operatorLogoChanged()));
            QObject::connect(config_other_logo, SIGNAL(valueChanged()),
                    this, SLOT(contextChanged()));
            QObject::connect(config_other_enabled, SIGNAL(valueChanged()),
                    this, SLOT(contextChanged()));
            QObject::connect(config_text, SIGNAL(valueChanged()),
                    this, SLOT(textChanged()));
            QObject::connect(config_color, SIGNAL(valueChanged()),
                    this, SLOT(contextChanged()));
            QObject::connect(config_alignment, SIGNAL(valueChanged()),
                    this, SLOT(contextChanged()));
            QObject::connect(config_enabled, SIGNAL(valueChanged()),
                    this, SLOT(enabledChanged()));
            QObject::connect(config_size, SIGNAL(valueChanged()),
                    this, SLOT(contextChanged()));
            QObject::connect(config_font, SIGNAL(valueChanged()),
                    this, SLOT(contextChanged()));

            context = new Context(this);
            QObject::connect(context, SIGNAL(changed()),
                    this, SLOT(contextChanged()));

            gconfmon = new GConfMonitor(this);
            QObject::connect(gconfmon, SIGNAL(changed()),
                    this, SLOT(contextChanged()));

            updateTimer.setInterval(500);
            updateTimer.setSingleShot(true);

            QObject::connect(&updateTimer, SIGNAL(timeout()),
                    this, SLOT(render()));

            sysAlignedTimer.setMinimumInterval(EVERY_HALF_HOUR);
            sysAlignedTimer.setMaximumInterval(EVERY_HOUR);
            sysAlignedTimer.setSingleShot(false);
            sysAlignedTimer.start();

            QObject::connect(&sysAlignedTimer, SIGNAL(timeout()),
                    this, SLOT(sysAlignedUpdate()));

            enabledChanged();
            textChanged();
            render(true);
        }

        ~Daemon() {
            quitting = true;
            resetOperatorLogo();
        }

    private:
        QDir file_path;
        QString filename_a;
        QString filename_b;
        bool quitting;
        GConfItem *operator_logo;
        GConfItem *config_other_logo;
        GConfItem *config_other_enabled;
        GConfItem *config_text;
        GConfItem *config_color;
        GConfItem *config_alignment;
        GConfItem *config_enabled;
        GConfItem *config_size;
        GConfItem *config_font;
        bool enabled;

        Context *context;
        GConfMonitor *gconfmon;
        Organizer organizer;
        VolumeInterface volume;

        QTimer updateTimer;
        QSystemAlignedTimer sysAlignedTimer;
        QString lastRender;
        QString templateText;

        QList<QString> getInterestingProperties() {
            QSet<QString> props;

            Formatter::format(templateText, templatemap(), &props);

            if (props.contains("ip")) {
                props << "network-name";
            }

            if (props.contains("song-volume-bar")) {
                props << "volume";
                props << "song";
            }
            if (props.contains("volume-bar")) {
                props << "volume";
            }

            if (props.contains("battery-bar")) {
                props << "battery";
            }
            if (props.contains("color-battery-bar")) {
                props << "battery";
            }
            if (props.contains("battery-icon")) {
                props << "battery";
            }

            return props.toList();
        }

        QMap<QString,QString> templatemap() {
            QDate now = QDate::currentDate();

            QString shortDate = now.toString(Qt::DefaultLocaleShortDate);
            QString longDate = now.toString(Qt::DefaultLocaleLongDate);

            QMap<QString,QString> tmpl;
            tmpl["date"] = shortDate;
            tmpl["full-date"] = longDate;

            tmpl["week"] = QString::number(now.weekNumber());
            tmpl["weekday"] = now.toString("dddd");
            tmpl["day"] = now.toString("d");
            tmpl["month"] = now.toString("MMMM");
            tmpl["short-month"] = now.toString("MMM");
            tmpl["month-number"] = now.toString("M");
            tmpl["year"] = now.toString("yyyy");
            tmpl["ip"] = get_ip_addresses();

            tmpl["events"] = organizer.getUpcomingEvents();
            tmpl["events-today"] = organizer.getUpcomingEvents(0);

            tmpl.unite(context->properties());
            tmpl.unite(gconfmon->properties());

            double battery = 0.01 * tmpl["battery"].toDouble();
            tmpl["battery-bar"] = "{{=" + QString::number(battery) + "}}";
            tmpl["battery-icon"] = "<<" + BatteryIcon::fromPercentage(battery) + ">>";

            // Colored battery bar - yellow and red
            if (battery < BATTERY_CRITICAL_FRAC) {
                tmpl["color-battery-bar"] = "{{red}}" + tmpl["battery-bar"] + "{{}}";
            } else if (battery < BATTERY_WARNING_FRAC) {
                tmpl["color-battery-bar"] = "{{yellow}}" + tmpl["battery-bar"] + "{{}}";
            } else {
                tmpl["color-battery-bar"] = tmpl["battery-bar"];
            }

            tmpl["battery-bar"] = "{{=" + QString::number((double)(tmpl["battery"].toDouble()*.01)) + "}}";

            tmpl["volume"] = QString::number((int)(volume.volume()*100.));
            tmpl["volume-bar"] = "{{=" + QString::number(volume.volume()) + "}}";

            // Only show volume bar when something is being played back
            if (tmpl["song"] != "") {
                tmpl["song-volume-bar"] = tmpl["volume-bar"];
            } else {
                tmpl["song-volume-bar"] = "";
            }

            return tmpl;
        }

        void resetOperatorLogo() {
            operator_logo->set(config_other_logo->value(""));
        }

    public slots:
        void enabledChanged() {
            bool newEnabled = config_enabled->value(BILLBOARD_CONFIG_ENABLED).toBool();

            if (enabled != newEnabled) {
                enabled = newEnabled;

                if (enabled) {
                    /* Re-subscribe to contextkit subscriptions */
                    textChanged();

                    /* Render twice, just to be sure it picks the right file? */
                    render(true);
                    render(true);
                } else {
                    /* Unsubscribe from all contextkit subscriptions */
                    context->subscribeTo(QList<QString>());

                    /* Unsubscribe from all gconf monitor subscriptions */
                    gconfmon->subscribeTo(QList<QString>());

                    /* Unsubscribe from volume notifications */
                    volume.disconnect();

                    /* Reset operator logo to "other" logo (if any) */
                    resetOperatorLogo();
                }
            }
        }

        void contextChanged() {
            updateTimer.start();
        }

        void textChanged() {
            if (!enabled) {
                return;
            }

            templateText = config_text->value(BILLBOARD_CONFIG_TEXT).toString();
            QList<QString> interestingProperties = getInterestingProperties();
            context->subscribeTo(interestingProperties);
            gconfmon->subscribeTo(interestingProperties);

            if (interestingProperties.contains("volume")) {
                QObject::connect(&volume, SIGNAL(changed()),
                        this, SLOT(contextChanged()));
            } else {
                volume.disconnect();
            }

            updateTimer.start();
        }

        void operatorLogoChanged() {
            if (quitting) {
                /* Ignore operator logo changes if we are quitting */
                return;
            }

            QString filename = operator_logo->value("").toString();
            if (filename != filename_a && filename != filename_b) {
                config_other_logo->set(filename);

                /* Render twice to make sure the logo is updated and seen */
                render(true);
                render(true);
            }
        }

        void sysAlignedUpdate()
        {
            render();
        }

        void render(bool force=false)
        {
            sysAlignedTimer.wokeUp();

            if (!enabled) {
                /* Do not render anything, even when forced */
                return;
            }

            updateTimer.stop();

            QString text = Formatter::format(templateText, templatemap(), NULL);

            if (!force && lastRender == text) {
                /* Don't update when not necessary */
                return;
            }

            int flags = 0;

            QString alignment = config_alignment->value(BILLBOARD_CONFIG_ALIGNMENT).toString();
            if (alignment == "left") {
                flags |= Qt::AlignLeft;
            } else if (alignment == "center") {
                flags |= Qt::AlignHCenter;
            } else {
                flags |= Qt::AlignRight;
            }

            QImage other;
            if (config_other_enabled->value(BILLBOARD_CONFIG_OTHER_ENABLED).toBool()) {
                other = QImage(config_other_logo->value("").toString());
            }

            QColor color = QColor(config_color->value(BILLBOARD_CONFIG_COLOR).toString());
            QFont font = QFont(config_font->value(BILLBOARD_CONFIG_FONT).toString(),
                    config_size->value(BILLBOARD_CONFIG_SIZE).toInt());

            QImage rendered = Renderer::render(text, flags, other, color, font);

            QString filename = ((operator_logo->value("") == filename_a)?
                    filename_b:filename_a);

            rendered.save(filename);

            operator_logo->set(filename);
        }
};

#endif
