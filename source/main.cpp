#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

#include "gfx/gfx.h"
#include "gfx/widget.h"

int main(int argc, char** argv) {
    // #ifdef BUILD_DEBUG
    
    // #endif
    //    Textures::Init();
    //    ptmuInit();
    //    cfguInit();
    //    dspInit();
    //    socInit(static_cast<u32 *>(memalign(0x1000, 0x10000)), 0x10000);
    GFX::Init();
    consoleInit(GFX_TOP, NULL);
    printf("hi\n");

    Widget* w = new Widget(10, 10, 200, 10, true, true);
    while (aptMainLoop()) {
        // gfxFlushBuffers();
        // gfxSwapBuffers();
        GFX::FrameBegin();

        
        

        // if (hidKeysDown() & KEY_START)
        //     break;
        w->pleaseDraw();
        GFX::FrameEnd();




        
        // gspWaitForVBlank();
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
    }

    GFX::Exit();
    return 0;

}
