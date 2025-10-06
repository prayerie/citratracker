#include "gfx/gui.h"

GUI::GUI()
    : activeWidget(0), activeScreen(SUB_SCREEN), overlayWidgetMain(0),
      overlayWidgetSub(0), overlayShortcuts(0) {
    for (u8 i = 0; i < 14; ++i) {   
        shortcuts.push_back(0);
    }
}

