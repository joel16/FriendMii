#include <3ds.h>

#include <time.h>

#include "clock.h"
#include "screen.h"

void digitalTime(void)
{
	float width = 0;
	
	time_t unix_time = time(0);
	struct tm* time_struct = gmtime((const time_t*)&unix_time);
	int hours = time_struct->tm_hour;
	int minutes = time_struct->tm_min;
	int amOrPm = 0;
	
	if (hours < 12)
		amOrPm = 1;
	if (hours == 0)
		hours = 12;
	else if (hours > 12)
		hours = hours - 12;

	if ((hours >= 1) && (hours < 10))  
	{
		width = screen_get_string_width("0:00 XM", 0.5f, 0.5f);
		screen_draw_stringf((360 - width), 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "%2i:%02i %s", hours, minutes, amOrPm ? "AM" : "PM");
	}
	else
	{
		width = screen_get_string_width("00:00 XM", 0.5f, 0.5f);
		screen_draw_stringf((360 - width), 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "%2i:%02i %s", hours, minutes, amOrPm ? "AM" : "PM");
	}
}

int getDayOfWeek(void)
{
	time_t unixTime = time(NULL);
	struct tm * timeStruct = gmtime((const time_t *)&unixTime);
	
	return timeStruct->tm_mday;
}

int getMonthOfYear(void)
{	
	time_t unixTime = time(NULL);
	struct tm * timeStruct = gmtime((const time_t *)&unixTime);
	
	return timeStruct->tm_mon;;
}

char * getCurrentDay(int type)
{
	static const char days[7][16] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	
	time_t unixTime = time(NULL);
	struct tm * timeStruct = gmtime((const time_t *)&unixTime);
	
	static char buffer[16];
	sprintf(buffer, "%s", days[timeStruct->tm_wday]);
    
	if (type == 1)
		buffer[3] = 0;
	
	return buffer;
}