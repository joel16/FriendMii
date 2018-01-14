#include "draw.h"
#include "power.h"
#include "pp2d.h"
#include "status_bar.h"

void drawTopBar(void) // to make the main function look cleaner I guess
{
	u32 width = 0;
	
	pp2d_draw_rectangle(0, 0, 400, 20, RGBA8(164, 164, 164, 255));
	
	StatusBar_DisplayBar();
	
	pp2d_draw_textf(5, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "FriendMii v%d.%02d", VERSION_MAJOR, VERSION_MINOR);
}

void drawTopScreen(int state)
{
	switch (state)
	{
		case 0:
			pp2d_draw_rectangle(0, 20, 400, 220, RGBA8(45, 45, 45, 255));
			pp2d_draw_rectangle(46, 33, 306, 184, RGBA8(53, 119, 151, 255));
			pp2d_draw_rectangle(48, 35, 302, 180, RGBA8(80, 80, 80, 255));
			pp2d_draw_rectangle(48, 35, 302, 137, RGBA8(216, 216, 216, 255));
			
			// Game icon border
			pp2d_draw_rectangle(53, 62, 52, 52, RGBA8(53, 119, 151, 255));
			break;
		
		case 1:
			pp2d_draw_rectangle(0, 20, 400, 220, RGBA8(45, 45, 45, 255));
			pp2d_draw_rectangle(4, 24, 392, 210, RGBA8(80, 80, 80, 255));
			break;
			
		case 2:
			pp2d_draw_rectangle(0, 20, 400, 220, RGBA8(45, 45, 45, 255));
			pp2d_draw_rectangle(2, 22, 396, 216, RGBA8(53, 119, 151, 255));
			pp2d_draw_rectangle(4, 24, 392, 212, RGBA8(216, 216, 216, 255));
			break;
	}
}

void drawBottomScreen(void)
{
	pp2d_draw_rectangle(0, 0, 320, 240, RGBA8(45, 45, 45, 255));
	pp2d_draw_rectangle(0, 0, 320, 26, RGBA8(164, 164, 164, 255));
	pp2d_draw_rectangle(32, 47, 256, 44, RGBA8(53, 119, 151, 255));
	pp2d_draw_rectangle(34, 49, 252, 40, RGBA8(216, 216, 216, 255));
	pp2d_draw_rectangle(96, 104, 128, 82, RGBA8(53, 119, 151, 255));
	pp2d_draw_rectangle(98, 106, 124, 76, RGBA8(216, 216, 216, 255));
	pp2d_draw_rectangle(98, 158, 124, 26, RGBA8(80, 80, 80, 255));
}