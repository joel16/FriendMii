#ifndef FRIENDMII_COMMON_H
#define FRIENDMII_COMMON_H

#include <setjmp.h>

#define wait(msec) svcSleepThread(10000000 * (s64)msec)

#define STATE_FRIENDCARD 0
#define STATE_FRIENDLIST 1
#define STATE_MII        2

int MENU_STATE;

jmp_buf exitJmp;

#endif