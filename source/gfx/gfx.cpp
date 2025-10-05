#include <3ds.h>
#include <citro2d.h>
#include <cstdarg>
#include <cstdio>
#include <malloc.h>

#include "gfx/gfx.h"
namespace GFX {
    void Init(void) {
        romfsInit();
        gfxInitDefault();
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        C2D_Prepare();

        c3dRenderTarget[TARGET_TOP] = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
        c3dRenderTarget[TARGET_BOTTOM] = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

        // guiStaticBuf  = C2D_TextBufNew(4096);
        // guiDynamicBuf  = C2D_TextBufNew(4096);
        // guiSizeBuf = C2D_TextBufNew(4096);

        // Textures::Init();
#if defined BUILD_DEBUG
        Log::Open();
#endif
        // // Real time services
        // ptmuInit();
        // cfguInit();
        // dspInit();
        // socInit(static_cast<u32 *>(memalign(0x1000, 0x10000)), 0x10000);
    }

    void Exit(void) {
        // socExit();
        // dspExit();
        // cfguExit();
        // ptmuExit();
#if defined BUILD_DEBUG
        Log::Close();
#endif
        // Textures::Exit();
        // C2D_TextBufDelete(guiSizeBuf);
        // C2D_TextBufDelete(guiDynamicBuf);
        // C2D_TextBufDelete(guiStaticBuf);
        C2D_Fini();
        C3D_Fini();
        gfxExit();
        romfsExit();
    }

    void FrameBegin(u32 topScreenColour, u32 bottomScreenColour) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(c3dRenderTarget[TARGET_TOP], topScreenColour);
        C2D_TargetClear(c3dRenderTarget[TARGET_BOTTOM], bottomScreenColour);
        C2D_SceneBegin(c3dRenderTarget[TARGET_TOP]);
        C2D_SceneBegin(c3dRenderTarget[TARGET_BOTTOM]);
    }

    void FrameEnd(void) {
        // C2D_TextBufClear(guiDynamicBuf);
        // C2D_TextBufClear(guiSizeBuf);
        C3D_FrameEnd(0);
    }

}