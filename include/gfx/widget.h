#pragma once
#include <3ds/types.h>
#include <c2d/base.h>
#include "gfx.h"
#include "colour.h"

class Widget {
    public:
        Widget(u8 _x, u8 _y, u8 _width, u8 _height, bool _visible=true, bool _occluded=true);
        virtual ~Widget(void) {}

        bool is_visible(void) { return visible; }
        bool set_visible(bool visible_) {
            visible = visible_;
            on_visibility_changed(visible);
        }

        
        void show(void) { set_visible(true); };
        void hide(void) { set_visible(false); };
        void pleaseDraw(void) { draw(); }
        
        bool is_occluded(void) { return occluded; }
        bool set_occluded(bool occluded_) { occluded = occluded_; }

        virtual void touched(void);
        virtual void untouched(void);
    protected:
        u16 x, y;
        u16 width, height;

        void drawBox(u8 tx, u8 ty, u8 tw, u8 th, u32 colour=Colour::BLACK);
        void drawFilledBox(u8 tx, u8 ty, u8 tw, u8 th, u32 colour=Colour::BLACK);
        void drawBorder(u32 colour);

        void draw(void);

    private:
        bool is_touched;
        bool visible;
        bool occluded;
        virtual void on_visibility_changed(bool visibility);
};