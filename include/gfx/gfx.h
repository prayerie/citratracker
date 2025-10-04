#pragma once
// #include <types.h>


namespace GFX {
    enum {
        TARGET_TOP = 0,
        TARGET_BOTTOM,
        TARGET_BOTH
    };

    static C3D_RenderTarget *c3dRenderTarget[TARGET_BOTH];
    static C2D_TextBuf guiStaticBuf, guiDynamicBuf, guiSizeBuf;
    
    static const u32 guiBgcolour = C2D_Color32(62, 62, 62, 255);
    static const u32 guiStatusBarColour = C2D_Color32(44, 44, 44, 255);
    static const u32 guiMenuBarColour = C2D_Color32(52, 52, 52, 255);
    static const u32 guiSelectorColour = C2D_Color32(223, 74, 22, 255);
    static const u32 guiTitleColour = C2D_Color32(252, 252, 252, 255);
    static const u32 guiDescrColour = C2D_Color32(182, 182, 182, 255);

    
    void Init(void);
    void Exit(void);
    void MainMenu(void);
    void FrameBegin(u32 upperBg=guiBgcolour, u32 lowerBg=guiBgcolour); // static?
    void FrameEnd(void); // static?
}
