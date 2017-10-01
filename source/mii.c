#include "mii.h"
#include "screen.h"

u32 MII_GetMiiColour(u16 colour)
{
	u32 ret = 0;
	
	switch(colour)
	{
		case 0:
			ret = RGBA8(210, 30, 20, 255);
			break;
			
		case 1:
			ret = RGBA8(255, 110, 25, 255);
			break;
			
		case 2:
			ret = RGBA8(255, 216, 32, 255);
			break;
			
		case 3:
			ret = RGBA8(120, 210, 32, 255);
			break;
			
		case 4:
			ret = RGBA8(0, 120, 48, 255);
			break;
			
		case 5:
			ret = RGBA8(32, 72, 152, 255);
			break;
			
		case 6:
			ret = RGBA8(60, 170, 222, 255);
			break;
			
		case 7:
			ret = RGBA8(245, 90, 125, 255);
			break;
			
		case 8:
			ret = RGBA8(115, 40, 173, 255);
			break;
			
		case 9:
			ret = RGBA8(54, 42, 18, 255);
			break;
			
		case 10:
			ret = RGBA8(224, 224, 224, 255);
			break;
			
		case 11:
			ret = RGBA8(24, 24, 20, 255);
			break;
	}
	
	return ret;
}