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

#include <stdlib.h>
#include <stdarg.h>

#include "gfx/messagebox.h"

#include <cstring>
#include <cstdlib>

#define MB_MIN_WIDTH	150
#define MB_HEIGHT	46


/* ===================== PUBLIC ===================== */

// Takes a list of alternating button captions and callbacks
MessageBox::MessageBox(const char *message, u8 n_buttons, u8 *fb_main_, u8 *fb_sub_, ...)
	:Widget((SCREEN_WIDTH_BOTTOM-MB_MIN_WIDTH)/2, (SCREEN_HEIGHT-MB_HEIGHT)/2, MB_MIN_WIDTH, MB_HEIGHT),
	n_buttons(n_buttons), gui(fb_main_, fb_sub_)
{
	msg = (char*)calloc(1, strlen(message)+1);
	strcpy(msg, message);

	// Go through the list of given captions/callback pointers and add a button for each of them
	callbacks = (void (**)(void))malloc(sizeof(void (*)(void))*n_buttons);

	va_list marker;
	va_start(marker, n_buttons);

	// Calculate width
	width = 10;
	char *str;
	// void (*onPush)(void);
	for(u8 i=0;i<n_buttons;++i)
	{
		str = va_arg(marker, char*);
		/* onPush = */ va_arg(marker, void (*)(void));
		width += getStringWidth(str) + 14;
	}
	va_end(marker);

	int minwidth = getStringWidth(message) + 8;
	if (minwidth < MB_MIN_WIDTH)
		minwidth = MB_MIN_WIDTH;

	int fixedbuttonwidth = 0;
	if(width < minwidth)
	{
		width = minwidth;
		if(n_buttons>0) {
			fixedbuttonwidth = (width - 10) / n_buttons - 10;
		}
	} else if(width > 256) {
		width = 256;
	}

	// Set x
	x = (SCREEN_WIDTH_BOTTOM-width)/2;

	if(n_buttons > 0)
	{
		u8 xpos = x + 10;
		buttons = (Button**)malloc(sizeof(Button*)*n_buttons);
		u8 buttonwidth;

		va_start(marker, n_buttons);

		for(u8 i=0; i<n_buttons; ++i)
		{
			char* caption = va_arg(marker, char*);
			void (*onPush)(void) = va_arg(marker, void (*)(void));

			if(fixedbuttonwidth == 0) {
				buttonwidth = getStringWidth(caption)+4;
			} else {
				buttonwidth = fixedbuttonwidth;
			}
			buttons[i] = new Button(xpos, y+24, buttonwidth, 14, true);
			gui.registerWidget(buttons[i], 0);
			buttons[i]->setCaption(caption);
			buttons[i]->registerPushCallback(onPush);

			xpos += buttonwidth + 10;
		}

		va_end(marker);

	}
}

MessageBox::~MessageBox(void)
{
	free(msg);

	//delete label;
	free(callbacks);
	for(u8 i=0; i<n_buttons; ++i) {
		delete buttons[i];
	}
	if(n_buttons > 0) {
		free(buttons);
	}
}


// Drawing request
void MessageBox::pleaseDraw(void) {
	draw();
}


// Event calls
void MessageBox::penDown(u16 x, u16 y) {
	gui.penDown(x, y);
}

void MessageBox::penUp(u16 x, u16 y) {
	gui.penUp(x, y);
}

void MessageBox::show(void)
{
	gui.showAll();
	if(!isExposed())
	{
		Widget::show();
		pleaseDraw();
	}
}

void MessageBox::reveal(void)
{
	Widget::reveal();
	gui.revealAll();
}

void MessageBox::setTheme(Theme *theme_, u16 bgcolour_)
{
	theme = theme_;
	bgColour = bgcolour_;
	gui.setTheme(theme, theme->col_light_bg);
}

/* ===================== PRIVATE ===================== */

void MessageBox::draw(void)
{
	drawGradient(theme->col_list_highlight1, theme->col_list_highlight2, 0, 0, width, 16);
	drawHLine(0, 16, width, RGB15(0,0,0)|BIT(15));
	drawFilledBox(0, 17, width, MB_HEIGHT-17, theme->col_light_bg);
	drawBorder();
	u8 labelx = (width-getStringWidth(msg))/2;
	drawString(msg, labelx, 3, 0, width);
	gui.draw();
}
