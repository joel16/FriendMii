#include <stdio.h>

#include "draw.h"
#include "frd.h"
#include "friend.h"
#include "fs.h"
#include "mii.h"
#include "power.h"
#include "screen.h"
#include "screenshot.h"
#include "utils.h"

#define STATE_FRIENDCARD 0
#define STATE_FRIENDLIST 1
#define STATE_MII        2

#define selector_yDistance 35
#define LIST_PER_PAGE 3

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
	screen_load_texture_png(TEXTURE_FRIENDS_HOME, "romfs:/ic_material_light_home.png", true);
	screen_load_texture_png(TEXTURE_FRIENDS_LIST, "romfs:/ic_material_light_filesystem.png", true);
	screen_load_texture_png(TEXTURE_FRIENDS_MII, "romfs:/ic_fso_type_contact.png", true);
}

static void termServices(void)
{
	screen_unload_texture(TEXTURE_FRIENDS_MII);
	screen_unload_texture(TEXTURE_FRIENDS_LIST);
	screen_unload_texture(TEXTURE_FRIENDS_HOME);
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
	FRD_GetFriendKeyList(friendKey, &friendCount, 0, FRIEND_LIST_SIZE);
	
	MiiStoreData friendMii[FRIEND_LIST_SIZE];
    char friendNames[FRIEND_LIST_SIZE * 0xB];
	char friendAuthor[FRIEND_LIST_SIZE * 0xB];
	FRD_GetFriendMii(friendMii, friendKey, FRIEND_LIST_SIZE); 
	
	bool isValid[FRIEND_LIST_SIZE];
	
	u64 friendCodes[FRIEND_LIST_SIZE];
	for (size_t i = 0x0; i < friendCount; i++) 
	{
		FRD_PrincipalIdToFriendCode(friendKey[i].principalId, &friendCodes[i]);
		FRD_IsValidFriendCode(friendCodes[i], &isValid[i]);
		u16_to_u8(&friendNames[i * 0xB], friendMii[i].mii_name, 0xB);
		friendNames[i * 0xB + 10] = 0;
		u16_to_u8(&friendAuthor[i * 0xB], friendMii[i].mii_name, 0xB);
		friendAuthor[i * 0xB + 10] = 0;
	}
	
	static char mii_name[0x14], mii_author[0x14];
	MiiStoreData miiData;
	FRD_GetMyMii(&miiData);
	
	u16_to_u8(mii_name, miiData.mii_name, 0x14);
	u16_to_u8(mii_author, miiData.author, 0x14);
	
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
				screen_draw_string(348 - screen_get_string_width(Friend_GetScreenName(), 0.7f, 0.7f), 176, 0.7f, 0.7f, RGBA8(170, 170, 170, 255), Friend_GetScreenName());
				screen_draw_stringf(348 - (screen_get_string_width("Friend Code: ", 0.5f, 0.5f) + screen_get_string_width("0000 - 0000 - 0000", 0.5f, 0.5f)), 
							200, 0.5f, 0.5f, RGBA8(148, 148, 148, 255), "Friend Code: %s", Friend_GetFriendKey());
		
				screen_draw_string(53, 42, 0.6f, 0.6f, RGBA8(82, 82, 82, 255), "Favourite Title:");
				screen_draw_string(112, 70, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetGameTitle(Friend_GetFavouriteGame()));
				screen_draw_string(112, 90, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetPlatform(Friend_GetFavouriteGame()));
		
				if (!(Friend_GetHideStatus()))
					screen_draw_string(((400 - screen_get_string_width("Hide online status", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(53, 119, 151, 255), "Hide online status");
				else if (!(Friend_GetHideTitleStatus()))
					screen_draw_string(((400 - screen_get_string_width("Hide titles being played", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(53, 119, 151, 255), "Hide titles being played");
				break;
				
			case STATE_FRIENDLIST:
				drawTopScreen(STATE_FRIENDLIST);
				screen_draw_rect(4, selector_image_y, 392, 35, RGBA8(255, 255, 255, 255));
				
				for (size_t i = 0x0; i < friendCount; i++)
				{
					screen_draw_stringf(10, 30 + (i * 35), 0.5f, 0.5f, RGBA8(255, 255, 255, 255), "%s : %04llu-%04llu-%04llu %02X:%02X:%02X:%02X:%02X:%02X", &friendNames[i * 0xB], friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000, friendMii[i].mac[0], friendMii[i].mac[1], friendMii[i].mac[2], friendMii[i].mac[3], friendMii[i].mac[4], friendMii[i].mac[5] /*isValid[i]? "Valid" : "Invalid"*/);
					
					if (i == (selection - 1))
						screen_draw_stringf(10, 30 + (i * 35), 0.5f, 0.5f, RGBA8(82, 82, 82, 255), "%s : %04llu-%04llu-%04llu %02X:%02X:%02X:%02X:%02X:%02X", &friendNames[i * 0xB], friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000, friendMii[i].mac[0], friendMii[i].mac[1], friendMii[i].mac[2], friendMii[i].mac[3], friendMii[i].mac[4], friendMii[i].mac[5] /*isValid[i]? "Valid" : "Invalid"*/);
				}
				
				if (kDown & KEY_DDOWN)
				{
					if (selection < (friendCount))
						selection++;
					else 
						selection = 1;
				}
				else if (kDown & KEY_DUP)
				{
					if (selection > 1)
						selection--;
					else 
						selection = friendCount;
				}
				
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
				break;
				
			case STATE_MII:
				drawTopScreen(STATE_MII);
				/*screen_draw_rect(10 + screen_get_string_width("Mii color: ", 0.6f, 0.6f), 91, 16, 16, RGBA8(53, 119, 151, 255));
				screen_draw_rect(10 + screen_get_string_width("Mii color: ", 0.6f, 0.6f) + 1, 92, 14, 14, MII_GetMiiColour(miiData.color));
				screen_draw_stringf(10, 30, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii name: %s", mii_name);
				screen_draw_stringf(10, 50, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii mac address: %02X:%02X:%02X:%02X:%02X:%02X", miiData.mac[0], miiData.mac[1], miiData.mac[2], miiData.mac[3], miiData.mac[4], miiData.mac[5]);
				screen_draw_stringf(10, 70, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii gender: %s", miiData.gender? "Female" : "Male");
				screen_draw_string(10, 90, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii color: ");
				screen_draw_stringf(10, 110, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii Birthday: %d/%d", miiData.bday_month, miiData.bday_day);
				screen_draw_stringf(10, 130, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii favorite: %s", miiData.favorite? "Yes" : "No");
				screen_draw_stringf(10, 150, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii copying: %s", miiData.copyable? "Enabled" : "Disabled");
				screen_draw_stringf(10, 170, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii sharing: %s", miiData.disable_sharing? "Disabled" : "Enabled");*/
				break;
		}
		
		screen_select(GFX_BOTTOM);
		
		drawBottomScreen();
		
		screen_draw_rect((34 * defaultState), 0, 34, 26, RGBA8(53, 119, 151, 255));
		
		screen_draw_texture(TEXTURE_FRIENDS_HOME, 4, 0);
		screen_draw_texture(TEXTURE_FRIENDS_LIST, 38, 0);
		screen_draw_texture(TEXTURE_FRIENDS_MII, 72, 0);
		
		screen_draw_string(34 + ((252 - screen_get_string_width(Friend_GetMyComment(), 0.5f, 0.5f)) / 2), 64, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetMyComment());
		
		screen_draw_stringf(100, 106, 0.5f, 0.5f, RGBA8(53, 119, 151, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
		screen_draw_string(220 - screen_get_string_width(Friend_GetScreenName(), 0.5f, 0.5f), 164, 0.5f, 0.5f, RGBA8(148, 148, 148, 255), Friend_GetScreenName());
		
		//screen_draw_stringf(10, 200, 0.5f, 0.5f, RGBA8(53, 119, 151, 255), "%02X:%02X:%02X:%02X:%02X:%02X", miiData.mac[0], miiData.mac[1], miiData.mac[2], miiData.mac[3], miiData.mac[4], miiData.mac[5]);
		
		screen_end_frame();
		
		if (kDown & KEY_START)
			break;
		
		if ((kDown & KEY_TOUCH))
		{
			if (touchInRect(4, 34, 0, 26))
				defaultState = STATE_FRIENDCARD;
			else if (touchInRect(38, 68, 0, 26))
				defaultState = STATE_FRIENDLIST;
			else if (touchInRect(72, 102, 0, 26))
				defaultState = STATE_MII;
		}
		
		if ((kHeld & KEY_L) && (kHeld & KEY_R))
			captureScreenshot();
	}

	termServices();
	return 0;
}