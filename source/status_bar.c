#include "clock.h"
#include "power.h"
#include "pp2d.h"
#include "status_bar.h"

void StatusBar_DisplayBar(void)
{
	float time_x = 395 - pp2d_get_text_width(Clock_GetCurrentTime(true), 0.5f, 0.5f);
	float battery_texture_x = time_x - pp2d_get_texture_width(TEXTURE_BATTERY_4) - 5;
	float date_x = battery_texture_x - ((pp2d_get_text_width("0", 0.5f, 0.5f) + pp2d_get_text_width("/00", 0.5f, 0.5f) + pp2d_get_text_width(" ()", 0.5f, 0.5f) + 
				pp2d_get_text_width(Clock_GetCurrentDay(1), 0.5f, 0.5f) + pp2d_get_text_width(Clock_GetCurrentDay(1), 0.5f, 0.5f)));

	pp2d_draw_text(time_x, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), Clock_GetCurrentTime(true));
	Battery_DrawStatus(battery_texture_x, 0);
	
	pp2d_draw_textf(date_x, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "%d/%d (%s)", Clock_GetDayOfWeek(), Clock_GetMonthOfYear(), Clock_GetCurrentDay(1));
}