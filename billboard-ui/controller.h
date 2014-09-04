
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

#ifndef BILLBOARD_UI_CONTROLLER_H
#define BILLBOARD_UI_CONTROLLER_H

#include <QtCore>
#include <GConfItem>

#include "../include/defaults.h"

#define LOGO_KEY "/desktop/meego/screen_lock/low_power_mode/operator_logo"
#define CONFIG_KEY "/apps/billboard"

class Controller : public QObject {
    Q_OBJECT

    public:
        Controller(QObject *parent=NULL)
            : QObject(parent),
              config_text(NULL),
              config_color(NULL),
              config_alignment(NULL),
              config_enabled(NULL),
              config_other_enabled(NULL),
              config_size(NULL),
              config_font(NULL),
              operator_logo(NULL)
        {
            config_text = new GConfItem(CONFIG_KEY "/text", this);
            config_color = new GConfItem(CONFIG_KEY "/color", this);
            config_alignment = new GConfItem(CONFIG_KEY "/alignment", this);
            config_enabled = new GConfItem(CONFIG_KEY "/enabled", this);
            config_other_enabled = new GConfItem(CONFIG_KEY "/other_enabled", this);
            config_size = new GConfItem(CONFIG_KEY "/size", this);
            config_font = new GConfItem(CONFIG_KEY "/font", this);
            operator_logo = new GConfItem(LOGO_KEY, this);

            QObject::connect(config_text, SIGNAL(valueChanged()),
                    this, SIGNAL(textChanged()));

            QObject::connect(config_color, SIGNAL(valueChanged()),
                    this, SIGNAL(colorChanged()));

            QObject::connect(config_alignment, SIGNAL(valueChanged()),
                    this, SIGNAL(alignmentChanged()));

            QObject::connect(config_enabled, SIGNAL(valueChanged()),
                    this, SIGNAL(enabledChanged()));

            QObject::connect(config_other_enabled, SIGNAL(valueChanged()),
                    this, SIGNAL(otherEnabledChanged()));

            QObject::connect(operator_logo, SIGNAL(valueChanged()),
                    this, SIGNAL(logoChanged()));

            QObject::connect(config_size, SIGNAL(valueChanged()),
                    this, SIGNAL(sizeChanged()));

            QObject::connect(config_font, SIGNAL(valueChanged()),
                    this, SIGNAL(fontChanged()));
        }

        void setText(QString text) {
            config_text->set(text);
        }

        void setColor(QString color) {
            config_color->set(color);
        }

        void setAlignment(QString alignment) {
            config_alignment->set(alignment);
        }

        void setOtherEnabled(bool otherEnabled) {
            if (otherEnabled != this->otherEnabled()) {
                config_other_enabled->set(otherEnabled);
            }
        }

        void setEnabled(bool enabled) {
            if (enabled != this->enabled()) {
                config_enabled->set(enabled);
            }
        }

        void setSize(int size) {
            if (size != this->size()) {
                config_size->set(size);
            }
        }

        void setFont(QString font) {
            if (font != this->font()) {
                config_font->set(font);
            }
        }

        QString text() {
            return config_text->value(BILLBOARD_CONFIG_TEXT).toString();
        }

        QString color() {
            return config_color->value(BILLBOARD_CONFIG_COLOR).toString();
        }

        QString alignment() {
            return config_alignment->value(BILLBOARD_CONFIG_ALIGNMENT).toString();
        }

        bool enabled() {
            return config_enabled->value(BILLBOARD_CONFIG_ENABLED).toBool();
        }

        bool otherEnabled() {
            return config_other_enabled->value(BILLBOARD_CONFIG_OTHER_ENABLED).toBool();
        }

        QString logo() {
            return operator_logo->value("").toString();
        }

        int size() {
            return config_size->value(BILLBOARD_CONFIG_SIZE).toInt();
        }

        QString font() {
            return config_font->value(BILLBOARD_CONFIG_FONT).toString();
        }

        Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
        Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(QString alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)
        Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
        Q_PROPERTY(bool otherEnabled READ otherEnabled WRITE setOtherEnabled NOTIFY otherEnabledChanged)
        Q_PROPERTY(QString logo READ logo NOTIFY logoChanged)
        Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
        Q_PROPERTY(QString font READ font WRITE setFont NOTIFY fontChanged)

    signals:
        void textChanged();
        void colorChanged();
        void alignmentChanged();
        void enabledChanged();
        void otherEnabledChanged();
        void logoChanged();
        void sizeChanged();
        void fontChanged();

    private:
        GConfItem *config_text;
        GConfItem *config_color;
        GConfItem *config_alignment;
        GConfItem *config_enabled;
        GConfItem *config_other_enabled;
        GConfItem *config_size;
        GConfItem *config_font;
        GConfItem *operator_logo;

};

#endif
