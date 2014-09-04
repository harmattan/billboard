
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

#include "volumeinterface.h"

#include <QDebug>

#include <cstdlib>
#include <cstring>
#include <dbus/dbus-glib-lowlevel.h>

#define DEFAULT_ADDRESS "unix:path=/var/run/pulse/dbus-socket"

#define VOLUME_SV    "com.Nokia.MainVolume1"
#define VOLUME_PATH  "/com/meego/mainvolume1"
#define VOLUME_IF    "com.Nokia.MainVolume1"

void on_steps_updated_proc(DBusConnection *conn, DBusMessage *message, VolumeInterface *logic);

VolumeInterface::VolumeInterface() :
    QObject(NULL),
    m_dbus_conn(NULL),
    m_volume(0.)
{
    connect();
}

VolumeInterface::~VolumeInterface ()
{
    if (m_dbus_conn) {
        dbus_connection_unref(m_dbus_conn);
    }
}

void VolumeInterface::connect()
{
    if ((m_dbus_conn != NULL) && (dbus_connection_get_is_connected(m_dbus_conn))) {
        return;
    }

    char *pa_bus_address = getenv ("PULSE_DBUS_SERVER");
    if (pa_bus_address == NULL) {
        pa_bus_address = (char *)DEFAULT_ADDRESS;
    }

    m_dbus_conn = dbus_connection_open(pa_bus_address, NULL);

    if (m_dbus_conn != NULL) {
        dbus_connection_setup_with_g_main(m_dbus_conn, NULL);

        dbus_connection_add_filter(m_dbus_conn,
                (DBusHandleMessageFunction) on_steps_updated_proc,
                (void *) this, NULL);

        DBusMessage *msg;
        DBusMessage *reply;
        quint32 currentmax = 22, currentvolume = 0;

        msg = dbus_message_new_method_call (VOLUME_SV,
                VOLUME_PATH,
                "org.freedesktop.DBus.Properties",
                "GetAll");

        const char *volume_if = VOLUME_IF;
        dbus_message_append_args (msg,
                DBUS_TYPE_STRING, &volume_if,
                DBUS_TYPE_INVALID);

        reply = dbus_connection_send_with_reply_and_block(m_dbus_conn,
                msg, -1, NULL);

        dbus_message_unref (msg);

        if (reply && (dbus_message_get_type (reply) ==
                    DBUS_MESSAGE_TYPE_METHOD_RETURN)) {
            DBusMessageIter iter;
            dbus_message_iter_init (reply, &iter);
            // Recurse into the array [array of dicts]
            while (dbus_message_iter_get_arg_type (&iter) != DBUS_TYPE_INVALID) {
                DBusMessageIter dict_entry;
                dbus_message_iter_recurse (&iter, &dict_entry);

                // Recurse into the dict [ dict_entry (string, variant(int)) ]
                while (dbus_message_iter_get_arg_type (&dict_entry) != DBUS_TYPE_INVALID) {
                    DBusMessageIter in_dict;
                    // Recurse into the dict_entry [ string, variant(int) ]
                    dbus_message_iter_recurse (&dict_entry, &in_dict);
                    {
                        char *prop_name = NULL;
                        // Get the string value, "property name"
                        dbus_message_iter_get_basic (&in_dict, &prop_name);

                        dbus_message_iter_next (&in_dict);

                        DBusMessageIter variant;
                        // Recurese into the variant [ variant(int) ]
                        dbus_message_iter_recurse (&in_dict, &variant);

                        quint32 value;
                        // Get the variant value which is uint32
                        dbus_message_iter_get_basic (&variant, &value);

                        if (prop_name && strcmp (prop_name, "StepCount") == 0) {
                            currentmax = value;
                        } else if (prop_name && strcmp (prop_name, "CurrentStep") == 0) {
                            currentvolume = value;
                        }
                    }

                    dbus_message_iter_next (&dict_entry);
                }
                dbus_message_iter_next (&iter);
            }
        }

        if (reply) {
            dbus_message_unref (reply);
        }

        setVolume((qreal)currentvolume / (qreal)(currentmax - 1));

        DBusMessage     *message = NULL;
        char            *signal = (char *) "com.Nokia.MainVolume1.StepsUpdated";
        char           **emptyarray = { NULL };

        message = dbus_message_new_method_call (NULL,
                "/org/pulseaudio/core1",
                NULL,
                "ListenForSignal");

        if (message) {
            dbus_message_append_args (message,
                    DBUS_TYPE_STRING, &signal,
                    DBUS_TYPE_ARRAY, DBUS_TYPE_OBJECT_PATH, &emptyarray, 0,
                    DBUS_TYPE_INVALID);

            dbus_connection_send (m_dbus_conn, message, NULL);
            dbus_message_unref (message);
        }
    }
}

void on_steps_updated_proc(DBusConnection *conn, DBusMessage *message,
        VolumeInterface *logic)
{
    if (message && dbus_message_has_member (message, "StepsUpdated")) {
        quint32 value = 0;
        quint32 maxvalue = 0;

        if (dbus_message_get_args (message, NULL,
                                   DBUS_TYPE_UINT32, &maxvalue,
                                   DBUS_TYPE_UINT32, &value,
                                   DBUS_TYPE_INVALID)) {
            logic->setVolume((qreal)value / (qreal)(maxvalue - 1));
        }
    }
}
