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
#include <string.h>
#include <stdlib.h>

#include "gfx/button.h"

/* ===================== PUBLIC ===================== */

Button::Button(u16 _x, u16 _y, u16 _width, u16 _height, bool _visible)
	:Widget(_x, _y, _width, _height, _visible), // not passing framebuffer pointer to widgets -- it needs to be explicitly specified on each call
	 penIsDown(false), caption(0)
{
	onPush = 0;
    theme = new Theme();
}

Button::~Button()
{
	if (caption) free(caption);
}

void Button::registerPushCallback(void (*onPush_)(void)) {
	onPush = onPush_;
}
		
// Drawing request
void Button::pleaseDraw(void) {
	draw(penIsDown);
}
		
// Event calls
void Button::penDown(u16 px, u16 py)
{
    if (!isInRect(px, py, x, y, x + width, y + height))
        return;
        
	penIsDown = true;
	draw(1);
}

void Button::penUp(u16 px, u16 py)
{
	penIsDown = false;
	draw(0);
	if(onPush) {
		onPush();
	}
}

void Button::penMove(u16 px, u16 py) {

}

void Button::buttonPress(u16 button) {
	if(onPush) {
		onPush();
	}
}

void Button::setCaption(const char *_caption) {
	if (caption) free(caption);
	caption = (char*)malloc(strlen(_caption)+1);
	strcpy(caption, _caption);
}

/* ===================== PRIVATE ===================== */

void Button::draw(u8 down) {
	if(!isExposed()) return;
	
	if(enabled)
	{
		if(down) {
			drawGradient(theme->col_light_ctrl, theme->col_dark_ctrl, 0, 0, width, height);
		} else {
			drawGradient(theme->col_dark_ctrl, theme->col_light_ctrl, 0, 0, width, height);
		}
	} else {
		drawGradient(theme->col_light_ctrl_disabled, theme->col_dark_ctrl_disabled, 0, 0, width, height);
	}
	drawBorder();
	// printf("here2\n");
	drawString(caption, (width-getStringWidth(caption))/2, height/2-5, 0);
}
