// adapted from widget.cpp in tobkit
// by Tobias Weyand, licensed under
// the Apache License version 2.0

#include <math.h>
#include <stdio.h>

#include "gfx/widget.h"
#include "gfx/colour.h"
#include "gfx/font/font_8x11.inc"
#include "gfx/widget.h"



Widget::Widget(u16 _x, u16 _y, u16 _width, u16 _height,  bool _visible, bool _occluded)
    : x(_x), y(_y), width(_width), height(_height), enabled(true), visible(_visible), occluded(_occluded), is_touched(false) {
        occluded = false; //todo change lol
}

void Widget::getPos(u16 *_x, u16 *_y, u16 *_width, u16 *_height)
{
	if (_x != NULL) *_x = x;
	if (_y != NULL) *_y = y;
	if (_width != NULL) *_width = width;
	if (_height != NULL) *_height = height;
}

void Widget::setPos(u16 _x, u16 _y)
{
	x = _x;
	y = _y;
}

void Widget::occlude(void)
{
	if(isExposed())
        overdraw();
	occluded = true;
}

void Widget::resize(u16 w, u16 h) {
	overdraw();
	width = w;
	height = h;
	pleaseDraw();
}

void Widget::reveal(void)
{
	if(occluded)
	{
		occluded = false;
		if(visible)
			pleaseDraw();
	}
}

void Widget::drawBox(u16 tx, u16 ty, u16 tw, u16 th, u32 colour) {
    if (th <= 0 || tw <= 0) return;

    u8 r = RED(colour);
    u8 g = GREEN(colour);
    u8 b = BLUE(colour);

    uint_fast8_t i, j;
    for (i = 0;i < tw;++i) {
        drawPixel(i + tx, ty, r, g, b);
        drawPixel(i + tx, ty + th - 1, r, g, b);
    }
    for (j = 1;j < th - 1;++j) {
        drawPixel(tx, ty + j, r, g, b);
        drawPixel(tx + tw - 1, ty + j, r, g, b);
    }
}

void Widget::drawHLine(u16 tx, u16 ty, u16 length, u32 col) {
    u8 r = RED(col); // don't convert these for every pixel drawn
    u8 g = GREEN(col);
    u8 b = BLUE(col);

    for (int i = 0;i < length;++i) {
        drawPixel(i + tx, ty, r, g, b);
    }
}

void Widget::drawVLine(u16 tx, u16 ty, u16 length, u32 col) {
    u8 r = RED(col);
    u8 g = GREEN(col);
    u8 b = BLUE(col);

    for (int i = 0;i < length;++i) {
        drawPixel(tx, i + ty, r, g, b);
    }
}

void Widget::drawFilledBox(u16 tx, u16 ty, u16 tw, u16 th, u32 colour) {
    if (tw == 0) return;

    // int bw = (int)tw * 2;
    for (int j = 0;j < th;++j) {
        drawHLine(tx, ty + j, tw, colour); // todo use DMA
    }
}

void Widget::enable(void)
{
	if(!enabled)
	{
		enabled = true;
		pleaseDraw();
	}
}

void Widget::disable(void)
{
	if(enabled)
	{
		enabled = false;
		pleaseDraw();
	}
}

void Widget::onVisibilityChanged(bool visible) {

}

void Widget::show(void)
{
	if(!visible)
	{
		visible = true;
		if(!occluded)
			pleaseDraw();
	}
}

void Widget::hide(void)
{
	if(isExposed())
		overdraw();
	visible = false;
}

bool Widget::setOccluded(bool value)
{
	bool changed = value != occluded;
	if (value) occlude(); else reveal();
	return changed;
}

bool Widget::setEnabled(bool value)
{
	bool changed = value != enabled;
	if (value) enable(); else disable();
	return changed;
}

void Widget::touched(void) {
    is_touched = true;
}

void Widget::untouched(void) {
    is_touched = false;
}

void Widget::drawBorder(u32 colour) {
    drawBox(0, 0, width, height, colour);
}

void Widget::setTheme(Theme *theme_, u32 bgcolour) {
    theme = theme_;
    bgColour = bgcolour;
}

void Widget::setFramebuf(u8 *framebuffer) {
    fb = framebuffer;
}

void Widget::drawBresLine(u16 tx1, u16 ty1, u16 tx2, u16 ty2, u32 col)
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

            drawPixel(xp - x, yp - y, r, g, b); // account for widget offset

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

            drawPixel(yp - y, xp - x, r, g, b);

            d += 2 * dy;
        }
    }
}



void Widget::drawGradient(u32 col1, u32 col2, u16 tx, u16 ty, u16 tw, u16 th) {
    if (col1 == col2) {
        drawFilledBox(tx, ty, tw, th, col1);
        return;
    }

    u8 j;
    u32 col;

    if (tw == 0) return;

    int bw = tw * 2;
    // int step = div32((1<<12), th);
    u32 step = (u32)256 / (u32)th;

    u32 pos = 0;
    for (j = 0;j < th;++j, pos += step) {
        col = interpolateColour(col1, col2, pos);
        drawHLine(tx, ty + j, tw, col);
        // dmaFillHalfWords(col, (*vram+SCREEN_WIDTH*(y+ty+j)+(x+tx)), bw); // todo find out how 3ds dma works and use that instead
    }
}


void Widget::drawString(const char* str, u16 tx, u16 ty, u8 tz, u16 maxwidth, u32 colour, u16 maxheight)
{
    u8 r = RED(colour);
    u8 g = GREEN(colour);
    u8 b = BLUE(colour);

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
                    drawPixel(i+tx+drawpos, j+ty, r, g, b);

                }
            }
        }

        drawpos += width + 1;
        str++;
    }
}

void Widget::drawMonochromeIcon(u16 tx, u16 ty, u16 tw, u16 th, const u8 *icon, u32 colour) {
	u16 pixelidx = 0;
	for(u8 j=0;j<th;++j) {
		for(u8 i=0;i<tw;++i,++pixelidx) {
			if(icon[pixelidx/8] & BIT(pixelidx%8) ) {
				drawPixel(tx+i, ty+j, colour);
			}
		}
	}
}

void Widget::drawMonochromeIconOffset(u16 tx, u16 ty, u16 tw, u16 th, u16 ix, u16 iy, u16 iw, u16 ih, const u8 *icon, u32 colour) {
	for(u8 j=0;j<th;++j) {
		u16 pixelidx = ((iy+j) * iw) + ix;
		for(u8 i=0;i<tw;++i,++pixelidx) {
			if(icon[pixelidx/8] & BIT(pixelidx%8) ) {
				drawPixel(tx+i, ty+j, colour);
			}
		}
	}
}

u32 Widget::getStringWidth(const char *str, u16 limit)
{
	u32 res = 0;
	for(u16 i=0; i<limit; ++i, ++str) {
		char c = *str;
		if (c == '\0') {
			return res;
		}
		res += font_8x11.char_widths[font_8x11.char_index[(u8) *str]] + 1;
	}
	return res;
}

bool Widget::isInRect(u16 x, u16 y, u16 x1, u16 y1, u16 x2, u16 y2)
{
	return ( (x >= x1) && (x <= x2) && (y >= y1) && (y <= y2) );
}

bool Widget::isExposed(void)
{
	return visible && !occluded;
}

void Widget::overdraw(void)
{
	drawFilledBox(0, 0, width, height, bgColour);
}