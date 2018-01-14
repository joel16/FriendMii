#include <stdio.h>
#include <time.h>

#include "clock.h"

char * Clock_GetCurrentTime(bool _12hour)
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

int Clock_GetDayOfWeek(void)
{
	time_t unixTime = time(NULL);
	struct tm * timeStruct = gmtime((const time_t *)&unixTime);
	
	return timeStruct->tm_mday;
}

int Clock_GetMonthOfYear(void)
{	
	time_t unixTime = time(NULL);
	struct tm * timeStruct = gmtime((const time_t *)&unixTime);
	
	return timeStruct->tm_mon;;
}

char * Clock_GetCurrentDay(int type)
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
