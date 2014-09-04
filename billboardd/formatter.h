
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

#ifndef BILLBOARDD_FORMATTER_H
#define BILLBOARDD_FORMATTER_H

#include <QtCore>

/* ms after which we forcibly kill the formatter script to not cause hangs */
#define FORMATTER_SCRIPT_TIMEOUT_MS 1000

class Formatter {
    public:
        static QString
        format(QString text, const QMap<QString,QString> &tmpl,
                QSet<QString> *props)
        {
            QStringList result;

            int pos = 0;
            int length = text.length();
            int depth = 0;
            int last = 0;

            while (pos < length) {
                switch (text.at(pos).unicode()) {
                    case '{':
                        if (depth == 0) {
                            if (pos - last != 0) {
                                // Append static text outside of {}
                                QString add = text.mid(last, pos - last);
                                result.append(add);
                            }
                            last = pos;
                        }
                        depth++;
                        break;
                    case '}':
                        depth--;
                        if (depth == 0) {
                            // Look up a text in {} (without the {})
                            QString add = text.mid(last+1, pos - last - 1);
                            result.append(lookup(add, tmpl, props));
                            last = pos+1;
                        }
                        break;
                    default:
                        break;
                }

                pos++;
            }

            if (pos - last > 0) {
                // Non-empty "rest" - append normally
                QString add = text.mid(last, pos-last);
                result.append(add);
            }

            return result.join("");
        }

    private:
        static QString
        lookup(QString key, const QMap<QString,QString> &tmpl,
                QSet<QString> *props)
        {
            if (key.indexOf("{") != -1) {
                key = format(key, tmpl, props);
            }

            int pos;

            // {query?value-if-nonempty}
            if ((pos = key.indexOf("?")) != -1) {
                QString query = key.mid(0, pos);
                if (lookup(query, tmpl, props) != "") {
                    QString placeholder = key.mid(pos+1);
                    return format(placeholder, tmpl, props);
                }
                return "";
            }

            // {query!value-if-empty}
            if ((pos = key.indexOf("!")) != -1) {
                QString query = key.mid(0, pos);
                if (lookup(query, tmpl, props) == "") {
                    QString placeholder = key.mid(pos+1);
                    return format(placeholder, tmpl, props);
                }
                return "";
            }

            // Run script on the command line (MUST NOT CONTAIN { or })
            if (key.startsWith("script:")) {
                QString command = key.mid(7);
                QProcess process;
                process.start(command);
                process.waitForFinished(FORMATTER_SCRIPT_TIMEOUT_MS);
                return QString::fromUtf8(process.readAllStandardOutput().data());
            }

            if (tmpl.contains(key)) {
                if (props != NULL) {
                    props->insert(key);
                }
                return tmpl[key];
            }

            return QString("{") + key + QString("}");
        }
};

#endif /* BILLBOARDD_FORMATTER_H */
