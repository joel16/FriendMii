#include "utils.h"

bool Utils_IsN3DS(void)
{
	bool isNew3DS = false;
	
	if (R_SUCCEEDED(APT_CheckNew3DS(&isNew3DS)))
		return isNew3DS;
	
	return false;
}

void u16_to_u8(char * buf, const u16 * input, size_t bufsize)
{
	ssize_t units = utf16_to_utf8((u8 *)buf, input, bufsize);
	
	if (units < 0) 
		units = 0;
	
	buf[units] = 0;
}

u16 touchGetX(void)
{
	touchPosition pos;
	hidTouchRead(&pos);
	
	return pos.px;
}

u16 touchGetY(void)
{
	touchPosition pos;
	hidTouchRead(&pos);
	
	return pos.py;
}