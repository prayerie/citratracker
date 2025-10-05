#pragma once
#include <3ds/types.h>
#include "colour.h"

struct Font {
	u8 width, height;
	const u8* char_index;
	const u8* char_widths;
	const u8* data;
};

#define SCREEN_WIDTH_TOP 400
#define SCREEN_WIDTH_BOTTOM 320
#define SCREEN_HEIGHT 240

class Widget {
    public:
        Widget(u8 _x, u8 _y, u8 _width, u8 _height, bool _visible=true, bool _occluded=true);
        virtual ~Widget(void) {}

        bool isVisible(void) { return visible; }
        bool setVisible(bool visible_) {
            visible = visible_;
            onVisibilityChanged(visible);
        }

        
        void show(void) { setVisible(true); }
        void hide(void) { setVisible(false); }
        void pleaseDraw(u8 *fb) { draw(fb); } // TODO no
        
        bool isOccluded(void) { return occluded; }
        bool setOccluded(bool occluded_) { occluded = occluded_; }

        virtual void touched(void);
        virtual void untouched(void);
    protected:
        u16 x, y;
        u16 width, height;

        void drawBox(u8 *fb, u16 tx, u16 ty, u16 tw, u16 th, u32 colour=Colour::BLACK);
        void drawFilledBox(u8 *fb, u16 tx, u16 ty, u16 tw, u16 th, u32 colour=Colour::BLACK);
		void drawBorder(u8 *fb, u32 colour = 0x000000);
		void drawHLine(u8 *fb, u16 tx, u16 ty, u16 length, u32 colour);
		void drawVLine(u8 *fb, u16 tx, u16 ty, u16 length, u32 colour);
		void drawBresLine(u8 *fb, u16 tx1, u16 ty1, u16 tx2, u16 ty2, u32 col);

        
		void drawGradient(u8 *fb, u32 col1, u32 col2, u16 tx, u16 ty, u16 tw, u16 th); // todo maybe alias (u8*) as Framebuffer?
        
        void draw(u8 *fb);
        void drawString(const char* str, u16 tx, u16 ty, u8 tz, u16 maxwidth=400, u32 colour=Colour::BLACK, u16 maxheight=320);

        // tx, tx are widget space, not screen space!
        
        // 3ds framebuffer structure:
        // [u8 BLUE1 u8 GREEN1 u8 RED1][u8 BLUE2 u8 GREEN2 u8 RED2] etc...
        // sadly cannot represent colour in a single value...
        // unless we use u32, which would waste one whole byte........
        inline void drawPixel(u8 *fb, u16 tx, u16 ty, u8 r, u8 g, u8 b, bool wide=false) {
            u16 y_ = 240 - y - ty;
            u32 c = (240)*(x+tx)+y_; // 3ds screens are portrait internally
            u32 cb = c * 3;
            u32 cg = c * 3 + 1;
            u32 cr = c * 3 + 2;

            fb[cb] = b;
            fb[cg] = g;
            fb[cr] = r;
        }

        inline void drawPixel(u8 *fb, u16 tx, u16 ty, u32 colour, bool wide=false) {
            drawPixel(fb, tx, ty, RED(colour), GREEN(colour), BLUE(colour));
        }
    private:
        bool is_touched;
        bool visible;
        bool occluded;
        virtual void onVisibilityChanged(bool visibility);
};