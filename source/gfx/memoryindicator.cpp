/*====================================================================
Copyright 2006 Tobias Weyand

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
======================================================================*/

#include <stdio.h>
#include <malloc.h>

#include "gfx/memoryindicator.h"
#include <sys/unistd.h>

#define clamp(v, vmin, vmax) (((v) < (vmin)) ? (vmin) : ((v > (vmax)) ? (vmax) : (v)))

/* https://devkitpro.org/viewtopic.php?f=6&t=3057 */

extern u8 *fake_heap_end;
extern u8 *fake_heap_start;

static int getFreeMem() {
	struct mallinfo mi = mallinfo();
	return mi.fordblks + (fake_heap_end - (u8*)sbrk(0));
}

/* ===================== PUBLIC ===================== */

MemoryIndicator::MemoryIndicator(u16 _x, u16 _y, u16 _width, u16 _height, bool _visible)
	:Widget(_x, _y, _width, _height, _visible)
{
	total_ram = getFreeMem(); // only estimate!
}

MemoryIndicator::~MemoryIndicator()
{
}
// Drawing request
void MemoryIndicator::pleaseDraw(void)
{
	if (isExposed())
		draw();
}

/* ===================== PRIVATE ===================== */

#define COL_INDICATOR_OK (RGB15(17,24,16))
#define COL_INDICATOR_WARNING (RGB15(31,31,0))
#define COL_INDICATOR_ALERT (RGB15(31,0,0))

void MemoryIndicator::draw(void)
{
	if (!isExposed())
		return;

	u32 free_ram = getFreeMem();
	u32 used_ram = total_ram - free_ram;
	
	int boxwidth = clamp((width - 2) * used_ram / total_ram, 0, (u32) (width - 2));
	int percentfull = clamp(100 * used_ram / total_ram, 0, 100);
	
	// Color depends on percentage of used RAM
	u16 col;
	if(percentfull < 62)
		col = COL_INDICATOR_OK;
	else if(percentfull < 78)
		col = interpolateColour(COL_INDICATOR_WARNING, COL_INDICATOR_OK, (percentfull - 62) << 8);
	else if(percentfull < 94)
		col = interpolateColour(COL_INDICATOR_ALERT, COL_INDICATOR_WARNING, (percentfull - 78) << 8);
	else
		col = COL_INDICATOR_ALERT;

	drawBorder(theme->col_outline);
	drawFilledBox(1, 1, width-2, height-2, theme->col_light_bg);
	drawFilledBox(1, 1, boxwidth, height-2, col);
}
