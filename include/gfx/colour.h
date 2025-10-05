#pragma once
#include <3ds/types.h>


#define RED(colour)  (u8)((colour >> 16) & 0xff)
#define GREEN(colour)  (u8)((colour >> 8) & 0xff)
#define BLUE(colour)  (u8)(colour & 0xff)

#define RGB(r, g, b) r << 16 | g << 8 | b 

namespace Colour {
    static const u32 BLACK = C2D_Color32(0, 0, 0, 255);
    static const u32 MAGENTA = C2D_Color32(255, 0, 255, 255);
    static const u32 RED = C2D_Color32(255, 0, 0, 255);
}