#include <stdio.h>
#include <string.h>

#include <3ds.h>

bool Utils_IsN3DS(void) {
	bool isNew3DS = false;

	if (R_SUCCEEDED(APT_CheckNew3DS(&isNew3DS)))
		return isNew3DS;

	return false;
}

void Utils_U16_To_U8(u8 *buf, const u16 *input, size_t bufsize) {
	ssize_t units = utf16_to_utf8(buf, input, bufsize);

	if (units < 0)
		units = 0;
	
	buf[units] = 0;
}

void Utils_U8_To_U16(u16 *buf, const u8 *input, size_t bufsize) {
	ssize_t units = utf8_to_utf16(buf, input, bufsize);

	if (units < 0)
		units = 0;

	buf[units] = 0;
}

void Utils_SetMax(int *set, int value, int max) {
	if (*set > max)
		*set = value;
}

void Utils_SetMin(int *set, int value, int min) {
	if (*set < min)
		*set = value;
}
