#include <citro2d.h>
#include <c2d/base.h>

#include "gfx/widget.h"
#include "gfx/colour.h"

Widget::Widget(u8 _x, u8 _y, u8 _width, u8 _height, bool _visible, bool _occluded)
    : x(_x), y(_y), width(_width), height(_height), visible(_visible), occluded(_occluded), is_touched(false) {

    }



void Widget::drawBox(u8 tx, u8 ty, u8 tw, u8 th, u32 colour) {
    C2D_DrawRectSolid(tx, ty, 0, tw, th, colour);
}

void Widget::draw(void) {
    drawBox(x, y, width, height, is_touched ? Colour::RED : Colour::MAGENTA);
}

void Widget::on_visibility_changed(bool visible) {

}

void Widget::touched(void) {
    is_touched = true;
}

void Widget::untouched(void) {
    is_touched = false;
}