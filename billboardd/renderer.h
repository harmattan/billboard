
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

#ifndef BILLBOARDD_RENDERER_H
#define BILLBOARDD_RENDERER_H

#include <QtCore>
#include <QtGui>

#include "../include/defaults.h"
#include "dithering.h"

#define CLOCK_SPACING_RIGHT_PX 5
#define OTHER_LOGO_SPACING_PX 10

#define MAXIMUM_WIDTH_PX 420
#define MAXIMUM_HEIGHT_PX 200

#define OTHER_LOGO_MAX_WIDTH_PX 200
#define OTHER_LOGO_MAX_HEIGHT_PX 120

#define USERTEXT_MAX_LINES 20

#define LINE_SPACING_PX 1

#define BAR_WIDTH_PX 180
#define BAR_HEIGHT_PX 10
#define BAR_SPACING_PX 5


class RenderedItem {
    public:
        RenderedItem(QString text, QColor color, QPainter *painter, int remainingWidth)
            : m_text(text),
              m_color(color),
              m_painter(painter),
              m_remainingWidth(remainingWidth),
              m_width(0),
              m_height(0),
              m_is_bar(false),
              m_bar_value(0.),
              m_is_image(false),
              m_dither_image(false),
              m_image()
        {
            parseBar();
            parseImage();
            elideText();
        }

        void
        elideText()
        {
            if (!m_is_bar && !m_is_image) {
                QString ellipsis = QString::fromUtf8("…");

                QRect lineRect = m_painter->boundingRect(QRect(), 0, m_text);
                if (lineRect.width() > m_remainingWidth) {
                    do {
                        m_text.chop(1);
                        lineRect = m_painter->boundingRect(QRect(), 0, m_text + ellipsis);
                    } while (lineRect.width() > m_remainingWidth && m_text.length() > 0);

                    m_text += ellipsis;
                }

                m_width = lineRect.width();
                m_height = lineRect.height();
            }
        }

        void
        parseBar() {
            QRegExp re("[{]{2}=(.*)[}]{2}");
            if (re.indexIn(m_text) != -1) {
                bool ok = false;
                double result = re.cap(1).toDouble(&ok);
                if (ok) {
                    m_is_bar = true;
                    m_bar_value = qMax(0., qMin(result, 1.));

                    m_width = qMin(m_remainingWidth, BAR_WIDTH_PX);
                    m_height = BAR_HEIGHT_PX + 2*BAR_SPACING_PX;
                }
            }
        }

        void
        parseImage() {
            QRegExp re("[<]{2}(.*)[>]{2}");
            if (re.indexIn(m_text) != -1) {
                m_text = re.cap(1);
                m_is_image = true;
                if (m_text.endsWith("#")) {
                    m_text = m_text.left(m_text.size() - 1);
                    m_dither_image = true;
                } else {
                    m_dither_image = false;
                }
                m_image.load(m_text);
                //m_image = m_image.scaledToHeight(m_image.height()*1.5);
                if (m_dither_image &&
                       ((m_image.width() > OTHER_LOGO_MAX_WIDTH_PX) ||
                        (m_image.height() > OTHER_LOGO_MAX_HEIGHT_PX))) {
                    int w = qMin(m_image.width(), OTHER_LOGO_MAX_WIDTH_PX);
                    int h = qMin(m_image.height(), OTHER_LOGO_MAX_HEIGHT_PX);
                    m_image = m_image.scaled(w, h, Qt::KeepAspectRatio);
                }
                m_width = m_image.width();
                m_height = m_image.height();
            }
        }

        QString m_text;
        QColor m_color;
        QPainter *m_painter;
        int m_remainingWidth;

        int m_width;
        int m_height;

        bool m_is_bar;
        qreal m_bar_value;

        bool m_is_image;
        bool m_dither_image;
        QImage m_image;
};


