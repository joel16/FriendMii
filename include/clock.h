#ifndef CLOCK_H
#define CLOCK_H

#include <3ds.h>

char * Clock_GetCurrentTime(bool _12hour);
int Clock_GetDayOfWeek(void);
int Clock_GetMonthOfYear(void);
char * Clock_GetCurrentDay(int type);

#endif