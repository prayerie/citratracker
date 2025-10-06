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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <3ds/types.h>

#include "gfx/label.h"

/* ===================== PUBLIC ===================== */

Label::Label(u16 _x, u16 _y, u16 _width, u16 _height, bool _has_border, bool _albino, bool _no_bg, bool _right_aligned)
	:Widget(_x, _y, _width, _height),
	onPush(0), caption(0), has_border(_has_border), is_albino(_albino), no_bg(_no_bg), right_aligned(_right_aligned)
{

}

Label::~Label(void)
{
	if (caption) free(caption);
}

// Callback registration
void Label::registerPushCallback(void (*onPush_)(void))
{
	onPush = onPush_;
}

// Drawing request
void Label::pleaseDraw(void) {
	if(isExposed())
		draw();
}

// Event calls
void Label::penDown(u16 x, u16 y)
{
	if(onPush != 0) {
		onPush();
	}
}

void Label::setCaption(const char *_caption)
{
	if (caption) free(caption);

	caption = (char*)malloc(strlen(_caption)+1);
	strcpy(caption, _caption);

	draw();
}

char *Label::getCaption(void) {
	return caption;
}

/* ===================== PRIVATE ===================== */

void Label::draw(void)
{
	// if(!isExposed())
	// 	return;

	u32 col_bg, col_text;
	if(is_albino)
	{
		col_bg = theme->col_dark_bg;
		col_text = theme->col_lighter_bg;
	}
	else
	{
		col_bg = theme->col_light_bg;
		col_text = theme->col_text;
	}

	int caption_x_offset = 0;
	int caption_y_offset = 0;

	if(has_border)
	{
		if(!no_bg)
			drawFilledBox(0, 0, width, height, theme->col_lighter_bg);
		drawBorder();
		caption_x_offset += 2;
		caption_y_offset += 2;
	}
	else
	{
		if(!no_bg)
			drawFilledBox(0, 0, width, height, col_bg);
	}
    printf("A");
	if(caption) {
		int caption_width = width - (caption_x_offset * 2);
		if(right_aligned) {
			// TODO: this is a kludge...
			const char *caption_local = caption;
			int string_width = getStringWidth(caption_local);
			while (string_width > caption_width) {
				caption_local++;
				string_width = getStringWidth(caption_local);
			}
			drawString(caption_local,caption_x_offset,caption_y_offset,0,caption_width,col_text);
		} else {
			drawString(caption,caption_x_offset,caption_y_offset,0,caption_width,col_text);
		}
	}
}