class RenderedLine {
    public:
        RenderedLine(QString line, QColor color, QPainter *painter)
            : m_items(),
              m_color(color),
              m_painter(painter),
              m_width(0),
              m_height(0)
        {
            int remainingWidth = MAXIMUM_WIDTH_PX;

            QList< QPair<QString,QColor> > split = parseColors(line);

            for (int i=0; i<split.size(); i++) {
                color = split.at(i).second;

                QString text = split.at(i).first;
                QList<QString> splitString = splitImages(text);
                for (int j=0; j<splitString.size(); j++) {
                    RenderedItem *item = new RenderedItem(splitString.at(j),
                            color, painter, remainingWidth);
                    remainingWidth -= item->m_width;
                    m_items.append(item);

                    if (remainingWidth < 0) {
                        break;
                    }
                }

                if (remainingWidth < 0) {
                    break;
                }
            }

            for (int i=0; i<m_items.size(); i++) {
                RenderedItem *item = m_items.at(i);
                m_width += item->m_width;
                if (m_height < item->m_height) {
                    m_height = item->m_height;
                }
            }
        }

        QList<QString>
        splitImages(QString line)
        {
            QList<QString> result;

            QRegExp re("[<]{2}([^>]*)[>]{2}");

            int lastPos = 0;
            int pos = 0;
            while ((pos = re.indexIn(line, pos)) != -1) {
                QString content = re.cap(0);

                if (lastPos != pos) {
                    result << line.mid(lastPos, pos-lastPos);
                }

                result << content;

                pos += content.length();
                lastPos = pos;
            }

            result << line.mid(lastPos);

            return result;
        }

        QList< QPair<QString,QColor> >
        parseColors(QString line)
        {
            QList< QPair<QString,QColor> > result;
            QColor currentColor = m_color;

            QMap<QString,QColor> colorDefs;
            colorDefs.insert("white", Qt::white);
            colorDefs.insert("red", Qt::red);
            colorDefs.insert("green", Qt::green);
            colorDefs.insert("blue", Qt::blue);
            colorDefs.insert("cyan", Qt::cyan);
            colorDefs.insert("magenta", Qt::magenta);
            colorDefs.insert("yellow", Qt::yellow);


            QRegExp re("[{]{2}([^}]*)[}]{2}");

            int lastPos = 0;
            int pos = 0;
            while ((pos = re.indexIn(line, pos)) != -1) {
                QString content = re.cap(1);

                if (lastPos != pos) {
                    result << QPair<QString,QColor>(line.mid(lastPos, pos-lastPos), currentColor);
                }

                if (content == "") {
                    currentColor = m_color;
                } else if (colorDefs.contains(content)) {
                    currentColor = colorDefs[content];
                } else {
                    result << QPair<QString,QColor>(re.cap(0), currentColor);
                }

                /* length of "{{" + content + "}}" */
                pos += 2 + content.length() + 2;
                lastPos = pos;
            }

            result << QPair<QString,QColor>(line.mid(lastPos), currentColor);

            return result;
        }


        ~RenderedLine()
        {
            for (int i=0; i<m_items.size(); i++) {
                delete m_items.at(i);
            }
        }

        int
        renderTo(QPainter *painter, int x, int y)
        {
            for (int i=0; i<m_items.size(); i++) {
                RenderedItem *item = m_items.at(i);
                painter->setPen(QPen(item->m_color));

                int ydiff = (m_height - item->m_height) / 2;
                if (ydiff) {
                    painter->save();
                    painter->translate(0, ydiff);
                }

                if (item->m_is_image) {
                    if (item->m_dither_image) {
                        painter->drawImage(x, y, dither_image(item->m_image));
                    } else {
                        painter->drawImage(x, y, item->m_image);
                    }
                    x += item->m_width;
                } else if (item->m_is_bar) {
                    painter->setBrush(Qt::transparent);
                    painter->drawRect(x, y + BAR_SPACING_PX, item->m_width - 3, BAR_HEIGHT_PX);
                    painter->setBrush(painter->pen().color());
                    painter->drawRect(x, y + BAR_SPACING_PX, (item->m_width - 3)*item->m_bar_value, BAR_HEIGHT_PX);
                    x += item->m_width;
                } else {
                    QRect textSize;
                    painter->drawText(x, y, m_width, m_height, 0, item->m_text, &textSize);
                    x += textSize.width();
                }

                if (ydiff) {
                    painter->restore();
                }
            }

            return y + m_height + LINE_SPACING_PX;
        }

