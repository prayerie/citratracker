#include <3ds.h>
#include <stdio.h>

#include "gfx/widget.h"
#include "gfx/button.h"
#include "gfx/listbox.h"
#include "gfx/label.h"
#include "gfx/numberslider.h"
#include "gfx/gui.h"

int main(int argc, char** argv) {
    // #ifdef BUILD_DEBUG
    
    // #endif

    gfxInitDefault();
    gfxSetDoubleBuffering(GFX_TOP, false);
    gfxSetDoubleBuffering(GFX_BOTTOM, false);

    u8 *fb_main_l = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
    // u8 *fb_main_r = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
    u8 *fb_sub = gfxGetFramebuffer(GFX_BOTTOM, GFX_RIGHT, NULL, NULL);

    GUI *gui = new GUI(fb_main_l, fb_sub);
    bool was_just_touch = false;
    
    Theme *t = new Theme();
    Button *b = new Button(10, 10, 50, 12);
    b->setFramebuf(fb_sub);
    b->setCaption("hello!!!");
    Widget *bgg = new Widget(0, 1, 400, 240, true, false);
    Widget *bggg = new Widget(0, 1, 320, 240, true, false);

    Label *l_l = new Label(200, 120, 50, 14);
    Label *l_r = new Label(203, 120, 50, 14);
    NumberSlider *ns = new NumberSlider(105, 60, 32, 17, 12, 1, 256, true);
    ns->setFramebuf(fb_sub);
    ns->setTheme(t);
    

    bgg->setFramebuf(fb_main_l);
    bgg->setTheme(t);
    bggg->setFramebuf(fb_sub);
    bggg->setTheme(t);
    
    u16 last_x = 0;
    u16 last_y = 0;
    ListBox *l = new ListBox(141, 32, 114, 89, 0x80, true, true, false);
    
    l->setFramebuf(fb_sub);
    b->setTheme(t);
    
    bgg->drawFullBg();
    bggg->drawFullBg();
    l_l->setFramebuf(fb_main_l);
    l_l->setTheme(t);
    l_l->setCaption("hi");  
    gui->setTheme(t, t->col_bg);
    gui->registerWidget(b, 0, SUB_SCREEN);
    gui->registerWidget(ns, 0, SUB_SCREEN);
    gui->registerWidget(l, 0, SUB_SCREEN);
    gui->registerWidget(l_l, 0, MAIN_SCREEN);
    while (aptMainLoop()) {
    l->set(1, "fgdfsg");

        if (hidKeysDown() & KEY_START)
            break;

        hidScanInput();
        gui->draw();
        touchPosition touch;
        if (hidKeysHeld() & KEY_TOUCH) {
            hidTouchRead(&touch);
            if (touch.px != last_x || touch.py != last_y)
                gui->penMove(touch.px, touch.py);

            last_x = touch.px;
            last_y = touch.py;
            gui->penDown(touch.px, touch.py);
            was_just_touch = true;
        } else {
            if (was_just_touch) {

                gui->penUp(last_x, last_y);
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
