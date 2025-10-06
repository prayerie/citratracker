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

#ifndef TABBOX_H
#define TABBOX_H

#include "gui.h"
#include "widget.h"

#include <vector>

#define TABBOX_ORIENTATION_TOP 0
#define TABBOX_ORIENTATION_LEFT 1

class TabBox: public Widget {
	public:
		TabBox(u16 _x, u16 _y, u16 _width, u16 _height, u8 orientation, u8 icon_size, bool _visible=true, u8 *fb_main=NULL, u8 *fb_sub=NULL); // bad
		
		void addTab(const u8 *icon, u8 tabidx);
	
		// Adds a widget and specifies which button it listens to
		// Touches on widget's area are redirected to the widget
		void registerWidget(Widget *w, u16 listeningButtons, u8 tabidx, u8 screen=SUB_SCREEN);
		
		// Event calls
		void penDown(u16 px, u16 py);
		void penUp(u16 px, u16 py);
		void penMove(u16 px, u16 py);
		void buttonPress(u16 buttons);
		
		// Callback registration
		void registerTabChangeCallback(void (*onTabChange_)(u8 tab));
		
		// Drawing request
		void pleaseDraw(void);
		
		void show(void);
		void hide(void);
		void occlude(void);
		void reveal(void);
		
		void setTheme(Theme *theme_, u32 bgcolour_);
		
	private:
		u8 findGuiIdx(u8 tabidx);
		void draw(void);	
		void updateVisibilities(void);
	
		u8 orientation;
		u8 icon_size;
		u8 currentgui;
		u8 *fb_main, *fb_sub; // these are so they can be passed to GUIs it creates. need to think of a better way around this though
		std::vector<u8> tab_idx_map;
		std::vector<const u8*> icons;
		std::vector<GUI> guis;
	
		void (*onTabChange)(u8 tab);
};

#endif
