#pragma once
#include <3ds/types.h>


#define RED(colour)  (u8)((colour >> 16) & 0xff)
#define GREEN(colour)  (u8)((colour >> 8) & 0xff)
#define BLUE(colour)  (u8)(colour & 0xff)


#define RED15(col)    (((col) & 0x1F) << 3)
#define GREEN15(col)  ((((col) >> 5) & 0x1F) << 3)
#define BLUE15(col)   ((((col) >> 10) & 0x1F) << 3)

#define RGB(r, g, b) (r << 16) | (g << 8) | (b) 

#define RGB15(r, g, b) RGB((u8)(r*8), (u8)(g*8), (u8)(b*8)) // so we can port over the NitroTracker skin easily, will be made obsolete

namespace Colour {
    static const u32 BLACK = RGB(0, 0, 0);
    static const u32 MAGENTA = RGB(255, 0, 255);
    static const u32 RED = RGB(255, 0, 255);
}