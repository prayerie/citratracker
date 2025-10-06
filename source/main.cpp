#include <3ds.h>
#include <stdio.h>

#include "gfx/widget.h"
#include "gfx/button.h"
#include "gfx/listbox.h"
#include "gfx/label.h"

int main(int argc, char** argv) {
    // #ifdef BUILD_DEBUG
    
    // #endif

    gfxInitDefault();
    gfxSetDoubleBuffering(GFX_TOP, false);
    gfxSetDoubleBuffering(GFX_BOTTOM, false);

    u8 *fb_main_l = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    // u8 *fb_main_r = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
    u8 *fb_sub = gfxGetFramebuffer(GFX_BOTTOM, GFX_RIGHT, NULL, NULL);

    bool was_just_touch = false;
    
    Theme *t = new Theme();
    Button *b = new Button(10, 10, 50, 12);
    b->setFramebuf(fb_sub);
    b->setCaption("hello!!!");
    Widget *bgg = new Widget(0, 1, 400, 240, true, false);
    Widget *bggg = new Widget(0, 1, 320, 240, true, false);

    Label *l_l = new Label(200, 120, 50, 14);
    Label *l_r = new Label(203, 120, 50, 14);

    

    bgg->setFramebuf(fb_main_l);
    bgg->setTheme(t);
    bggg->setFramebuf(fb_sub);
    bggg->setTheme(t);
    

    ListBox *l = new ListBox(141, 32, 114, 89, 0x80, true, true, false);
    
    l->setFramebuf(fb_sub);
    b->setTheme(t);
    
    bgg->drawFullBg();
    bggg->drawFullBg();
l_l->setFramebuf(fb_main_l);
    l_l->setTheme(t);
    l_l->setCaption("hi");
    l->select(0);

    while (aptMainLoop()) {
    l->set(1, "fgdfsg");

        if (hidKeysDown() & KEY_START)
            break;
        l->pleaseDraw();
        b->pleaseDraw();
        l_l->pleaseDraw();

        hidScanInput();

        touchPosition touch;
        if (hidKeysHeld() & KEY_TOUCH) {
            hidTouchRead(&touch);
            
            l->penDown(touch.px, touch.py);
            b->penDown(touch.px, touch.py);
            was_just_touch = true;
        } else {
            if (was_just_touch) {
                l->penUp(0, 0);
                b->penUp(0, 0);
                was_just_touch = false;
            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();

        gspWaitForVBlank();
    }

    gfxExit();
    return 0;

}
