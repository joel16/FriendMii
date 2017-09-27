#include "clock.h"
#include "draw.h"
#include "power.h"
#include "screen.h"

void drawTopBar(void) // to make the main function look cleaner I guess
{
	u32 width = 0;
	
	screen_draw_rect(0, 0, 400, 20, RGBA8(255, 255, 255, 255));
	
	drawBatteryStatus(368, 0);
	
	digitalTime();
	
	width = (screen_get_string_width("0", 0.5f, 0.5f) + screen_get_string_width("/00", 0.5f, 0.5f) + screen_get_string_width(" ()", 0.5f, 0.5f) + 
				screen_get_string_width(getCurrentDay(1), 0.5f, 0.5f) + screen_get_string_width(getCurrentDay(1), 0.5f, 0.5f));
	
	screen_draw_stringf(316 - width, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "%d/%d (%s)", getDayOfWeek(), getMonthOfYear(), getCurrentDay(1));
	
	screen_draw_stringf(5, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "FriendMii v%d.%02d", VERSION_MAJOR, VERSION_MINOR);
}

void drawTopScreen(int state)
{
	switch (state)
	{
		case 0:
			screen_draw_rect(0, 20, 400, 220, RGBA8(234, 234, 234, 255));
			screen_draw_rect(46, 33, 306, 184, RGBA8(255, 255, 255, 255));
			screen_draw_rect(50, 37, 298, 137, RGBA8(255, 196, 142, 255));
			break;
		
		case 1:
			screen_draw_rect(0, 20, 400, 220, RGBA8(234, 234, 234, 255));
			screen_draw_rect(4, 24, 392, 210, RGBA8(255, 196, 142, 255));
	}
}

void drawBottomScreen(void)
{
	screen_draw_rect(0, 0, 320, 240, RGBA8(234, 234, 234, 255));
	screen_draw_rect(0, 0, 320, 26, RGBA8(204, 204, 204, 255));
	screen_draw_rect(34, 49, 252, 40, RGBA8(255, 255, 255, 255));
	screen_draw_rect(96, 104, 128, 80, RGBA8(255, 255, 255, 255));
	screen_draw_rect(98, 106, 124, 76, RGBA8(255, 194, 148, 255));
	screen_draw_rect(98, 106, 124, 38, RGBA8(255, 231, 213, 255));
	screen_draw_rect(98, 158, 124, 26, RGBA8(255, 255, 255, 255));
}