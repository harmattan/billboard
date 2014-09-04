
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

#include "dithering.h"

#include <QtGui>
#include <stdint.h>
#include <assert.h>

#define DITHERING_GET_PIXEL(d, w, bpp, x, y) ((uint32_t*)((d) + (y)*(w) + (x)*(bpp)))

static uint32_t
find_nearest_color(uint32_t color)
{
    uchar r = ((color >> 24) & 0xFF);
    uchar g = ((color >> 16) & 0xFF);
    uchar b = ((color >> 8) & 0xFF);
    uchar a = (color & 0xFF);

    uchar rr = 0x00, gr = 0x00, br = 0x00, ar = 0x00;
    int distance = 255*255 + 255*255 + 255*255 + 255*255 + 1;

    for (int i=0; i<16; i++) {
        uchar rt = (i & 1) ? 0xFF : 0x00;
        uchar gt = (i & 2) ? 0xFF : 0x00;
        uchar bt = (i & 4) ? 0xFF : 0x00;
        uchar at = (i & 8) ? 0xFF : 0x00;

        int rd = rt-r;
        int gd = gt-g;
        int bd = bt-b;
        int ad = at-a;

        int distancet = rd*rd + gd*gd + bd*bd + ad*ad;
        if (distancet < distance) {
            rr = rt;
            gr = gt;
            br = bt;
            ar = at;
            distance = distancet;
        }
    }

    return (rr << 24) | (gr << 16) | (br << 8) | ar;
}

static void
dither_error(uchar *d, int w, int bpp, int x, int y, uint32_t old_pixel, uint32_t new_pixel)
{
    uint32_t *a[4] = {
        DITHERING_GET_PIXEL(d, w, bpp, x+1, y),
        DITHERING_GET_PIXEL(d, w, bpp, x-1, y+1),
        DITHERING_GET_PIXEL(d, w, bpp, x, y+1),
        DITHERING_GET_PIXEL(d, w, bpp, x+1, y+1),
    };
    int factors[4] = {
        7, 3, 5, 1
    };

    for (int i=0; i<4; i++) {
        uint32_t *pixel = a[i];
        for (int offset=0; offset<bpp; offset++) {
            int old_pixel_channel = (old_pixel >> (8*offset)) & 0xFF;
            int new_pixel_channel = (new_pixel >> (8*offset)) & 0xFF;
            int error = old_pixel_channel - new_pixel_channel;
            int target = ((*pixel >> (8*offset)) & 0xFF);
            int errord = (error*factors[offset]) >> 5;
            target += errord;
            if (target < 0) {
                target = 0;
            } else if (target > 0xFF) {
                target = 0xFF;
            }
            *pixel = (*pixel & ~(0xFF << (8*offset))) |
                     ((target & 0xFF) << (8*offset));
        }
    }
}

static void
dither_lpm(QImage &image)
{
    QImage original = image.copy();

    // http://en.literateprograms.org/Floyd-Steinberg_dithering_(C)
    uchar *d = image.bits();
    uint bytesPerPixel = 4;
    for (int y=0; y<image.height(); y++) {
        for (int x=0; x<image.width(); x++) {
            uint32_t *pixel = DITHERING_GET_PIXEL(d, image.bytesPerLine(), bytesPerPixel, x, y);
            uint32_t old_pixel = *pixel;
            *pixel = find_nearest_color(*pixel);
            if (x > 1 && x < image.width()-1 && y < image.height()-1) {
                dither_error(d, image.bytesPerLine(), bytesPerPixel,
                        x, y, old_pixel, *pixel);
            }
        }
    }
}

QImage
dither_image(QImage src)
{
    QImage image = src.copy();
    if (image.format() != QImage::Format_ARGB32) {
        image = image.convertToFormat(QImage::Format_ARGB32);
    }
    image.fill(Qt::black);
    QPainter p;
    p.begin(&image);
    p.drawImage(0, 0, src);
    p.end();

    dither_lpm(image);

    p.begin(&image);
    p.setPen(Qt::black);

    for (int x=0; x<image.width(); x++) {
        for (int y=0; y<image.height(); y++) {
            if ((x + y) % 3 == 0) {
                p.drawPoint(x, y);
            }
        }
    }

    p.end();

    return image;
}

