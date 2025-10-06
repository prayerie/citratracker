#pragma once
#include <3ds/types.h>
#include "gfx/widget.h"

#include <vector>
#include <map>

#define MAIN_SCREEN	0
#define SUB_SCREEN	1

class GUI {
	public:
		GUI();

		// void setTheme(Theme *theme_, u16 bgcolour_);

		void registerWidget(Widget *w, u16 listeningButtons=0, u8 screen = SUB_SCREEN);

		void unregisterWidget(Widget *w);

		// Registers a widget that is in top of all other widgets and has input
		// priority, like a popup-window or something.
		void registerOverlayWidget(Widget *w, u16 listeningButtons, u8 screen = SUB_SCREEN);

		// Remove the overlay widget
		void unregisterOverlayWidget(u8 screen = SUB_SCREEN);

		// Event calls
		void penDown(u8 x, u8 y);
		void penUp(u8 x, u8 y); // Remove the coordinates here!
		void penMove(u8 x, u8 y);
		void buttonPress(u16 buttons);
		void buttonRelease(u16 buttons);

		// Draw requests
		void draw(void);
		void drawMainScreen(void);
		void drawSubScreen(void);

		// Screen switch
		void switchScreens(void);
		u8 getActiveScreen(void);

		// Show/Hide all elements
		void showAll(void);
		void hideAll(void);

		void occludeAll(void);
		void revealAll(void);

		inline std::vector<Widget*> getWidgets(u8 screen = SUB_SCREEN) {
			return screen == SUB_SCREEN ? widgets_sub : widgets_main;
		};

	private:
		std::vector<Widget*> widgets_main, widgets_sub;
		std::vector<Widget*> shortcuts;
		Widget *activeWidget;
		u8 activeScreen;
		Widget *overlayWidgetMain, *overlayWidgetSub;
		u16 overlayShortcuts;
		// Theme *theme;
		u32 bgColour;

		// Find the widget that got hit
		Widget *getWidgetAt(u8 x, u8 y);
		Widget *getWidgetForButtons(u16 buttons);
};