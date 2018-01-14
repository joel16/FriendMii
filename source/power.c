#include <3ds.h>

#include "power.h"
#include "pp2d.h"

void Battery_DrawStatus(int x, int y)
{
	u8 batteryLevel = 0, isCharging = 0;
	
	if (R_SUCCEEDED(PTMU_GetBatteryLevel(&batteryLevel)))
	{
		switch (batteryLevel)
		{
			case 0:
				pp2d_draw_texture(TEXTURE_BATTERY_1, x, y);
				break;
			case 1:
				pp2d_draw_texture(TEXTURE_BATTERY_1, x, y);
				break;
			case 2:
				pp2d_draw_texture(TEXTURE_BATTERY_2, x, y);
				break;
			case 3:
				pp2d_draw_texture(TEXTURE_BATTERY_2, x, y);
				break;
			case 4:
				pp2d_draw_texture(TEXTURE_BATTERY_3, x, y);
				break;
			case 5:
				pp2d_draw_texture(TEXTURE_BATTERY_4, x, y);
				break;
		}
	}
	
	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&isCharging)))
	{
		if (isCharging)
			pp2d_draw_texture(TEXTURE_BATTERY_CHARGE, x, y);
	}
}