#include <3ds.h>
#include <time.h>

#include "C2D_helper.h"
#include "common.h"
#include "status_bar.h"
#include "textures.h"

static char *Clock_GetCurrentTime(bool _12hour)
{
	static char buffer[10];

	time_t unix_time = time(0);
	struct tm* time_struct = gmtime((const time_t*)&unix_time);
	int hours = time_struct->tm_hour;
	int minutes = time_struct->tm_min;
	int amOrPm = 0;

	if (_12hour)
	{
		if (hours < 12)
			amOrPm = 1;
		if (hours == 0)
			hours = 12;
		else if (hours > 12)
			hours = hours - 12;

		if ((hours >= 1) && (hours < 10))
			snprintf(buffer, 10, "%2i:%02i %s", hours, minutes, amOrPm ? "AM" : "PM");
		else
			snprintf(buffer, 10, "%2i:%02i %s", hours, minutes, amOrPm ? "AM" : "PM");
	}

	return buffer;
}

static int Clock_GetCurrentDayNum(void)
{
	time_t unixTime = time(NULL);
	struct tm *timeStruct = gmtime((const time_t *)&unixTime);
	
	return timeStruct->tm_mday;
}

static int Clock_GetCurrentMonth(void)
{	
	time_t unixTime = time(NULL);
	struct tm *timeStruct = gmtime((const time_t *)&unixTime);
	
	return timeStruct->tm_mon;;
}

static char *Clock_GetCurrentDay(int type)
{
	static const char days[7][16] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	
	time_t unixTime = time(NULL);
	struct tm *timeStruct = gmtime((const time_t *)&unixTime);
	
	static char buffer[16];
	sprintf(buffer, "%s", days[timeStruct->tm_wday]);
    
	if (type == 1)
		buffer[3] = 0;
	
	return buffer;
}

static void Battery_GetStatus(void)
{
	u8 batteryLevel = 0, isCharging = 0;
	
	if (R_SUCCEEDED(PTMU_GetBatteryLevel(&batteryLevel)))
	{
		switch (batteryLevel)
		{
			case 0:
				Draw_Image(battery_1, (400 - (battery_1.subtex->width)), 0);
				break;
			case 1:
				Draw_Image(battery_1, (400 - (battery_1.subtex->width)), 0);
				break;
			case 2:
				Draw_Image(battery_2, (400 - (battery_2.subtex->width)), 0);
				break;
			case 3:
				Draw_Image(battery_2, (400 - (battery_2.subtex->width)), 0);
				break;
			case 4:
				Draw_Image(battery_3, (400 - (battery_3.subtex->width)), 0);
				break;
			case 5:
				Draw_Image(battery_4, (400 - (battery_4.subtex->width)), 0);
				break;
		}
	}
	
	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&isCharging)))
	{
		if (isCharging)
			Draw_Image(battery_charge, (400 - (battery_charge.subtex->width)), 0);
	}
}

void StatusBar_DisplayData(void)
{
	float width = 0, height = 0;
	Draw_GetTextSize(0.48f, &width, &height, Clock_GetCurrentTime(true));

	Battery_GetStatus();

	Draw_Text(((400 - (battery_charge.subtex->width)) - 5) - width, (20 - height) / 2, 0.48f, BLACK, Clock_GetCurrentTime(true));

	width = (Draw_GetTextWidth(0.48f, "0") + Draw_GetTextWidth(0.48f, "/00") + Draw_GetTextWidth(0.48f, " ()") + 
				Draw_GetTextWidth(0.48f, Clock_GetCurrentDay(1)) + Draw_GetTextWidth(0.48f, Clock_GetCurrentDay(1)));
	
	Draw_Textf(316 - width, (20 - height) / 2, 0.48f, BLACK, "%d/%d (%s)", Clock_GetCurrentDayNum(), Clock_GetCurrentMonth(), Clock_GetCurrentDay(1));

	Draw_Textf(5, (20 - Draw_GetTextHeight(0.5f, "FriendMii vX.XX")) / 2, 0.5f, BLACK, "FriendMii v%d.%02d", VERSION_MAJOR, VERSION_MINOR);
}