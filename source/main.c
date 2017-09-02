#include <stdio.h>

#include "power.h"
#include "clock.h"
#include "frd.h"
#include "friend.h"
#include "fs.h"
#include "screen.h"
#include "screenshot.h"
#include "utils.h"

static void drawTopBar(void) // to make the main function look cleaner I guess
{
	u32 width = 0;
	
	screen_draw_rect(0, 0, 400, 20, RGBA8(255, 255, 255, 255));
	
	drawBatteryStatus(368, 0);
	
	digitalTime();
	
	width = (screen_get_string_width("0", 0.5f, 0.5f) + screen_get_string_width("/00", 0.5f, 0.5f) + screen_get_string_width(" ()", 0.5f, 0.5f) + 
				screen_get_string_width(getCurrentDay(1), 0.5f, 0.5f) + screen_get_string_width(getCurrentDay(1), 0.5f, 0.5f));
	
	screen_draw_stringf(316 - width, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "%d/%d (%s)", getDayOfWeek(), getMonthOfYear(), getCurrentDay(1));
	
	screen_draw_stringf(5, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "FriendMii v%d.%02d", VERSION_MAJOR, VERSION_MINOR);
}

static void initServices(void)
{
	gfxInitDefault();
	romfsInit();
	screen_init();
	fsInit();
	sdmcInit();
	openArchive(&fsArchive, ARCHIVE_SDMC);
	hidInit();
	frdInit();
	aptInit();
	cfguInit();
	amInit();
	ptmuInit();
	
	if (isN3DS())
		osSetSpeedupEnable(true);
	
	screen_load_texture_png(TEXTURE_BATTERY_1, "romfs:/battery/battery_1.png", true);
	screen_load_texture_png(TEXTURE_BATTERY_2, "romfs:/battery/battery_2.png", true);
	screen_load_texture_png(TEXTURE_BATTERY_3, "romfs:/battery/battery_3.png", true);
	screen_load_texture_png(TEXTURE_BATTERY_4, "romfs:/battery/battery_4.png", true);
	screen_load_texture_png(TEXTURE_BATTERY_CHARGE, "romfs:/battery/battery_charge.png", true);
}

static void termServices(void)
{
	screen_unload_texture(TEXTURE_BATTERY_CHARGE);
	screen_unload_texture(TEXTURE_BATTERY_4);
	screen_unload_texture(TEXTURE_BATTERY_3);
	screen_unload_texture(TEXTURE_BATTERY_2);
	screen_unload_texture(TEXTURE_BATTERY_1);
	
	if (isN3DS())
		osSetSpeedupEnable(0);
	
	ptmuExit();
	amExit();
	cfguExit();
	aptExit();
	frdExit();
	hidExit();
	closeArchive(fsArchive);
	sdmcExit();
	fsExit();
	screen_exit();
	romfsExit();
	gfxExit();
}

int main(int argc, char **argv) 
{
	initServices();
	
	screen_clear(GFX_TOP, RGBA8(255, 255, 255, 255));
	screen_clear(GFX_BOTTOM, RGBA8(255, 255, 255, 255));
	
	Friend_GetGameIcon(Friend_GetFavouriteGame());
	
	while (aptMainLoop()) 
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		screen_begin_frame();
		
		screen_select(GFX_TOP);
		
		drawTopBar();
		
		screen_draw_rect(0, 20, 400, 220, RGBA8(234, 234, 234, 255));
		screen_draw_rect(46, 33, 306, 184, RGBA8(255, 255, 255, 255));
		screen_draw_rect(50, 37, 298, 137, RGBA8(255, 196, 142, 255));
		screen_draw_rect(24, 74, 212, 76, RGBA8(255, 255, 255, 255));
		
		screen_draw_texture(TEXTURE_GAME_ICON, 28, 98);
		
		screen_draw_string(348 - screen_get_string_width(Friend_GetScreenName(), 0.7f, 0.7f), 176, 0.7f, 0.7f, RGBA8(126, 52, 34, 255), Friend_GetScreenName());
		screen_draw_stringf(348 - (screen_get_string_width("Friend Code: ", 0.5f, 0.5f) + screen_get_string_width("0000 - 0000 - 0000", 0.5f, 0.5f)), 
							200, 0.5f, 0.5f, RGBA8(140, 138, 138, 255), "Friend Code: %s", Friend_GetFriendKey());
		
		screen_draw_string(28 + ((188 - screen_get_string_width("Favourite Title:", 0.6f, 0.6f)) / 2), 78, 0.6f, 0.6f, RGBA8(82, 82, 82, 255), "Favourite Title:");
		screen_draw_string(84, 105, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetGameTitle(Friend_GetFavouriteGame()));
		screen_draw_string(84, 125, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetPlatform(Friend_GetFavouriteGame()));
		
		if (!(Friend_GetHideStatus()))
			screen_draw_string(((400 - screen_get_string_width("Hide online status", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(82, 60, 60, 255), "Hide online status");
		else if (!(Friend_GetHideTitleStatus()))
			screen_draw_string(((400 - screen_get_string_width("Hide titles being played", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(82, 60, 60, 255), "Hide titles being played");
		
		screen_select(GFX_BOTTOM);
		
		screen_draw_rect(0, 0, 320, 240, RGBA8(234, 234, 234, 255));
		screen_draw_rect(34, 49, 252, 40, RGBA8(255, 255, 255, 255));
		screen_draw_rect(96, 104, 128, 80, RGBA8(255, 255, 255, 255));
		screen_draw_rect(98, 106, 124, 76, RGBA8(255, 194, 148, 255));
		screen_draw_rect(98, 106, 124, 38, RGBA8(255, 231, 213, 255));
		screen_draw_rect(98, 158, 124, 26, RGBA8(255, 255, 255, 255));
		
		screen_draw_string(34 + ((252 - screen_get_string_width(Friend_GetMyComment(), 0.5f, 0.5f)) / 2), 64, 0.5f, 0.5f, RGBA8(126, 52, 34, 255), Friend_GetMyComment());
		
		screen_draw_stringf(100, 106, 0.5f, 0.5f, RGBA8(241, 164, 34, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
		screen_draw_string(220 - screen_get_string_width(Friend_GetScreenName(), 0.5f, 0.5f), 164, 0.5f, 0.5f, RGBA8(126, 52, 34, 255), Friend_GetScreenName());
		
		screen_end_frame();
		
		if (kDown & KEY_START)
			break;
		
		if ((kHeld & KEY_L) && (kHeld & KEY_R))
			captureScreenshot();
	}

	termServices();
	return 0;
}