        QList<RenderedItem*> m_items;
        QColor m_color;
        QPainter *m_painter;

        int m_width;
        int m_height;
};

class RenderedDisplay : public QList<RenderedLine*> {
    public:
        RenderedDisplay(QFont font, QColor color, int flags)
            : QList<RenderedLine*>(),
              m_image(1, 1, QImage::Format_RGB32),
              m_painter(),
              m_font(font),
              m_color(color),
              m_flags(flags)
        {
            m_painter.begin(&m_image);
            m_painter.setFont(m_font);
        }

        ~RenderedDisplay()
        {
            for (int i=0; i<size(); i++) {
                delete at(i);
            }
        }

        void
        format(QString text)
        {
            append(new RenderedLine(text, m_color, &m_painter));
        }

        QSize
        requiredSize()
        {
            int width = 0;
            int height = 0;

            for (int i=0; i<size(); i++) {
                RenderedLine *line = at(i);
                if (line->m_width > width) {
                    width = line->m_width;
                }
                if (height) {
                    height += LINE_SPACING_PX;
                }
                height += line->m_height;
            }

            return QSize(width, height);
        }

        void
        renderTo(QPainter *painter)
        {
            QSize imageSize = requiredSize();

            int y = 0;
            for (int i=0; i<size(); i++) {
                RenderedLine *line = at(i);

                int x = 0;
                if (m_flags & Qt::AlignRight) {
                    x = imageSize.width() - line->m_width;
                } else if (m_flags & Qt::AlignCenter) {
                    x = (imageSize.width() - line->m_width) / 2;
                }

                y = line->renderTo(painter, x, y);
            }
        }

    private:
        QImage m_image;
        QPainter m_painter;

        QFont m_font;
        QColor m_color;
        int m_flags;
};

class Renderer {
    public:
        static QImage
        render(QString text, int flags, QImage other, QColor color, QFont font)
        {
            RenderedDisplay display(font, color, flags);
            QStringList lines = text.split("\n").mid(0, USERTEXT_MAX_LINES);
            for (int i=0; i<lines.size(); i++) {
                display.format(lines[i]);
            }

            QSize imageSize = display.requiredSize();

            if (!other.isNull()) {
                if ((other.width() > OTHER_LOGO_MAX_WIDTH_PX) ||
                        (other.height() > OTHER_LOGO_MAX_HEIGHT_PX)) {
                    int w = qMin(other.width(), OTHER_LOGO_MAX_WIDTH_PX);
                    int h = qMin(other.height(), OTHER_LOGO_MAX_HEIGHT_PX);
                    other = other.scaled(w, h, Qt::KeepAspectRatio);
                }

                int height = (imageSize.height() +
                        OTHER_LOGO_SPACING_PX +
                        other.height());
                int width = qMax(imageSize.width(), other.width());
                imageSize = QSize(width, height);
            }
            if (imageSize.width() > MAXIMUM_WIDTH_PX) {
                imageSize.setWidth(MAXIMUM_WIDTH_PX);
            }

            imageSize.setWidth(imageSize.width() + CLOCK_SPACING_RIGHT_PX);

            QImage image(imageSize, QImage::Format_RGB32);
            image.fill(Qt::black);
            QPainter painter;
            painter.begin(&image);
            painter.setFont(font);

            display.renderTo(&painter);

            if (!other.isNull()) {
                /* Draw other logo in the lower right corner */
                int x = (image.width() -
                        other.width() -
                        CLOCK_SPACING_RIGHT_PX);
                int y = image.height() - other.height();
                painter.drawImage(x, y, other);
            }

            painter.end();
            return image;
        }
};

#endif
