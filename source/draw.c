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