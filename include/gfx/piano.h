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

#ifndef PIANO_H
#define PIANO_H

#include <3ds/types.h>
#include "widget.h"

#define PIANO_WIDTH_TILES 28
#define PIANO_HEIGHT_TILES 5

class Piano: public Widget {
	public:
		Piano(u16 _x, u16 _y, u16 _width, u16 _height, u16 *_screen_base);
	
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u16 px, u16 py);
		void penUp(u16 px, u16 py);
		void penMove(u16 px, u16 py);
		
		// Callback registration
		void registerNoteCallback(void (*onNote_)(u16));
		void registerReleaseCallback(void (*onRelease_)(u16, bool));
		
		// Key label handling
		void showKeyLabels(void);
		void hideKeyLabels(void);
		void setKeyLabel(u16 key, char label);
		
	private:
		void (*onNote)(u16);
		void (*onRelease)(u16, bool);
		u16 *char_base, *map_base;
		
		void draw(void);
        void drawDirect(void);
		void drawInit(void);
		void setKeyPal(u16 note);
		u16 isHalfTone(u16 note);
		void resetPals(void);
		
		void drawKeyLabel(u16 key, bool visible=true);
		void eraseKeyLabel(u16 key);
		
		char key_labels[24];
		bool key_labels_visible;
		bool first_draw;
		u16 curr_note;
};

#endif
