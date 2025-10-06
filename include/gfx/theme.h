#pragma once
#include <3ds/types.h>

class Theme
{
	public:
		Theme();
		
		u32 col_bg;
		u32 col_dark_bg;
		u32 col_medium_bg;
		u32 col_light_bg;
		u32 col_lighter_bg;
		u32 col_light_ctrl;
		u32 col_dark_ctrl;
		u32 col_light_ctrl_disabled;
		u32 col_dark_ctrl_disabled;
		u32 col_list_highlight1;
		u32 col_list_highlight2;
		u32 col_outline;
		u32 col_sepline;
		u32 col_icon;
		u32 col_text;
		u32 col_signal;
};