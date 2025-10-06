#pragma once

#include "widget.h"

class Button: public Widget {
	public:
		Button(u16 _x, u16 _y, u16 _width, u16 _height, bool _visible=true);
	
		~Button();
		
		// Callback registration
		void registerPushCallback(void (*onPush_)(void));
		
		// Drawing request
		void pleaseDraw(void);
		
		// Event calls
		void penDown(u16 x, u16 y);
		void penUp(u16 x, u16 y);
		void penMove(u16 x, u16 y);
		void buttonPress(u16 button);
		// todo - widgets should probably not even be given the framebuffer ptr
        // instead they should probably inherit fb from Widget
        // but it MUST be updated each frame (perhaps force it to go stale)
		inline bool isPenDown(void) const { return penIsDown; }
		void setCaption(const char *caption);
		
	private:
		void (*onPush)(void);
		bool penIsDown;
		
		void draw(u8 down);
		char *caption;
};