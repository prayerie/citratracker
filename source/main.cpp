#include <3ds.h>
#include <stdio.h>

#include "gfx/widget.h"

int main(int argc, char** argv) {
    // #ifdef BUILD_DEBUG
    
    // #endif

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    printf("hi\n");
    gfxSetDoubleBuffering(GFX_BOTTOM, false);
    u8 *fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
    
    Widget* w = new Widget(10, 10, 200, 10, true, true);

    while (aptMainLoop()) {

        if (hidKeysDown() & KEY_START)
            break;
        w->pleaseDraw(fb);
        
        hidScanInput();

        touchPosition touch;
        if (hidKeysHeld() & KEY_TOUCH) {
            hidTouchRead(&touch);
            
            if (touch.px > 10 && touch.px < 210 && touch.py > 10 && touch.py < 20) {
                w->touched();
            }
        } else {
            w->untouched();
        }

        gfxFlushBuffers();
        gfxSwapBuffers();

        gspWaitForVBlank();
    }

    gfxExit();
    return 0;

}
