#include "clock.h"
#include "draw.h"
#include "power.h"
#include "screen.h"

void drawTopBar(void) // to make the main function look cleaner I guess
{
	u32 width = 0;
	
	screen_draw_rect(0, 0, 400, 20, RGBA8(164, 164, 164, 255));
	
	drawBatteryStatus(368, 0);
	
	digitalTime();
	
	width = (screen_get_string_width("0", 0.5f, 0.5f) + screen_get_string_width("/00", 0.5f, 0.5f) + screen_get_string_width(" ()", 0.5f, 0.5f) + 
				screen_get_string_width(getCurrentDay(1), 0.5f, 0.5f) + screen_get_string_width(getCurrentDay(1), 0.5f, 0.5f));
	
	screen_draw_stringf(310 - width, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "%d/%d (%s)", getDayOfWeek(), getMonthOfYear(), getCurrentDay(1));
	
	screen_draw_stringf(5, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "FriendMii v%d.%02d", VERSION_MAJOR, VERSION_MINOR);
}

void drawTopScreen(int state)
{
	switch (state)
	{
		case 0:
			screen_draw_rect(0, 20, 400, 220, RGBA8(45, 45, 45, 255));
			screen_draw_rect(46, 33, 306, 184, RGBA8(53, 119, 151, 255));
			screen_draw_rect(48, 35, 302, 180, RGBA8(80, 80, 80, 255));
			screen_draw_rect(48, 35, 302, 137, RGBA8(216, 216, 216, 255));
			
			// Game icon border
			screen_draw_rect(53, 62, 52, 52, RGBA8(53, 119, 151, 255));
			break;
		
		case 1:
			screen_draw_rect(0, 20, 400, 220, RGBA8(45, 45, 45, 255));
			screen_draw_rect(4, 24, 392, 210, RGBA8(80, 80, 80, 255));
			break;
			
		case 2:
			screen_draw_rect(0, 20, 400, 220, RGBA8(45, 45, 45, 255));
			screen_draw_rect(2, 22, 396, 216, RGBA8(53, 119, 151, 255));
			screen_draw_rect(4, 24, 392, 212, RGBA8(216, 216, 216, 255));
			break;
	}
}

void drawBottomScreen(void)
{
	screen_draw_rect(0, 0, 320, 240, RGBA8(45, 45, 45, 255));
	screen_draw_rect(0, 0, 320, 26, RGBA8(164, 164, 164, 255));
	screen_draw_rect(32, 47, 256, 44, RGBA8(53, 119, 151, 255));
	screen_draw_rect(34, 49, 252, 40, RGBA8(216, 216, 216, 255));
	screen_draw_rect(96, 104, 128, 82, RGBA8(53, 119, 151, 255));
	screen_draw_rect(98, 106, 124, 76, RGBA8(216, 216, 216, 255));
	screen_draw_rect(98, 158, 124, 26, RGBA8(80, 80, 80, 255));
}