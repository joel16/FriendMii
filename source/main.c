#include <stdio.h>

#include "draw.h"
#include "frd.h"
#include "friend.h"
#include "fs.h"
#include "power.h"
#include "screen.h"
#include "screenshot.h"
#include "utils.h"

#define STATE_FRIENDCARD 0
#define STATE_FRIENDLIST 1

#define selector_yDistance 35

static int defaultState;

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
	screen_load_texture_png(TEXTURE_NAVIGATION_DRAWER, "romfs:/drawer.png", true);
}

static void termServices(void)
{
	screen_unload_texture(TEXTURE_NAVIGATION_DRAWER);
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
	
	size_t friendCount = 0;
	FriendKey friendKey[FRIEND_LIST_SIZE];
	u64 friendCodes[FRIEND_LIST_SIZE];
	FRD_GetFriendKeyList(friendKey, &friendCount, 0, FRIEND_LIST_SIZE);
	
	for (size_t i = 0x0; i < friendCount && i < 0x64; i++) 
		FRD_PrincipalIdToFriendCode(friendKey[i].principalId, &friendCodes[i]);
	
	touchPosition touch;
	
	int selection = 1;
	int selector_y = -11; 
	int selector_image_y = 0;
	
	while (aptMainLoop()) 
	{
		hidScanInput();
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		selector_image_y = selector_y + (selector_yDistance * selection);
		
		screen_begin_frame();
		
		screen_select(GFX_TOP);
		
		drawTopBar();
		
		switch(defaultState)
		{
			case STATE_FRIENDCARD:
				drawTopScreen(STATE_FRIENDCARD);
				screen_draw_texture(TEXTURE_GAME_ICON, 55, 64);
				screen_draw_string(348 - screen_get_string_width(Friend_GetScreenName(), 0.7f, 0.7f), 176, 0.7f, 0.7f, RGBA8(126, 52, 34, 255), Friend_GetScreenName());
				screen_draw_stringf(348 - (screen_get_string_width("Friend Code: ", 0.5f, 0.5f) + screen_get_string_width("0000 - 0000 - 0000", 0.5f, 0.5f)), 
							200, 0.5f, 0.5f, RGBA8(140, 138, 138, 255), "Friend Code: %s", Friend_GetFriendKey());
		
				screen_draw_string(55, 42, 0.6f, 0.6f, RGBA8(82, 82, 82, 255), "Favourite Title:");
				screen_draw_string(112, 70, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetGameTitle(Friend_GetFavouriteGame()));
				screen_draw_string(112, 90, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetPlatform(Friend_GetFavouriteGame()));
		
				if (!(Friend_GetHideStatus()))
					screen_draw_string(((400 - screen_get_string_width("Hide online status", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(82, 60, 60, 255), "Hide online status");
				else if (!(Friend_GetHideTitleStatus()))
					screen_draw_string(((400 - screen_get_string_width("Hide titles being played", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(82, 60, 60, 255), "Hide titles being played");
				break;
				
			case STATE_FRIENDLIST:
				drawTopScreen(STATE_FRIENDLIST);
				screen_draw_rect(4, selector_image_y, 392, 35, RGBA8(255, 255, 255, 255));
				for (size_t i = 0x0; i < friendCount; i++)
					screen_draw_stringf(10, 30 + (i * 35), 0.5f, 0.5f, RGBA8(82, 82, 82, 255), "Friend #%lu: %s %04llu-%04llu-%04llu", i, friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000);
				
				if (kDown & KEY_DDOWN)
					selection++;
				else if (kDown & KEY_DUP)
					selection--;
				
				if (kHeld & KEY_CPAD_DOWN)
				{
					svcSleepThread(50000000);
					selection++;
				}
				else if (kHeld & KEY_CPAD_UP)
				{
					svcSleepThread(50000000);
					selection--;
				}
				
				if (selection > friendCount) 
					selection = 1;
				if (selection < 1) 
					selection = friendCount;
				
				break;
		}
		
		screen_select(GFX_BOTTOM);
		
		drawBottomScreen();
		
		screen_draw_texture(TEXTURE_NAVIGATION_DRAWER, 0, 0);
		
		screen_draw_string(34 + ((252 - screen_get_string_width(Friend_GetMyComment(), 0.5f, 0.5f)) / 2), 64, 0.5f, 0.5f, RGBA8(126, 52, 34, 255), Friend_GetMyComment());
		
		screen_draw_stringf(100, 106, 0.5f, 0.5f, RGBA8(241, 164, 34, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
		screen_draw_string(220 - screen_get_string_width(Friend_GetScreenName(), 0.5f, 0.5f), 164, 0.5f, 0.5f, RGBA8(126, 52, 34, 255), Friend_GetScreenName());
		
		screen_end_frame();
		
		if (kDown & KEY_START)
			break;
		
		if ((kDown & KEY_TOUCH) && (touchInRect(0, 26, 0, 26)))
		{
			if (defaultState == 0)
				defaultState = STATE_FRIENDLIST;
			else if (defaultState == 1)
				defaultState = STATE_FRIENDCARD;
		}
		
		if ((kHeld & KEY_L) && (kHeld & KEY_R))
			captureScreenshot();
	}

	termServices();
	return 0;
}