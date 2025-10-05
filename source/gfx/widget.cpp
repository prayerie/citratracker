// adapted from widget.cpp in tobkit
// by Tobias Weyand, licensed under
// the Apache License version 2.0

#include "gfx/widget.h"
#include "gfx/colour.h"
#include "gfx/font/font_8x11.inc"



Widget::Widget(u8 _x, u8 _y, u8 _width, u8 _height, bool _visible, bool _occluded)
    : x(_x), y(_y), width(_width), height(_height), visible(_visible), occluded(_occluded), is_touched(false) {

}

void Widget::drawBox(u8* fb, u16 tx, u16 ty, u16 tw, u16 th, u32 colour) {
    u8 r = RED(colour);
    u8 g = GREEN(colour);
    u8 b = BLUE(colour);

    uint_fast8_t i, j;
    for (i = 0;i < tw;++i) {
        drawPixel(fb, i + tx, ty, r, g, b);
        drawPixel(fb, i + tx, ty + th - 1, r, g, b);
    }
    for (j = 1;j < th - 1;++j) {
        drawPixel(fb, tx, ty + j, r, g, b);
        drawPixel(fb, tx + tw - 1, ty + j, r, g, b);
    }
}

void Widget::drawHLine(u8* fb, u16 tx, u16 ty, u16 length, u32 col) {
    u8 r = RED(col); // don't convert these for every pixel drawn
    u8 g = GREEN(col);
    u8 b = BLUE(col);

    for (int i = 0;i < length;++i) {
        drawPixel(fb, i + tx, ty, r, g, b);
    }
}

void Widget::drawVLine(u8* fb, u16 tx, u16 ty, u16 length, u32 col) {
    u8 r = RED(col);
    u8 g = GREEN(col);
    u8 b = BLUE(col);

    for (int i = 0;i < length;++i) {
        drawPixel(fb, tx, i + ty, r, g, b);
    }
}

void Widget::drawFilledBox(u8* fb, u16 tx, u16 ty, u16 tw, u16 th, u32 colour) {
    if (tw == 0) return;

    // int bw = (int)tw * 2;
    for (int j = 0;j < th;++j) {
        drawHLine(fb, tx, ty + j, tw, colour); // todo use DMA
    }
}


void Widget::draw(u8* fb) {
    drawGradient(fb, 0x00ff0000, 0x00ff00ff, 0, 0, width, height);
    // drawFilledBox(fb, 0, 0, width, height, is_touched ? 0xff0000 : 0xff00ff);
}

void Widget::onVisibilityChanged(bool visible) {

}

void Widget::touched(void) {
    is_touched = true;
}

void Widget::untouched(void) {
    is_touched = false;
}

void Widget::drawBorder(u8* fb, u32 colour) {
    drawBox(0, 0, width, height, colour);
}



void Widget::drawBresLine(u8* fb, u16 tx1, u16 ty1, u16 tx2, u16 ty2, u32 col)
{
    u8 r = RED(col);
    u8 g = GREEN(col);
    u8 b = BLUE(col);

    u32 x1, y1, x2, y2;
    x1 = tx1 + x;
    x2 = tx2 + x;
    y1 = ty1 + y;
    y2 = ty2 + y;

    // Guarantees that all lines go from left to right
    if (x2 < x1)
    {
        u32 tmp;
        tmp = x2; x2 = x1; x1 = tmp;
        tmp = y2; y2 = y1; y1 = tmp;
    }

    s32 dy, dx;
    dy = y2 - y1;
    dx = x2 - x1;

    // If the gradient is greater than one we have to flip the axes
    if (abs(dy) < dx)
    {
        u16 xp, yp;
        s32 d;
        s32 add = 1;

        xp = x1;
        yp = y1;

        if (dy < 0)
        {
            dy = -dy;
            add = -1;
        }

        d = 2 * dy - dx;

        for (; xp <= x2; xp++)
        {
            if (d > 0)
            {
                yp += add;
                d -= 2 * dx;
            }

            // *(*vram+SCREEN_WIDTH*yp+xp) = col;

            drawPixel(fb, xp - x, yp - y, r, g, b); // account for widget offset

            d += 2 * dy;
        }
    }
    else
    {
        u16 tmp;
        tmp = x1; x1 = y1; y1 = tmp;
        tmp = x2; x2 = y2; y2 = tmp;

        if (x2 < x1)
        {
            tmp = x2; x2 = x1; x1 = tmp;
            tmp = y2; y2 = y1; y1 = tmp;
        }

        u16 xp, yp;
        s32 d;

        dy = y2 - y1;
        dx = x2 - x1;

        s32 add = 1;

        if (dy < 0)
        {
            dy = -dy;
            add = -1;
        }

        xp = x1;
        yp = y1;

        d = 2 * dy - dx;

        for (xp = x1; xp <= x2; xp++) {

            if (d > 0)
            {
                yp += add;
                d -= 2 * dx;
            }

            drawPixel(fb, yp - y, xp - x, r, g, b);

            d += 2 * dy;
        }
    }
}

inline const u32 interpolateColour(u32 colour1, u32 colour2, int alpha /* 0 -> 256 */) {
    // even though this is the 3ds, we still want to save var alloc and mem when possible!
    return RGB(RED(colour1) * alpha / 255 + RED(colour2) - RED(colour2) * alpha / 255,
        GREEN(colour1) * alpha / 255 + GREEN(colour2) - GREEN(colour2) * alpha / 255,
        BLUE(colour1) * alpha / 255 + BLUE(colour2) - BLUE(colour2) * alpha / 255);
}

void Widget::drawGradient(u8* fb, u32 col1, u32 col2, u16 tx, u16 ty, u16 tw, u16 th) {
    if (col1 == col2) {
        drawFilledBox(fb, tx, ty, tw, th, col1);
        return;
    }

    u8 j;
    u32 col;

    if (tw == 0) return;

    int bw = tw * 2;
    // int step = div32((1<<12), th);
    u32 step = (u32)256 / (u32)th;

    u32 pos = 0;
    printf("th is %u\n", th);
    for (j = 0;j < th;++j, pos += step) {
        col = interpolateColour(col1, col2, pos);
        drawHLine(fb, tx, ty + j, tw, col);
        // dmaFillHalfWords(col, (*vram+SCREEN_WIDTH*(y+ty+j)+(x+tx)), bw); // todo find out how 3ds dma works and use that instead
    }
}


void Widget::drawString(const char* str, u16 tx, u16 ty, u8 tz, u16 maxwidth, u32 colour, u16 maxheight)
{
    // Draw text
    u8 charidx, i, j;
    u16 drawpos = 0; u8 col;

    u8 fontheight = font_8x11.height;
    u8 height = fontheight;
    if (height > maxheight) height = maxheight;

    while ((*str != '\0') && (drawpos + 6 < maxwidth))
    {
        charidx = font_8x11.char_index[(u8)*str];
        u8 width = font_8x11.char_widths[charidx];

        for (j = 0;j < height;++j) {
            col = font_8x11.data[fontheight * charidx + j];
            for (i = 0;i < 8;++i, col >>= 1) {
                // Print a character from the bitmap font
                // each char is 8 pixels wide, and 8 pixels
                // are in a byte.
                if (col & 1) {
                    // drawPixel(i+tx+drawpos, j+ty, color);

                }
            }
        }

        drawpos += width + 1;
        str++;
    }
}

