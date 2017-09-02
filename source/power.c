#include <3ds.h>

#include "power.h"
#include "screen.h"

void drawBatteryStatus(int x, int y)
{
	u8 batteryLevel = 0, isCharging = 0;
	
	if (R_SUCCEEDED(PTMU_GetBatteryLevel(&batteryLevel)))
	{
		switch (batteryLevel)
		{
			case 0:
				screen_draw_texture(TEXTURE_BATTERY_1, x, y);
				break;
			case 1:
				screen_draw_texture(TEXTURE_BATTERY_1, x, y);
				break;
			case 2:
				screen_draw_texture(TEXTURE_BATTERY_2, x, y);
				break;
			case 3:
				screen_draw_texture(TEXTURE_BATTERY_2, x, y);
				break;
			case 4:
				screen_draw_texture(TEXTURE_BATTERY_3, x, y);
				break;
			case 5:
				screen_draw_texture(TEXTURE_BATTERY_4, x, y);
				break;
		}
	}
	
	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&isCharging)))
	{
		if (isCharging)
			screen_draw_texture(TEXTURE_BATTERY_CHARGE, x, y);
	}
}