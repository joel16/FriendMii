#include <3ds.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "frd.h"
#include "friend.h"
#include "fs.h"
#include "keyboard.h"
#include "mii.h"
#include "power.h"
#include "pp2d.h"
#include "screenshot.h"
#include "status_bar.h"
#include "utils.h"

#define STATE_FRIENDCARD 0
#define STATE_FRIENDLIST 1
#define STATE_FRIENDADD  2

#define selector_yDistance 35
#define LIST_PER_PAGE 6

static int defaultState;
size_t TEXTURE_COLOUR[12];

static void Init_Services(void)
{
	FS_OpenArchive(&archive, ARCHIVE_SDMC);
	frdInit();
	cfguInit();
	amInit();
	ptmuInit();
	pp2d_init();
	romfsInit();
	
	if (Utils_IsN3DS())
		osSetSpeedupEnable(true);
	
	pp2d_load_texture_png(TEXTURE_BATTERY_1, "romfs:/battery/battery_1.png");
	pp2d_load_texture_png(TEXTURE_BATTERY_2, "romfs:/battery/battery_2.png");
	pp2d_load_texture_png(TEXTURE_BATTERY_3, "romfs:/battery/battery_3.png");
	pp2d_load_texture_png(TEXTURE_BATTERY_4, "romfs:/battery/battery_4.png");
	pp2d_load_texture_png(TEXTURE_BATTERY_CHARGE, "romfs:/battery/battery_charge.png");
	pp2d_load_texture_png(TEXTURE_FRIENDS_HOME, "romfs:/ic_material_light_home.png");
	pp2d_load_texture_png(TEXTURE_FRIENDS_LIST, "romfs:/ic_material_light_list.png");
	pp2d_load_texture_png(TEXTURE_FRIENDS_ADD, "romfs:/ic_material_light_add.png");

	pp2d_load_texture_png(TEXTURE_COLOUR_RED, "romfs:/colour/ic_colour_red.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_ORANGE, "romfs:/colour/ic_colour_orange.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_YELLOW, "romfs:/colour/ic_colour_yellow.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_LIME_GREEN, "romfs:/colour/ic_colour_lime_green.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_GREEN, "romfs:/colour/ic_colour_green.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_BLUE, "romfs:/colour/ic_colour_blue.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_LIGHT_BLUE, "romfs:/colour/ic_colour_light_blue.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_PINK, "romfs:/colour/ic_colour_pink.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_PURPLE, "romfs:/colour/ic_colour_purple.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_BROWN, "romfs:/colour/ic_colour_brown.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_WHITE, "romfs:/colour/ic_colour_white.png");
	pp2d_load_texture_png(TEXTURE_COLOUR_BLACK, "romfs:/colour/ic_colour_black.png");
}

static void Term_Services(void)
{
	pp2d_free_texture(TEXTURE_COLOUR_BLACK);
	pp2d_free_texture(TEXTURE_COLOUR_WHITE);
	pp2d_free_texture(TEXTURE_COLOUR_BROWN);
	pp2d_free_texture(TEXTURE_COLOUR_PURPLE);
	pp2d_free_texture(TEXTURE_COLOUR_PINK);
	pp2d_free_texture(TEXTURE_COLOUR_LIGHT_BLUE);
	pp2d_free_texture(TEXTURE_COLOUR_BLUE);
	pp2d_free_texture(TEXTURE_COLOUR_GREEN);
	pp2d_free_texture(TEXTURE_COLOUR_LIME_GREEN);
	pp2d_free_texture(TEXTURE_COLOUR_YELLOW);
	pp2d_free_texture(TEXTURE_COLOUR_ORANGE);
	pp2d_free_texture(TEXTURE_COLOUR_RED);

	pp2d_free_texture(TEXTURE_FRIENDS_ADD);
	pp2d_free_texture(TEXTURE_FRIENDS_LIST);
	pp2d_free_texture(TEXTURE_FRIENDS_HOME);
	pp2d_free_texture(TEXTURE_BATTERY_CHARGE);
	pp2d_free_texture(TEXTURE_BATTERY_4);
	pp2d_free_texture(TEXTURE_BATTERY_3);
	pp2d_free_texture(TEXTURE_BATTERY_2);
	pp2d_free_texture(TEXTURE_BATTERY_1);
	
	if (Utils_IsN3DS())
		osSetSpeedupEnable(0);
	
	romfsExit();
	pp2d_exit();
	ptmuExit();
	amExit();
	cfguExit();
	frdExit();
	FS_CloseArchive(archive);
}

int main(int argc, char **argv) 
{
	Init_Services();

	Handle event;
	svcCreateEvent(&event, RESET_ONESHOT);
	svcCreateEvent(&s_terminate, RESET_ONESHOT);
	Thread thread = threadCreate(FriendNotificationHandlerThread, NULL, 4096 * 2, 0x24, 0, true);
	
	Friend_GetGameIcon(Friend_GetFavouriteGame());
	
	size_t friendCount = 0;
	FriendKey friendKey[FRIEND_LIST_SIZE];
	
	FRD_GetFriendKeyList(friendKey, &friendCount, 0, FRIEND_LIST_SIZE);
	
	//Profile profileList[friendCount];
	//FRD_GetFriendProfile(profileList, friendKey, friendCount);

	//FriendPresence friendPresenceList[friendCount];
	//FRD_GetFriendPresence(friendPresenceList, friendKey, friendCount);
	
    char friendNames[friendCount * 0xB];
	char friendAuthor[friendCount * 0xB];
	MiiStoreData friendMii[friendCount];
	FRD_GetFriendMii(friendMii, friendKey, friendCount); 

	//FriendGameKey friendGameKey[friendCount];
	//FRD_GetFriendFavoriteGame(friendKey, friendGameKey, friendCount);

	wchar_t wfriendComment[FRIENDS_COMMENT_SIZE];
	char friendComment[friendCount][FRIENDS_COMMENT_SIZE];

	bool isValid[friendCount];
	bool isFromList[friendCount];
	
	u64 friendCodes[friendCount];
	for (size_t i = 0x0; i < friendCount; i++) 
	{
		FRD_PrincipalIdToFriendCode(friendKey[i].principalId, &friendCodes[i]);
		
		FRD_IsValidFriendCode(friendCodes[i], &isValid[i]);
		isFromList[i] = Friend_IsFromFriendList(&friendKey[i]);
		
		u16_to_u8(&friendNames[i * 0xB], friendMii[i].mii_name, 0xB);
		friendNames[i * 0xB + 0xA] = 0;
		
		u16_to_u8(&friendAuthor[i * 0xB], friendMii[i].mii_name, 0xB);
		friendAuthor[i * 0xB + 0xA] = 0;

		FRD_GetFriendComment(wfriendComment, &friendKey[i], i);
		size_t size = utf16_to_utf8((uint8_t*)friendComment[i], (u16 *)wfriendComment, FRIENDS_COMMENT_SIZE);
		friendComment[i][size] = '\0';
	}
	
	static char mii_name[0x14], mii_author[0x14];
	MiiStoreData miiData;
	FRD_GetMyMii(&miiData);
	
	u16_to_u8(mii_name, miiData.mii_name, 0x14);
	u16_to_u8(mii_author, miiData.author, 0x14);
	
	touchPosition touch;
	
	int selection = 0;

	while (aptMainLoop()) 
	{
		hidScanInput();
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
			pp2d_draw_textf(5, 3, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "FriendMii v%d.%02d", VERSION_MAJOR, VERSION_MINOR);

			switch(defaultState)
			{
				case STATE_FRIENDADD:
				case STATE_FRIENDCARD:
					drawTopScreen(STATE_FRIENDCARD);
					pp2d_draw_texture(TEXTURE_GAME_ICON, 55, 64);
					pp2d_draw_text(348 - pp2d_get_text_width(Friend_GetScreenName(), 0.7f, 0.7f), 176, 0.7f, 0.7f, RGBA8(241, 252, 255, 255), Friend_GetScreenName());
					pp2d_draw_textf(348 - (pp2d_get_text_width("Friend Code: ", 0.5f, 0.5f) + pp2d_get_text_width("0000 - 0000 - 0000", 0.5f, 0.5f)), 
						200, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "Friend Code: %s", Friend_GetFriendKey());
		
					pp2d_draw_text(53, 42, 0.6f, 0.6f, RGBA8(241, 252, 255, 255), "Favourite Title:");
					pp2d_draw_text(112, 70, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), Friend_GetGameTitle(Friend_GetFavouriteGame()));
					pp2d_draw_text(112, 90, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), Friend_GetPlatform(Friend_GetFavouriteGame()));
		
					if (!(Friend_GetHideStatus()))
						pp2d_draw_text(((400 - pp2d_get_text_width("Hide online status", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(241, 252, 255, 255), "Hide online status");
					else if (!(Friend_GetHideTitleStatus()))
						pp2d_draw_text(((400 - pp2d_get_text_width("Hide titles being played", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(241, 252, 255, 255), "Hide titles being played");
					break;
				
				case STATE_FRIENDLIST:
					drawTopScreen(STATE_FRIENDLIST);

					int printed = 0; // Print counter
				
					for (size_t i = 0x0; i < friendCount; i++)
					{
						if (printed == LIST_PER_PAGE)
							break;

						if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE))
						{
							if (i == selection)
							{
								pp2d_draw_rectangle(4, 22 + (selector_yDistance * printed), 392, 35, RGBA8(241, 252, 255, 255));
								pp2d_draw_rectangle(6, 24 + (selector_yDistance * printed), 388, 31, RGBA8(13, 54, 144, 255));
							}

							pp2d_draw_texture(Friend_GetFriendColour(friendMii[i].colour), 10, 25 + (selector_yDistance * printed));

							pp2d_draw_textf(50, 24 + (selector_yDistance * printed), 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "%s", &friendNames[i * 0xB]);
							pp2d_draw_textf(50, 38 + (selector_yDistance * printed), 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "%04llu-%04llu-%04llu", friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000);

							printed++; // Increase printed counter
						}
					}
				
					if (kDown & KEY_DDOWN)
					{
						if (selection < (friendCount - 1))
							selection++;
						else 
							selection = 0;
					}
					else if (kDown & KEY_DUP)
					{
						if (selection > 0)
							selection--;
						else 
							selection = friendCount - 1;
					}
				
					if (kHeld & KEY_CPAD_DOWN)
					{
						wait(5);

						if (selection < (friendCount - 1))
							selection++;
						else 
							selection = 0;
					}
					else if (kHeld & KEY_CPAD_UP)
					{
						wait(5);
						
						if (selection > 0)
							selection--;
						else 
							selection = friendCount - 1;
					}
					break;
			}
		StatusBar_DisplayBar();
		pp2d_end_draw();

		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
			switch(defaultState)
			{
				case STATE_FRIENDCARD:
					drawBottomScreen();

					pp2d_draw_text(34 + ((252 - pp2d_get_text_width(Friend_GetMyComment(), 0.6f, 0.6f)) / 2), 60, 0.6f, 0.6f, RGBA8(241, 252, 255, 255), Friend_GetMyComment());
		
					pp2d_draw_textf(100, 106, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
					pp2d_draw_text(220 - pp2d_get_text_width(Friend_GetScreenName(), 0.5f, 0.5f), 164, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), Friend_GetScreenName());
					break;

				case STATE_FRIENDLIST:
					drawBottomScreen();

					float width =  friendMii[selection].gender? pp2d_get_text_width("♀ ", 0.5f, 0.5f) : pp2d_get_text_width("♂ ", 0.5f, 0.5f);
					pp2d_draw_text(34 + ((252 - pp2d_get_text_width(friendComment[selection], 0.6f, 0.6f)) / 2), 60, 0.6f, 0.6f, RGBA8(241, 252, 255, 255), friendComment[selection]);
		
					//pp2d_draw_textf(100, 106, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
					pp2d_draw_textf(220 - (pp2d_get_text_width(&friendNames[selection * 0xB], 0.5f, 0.5f) + width), 164, 0.5f, 0.5f, RGBA8(241, 252, 255, 255), "%s %s", &friendNames[selection * 0xB], friendMii[selection].gender? "♀" : "♂");

					if (!isFromList[selection])
						pp2d_draw_text(((320 - pp2d_get_text_width("This person does not have you added", 0.55f, 0.55f)) / 2), 190, 0.55f, 0.55f, RGBA8(241, 252, 255, 255), "This person does not have you added");
					break;

				case STATE_FRIENDADD:
					pp2d_draw_rectangle(0, 0, 320, 240, RGBA8(13, 54, 144, 255));

					pp2d_draw_text(34 + ((252 - pp2d_get_text_width(Friend_GetMyComment(), 0.6f, 0.6f)) / 2), 60, 0.6f, 0.6f, RGBA8(241, 252, 255, 255), Friend_GetMyComment());
		
					break;
			}

			//for(int transp = 80; transp > 0; transp -= 3)
			//	pp2d_draw_rectangle((40 * defaultState), 0, 30, 30, RGBA8(241, 252, 255, transp));
			
			defaultState == STATE_FRIENDCARD? pp2d_draw_texture(TEXTURE_FRIENDS_HOME, 0, 0) : 
				pp2d_draw_texture_blend(TEXTURE_FRIENDS_HOME, 0, 0, RGBA8(0, 0, 0, 150));
			defaultState == STATE_FRIENDLIST? pp2d_draw_texture(TEXTURE_FRIENDS_LIST, 40, 0) : 
				pp2d_draw_texture_blend(TEXTURE_FRIENDS_LIST, 40, 0, RGBA8(0, 0, 0, 150));
			defaultState == STATE_FRIENDADD? pp2d_draw_texture(TEXTURE_FRIENDS_ADD, 290, 0) : 
				pp2d_draw_texture_blend(TEXTURE_FRIENDS_ADD, 290, 0, RGBA8(0, 0, 0, 100));

		pp2d_end_draw();
		
		if (kDown & KEY_START)
			break;
		
		if ((kDown & KEY_TOUCH))
		{
			if (touchInRect(0, 35, 0, 30))
				defaultState = STATE_FRIENDCARD;
			else if (touchInRect(36, 71, 0, 30))
				defaultState = STATE_FRIENDLIST;
			else if (touchInRect(290, 320, 0, 30))
			{	
				u32 principalId = 0;
				u64 friendCode = 0;
				int old_state = defaultState;
				defaultState = STATE_FRIENDADD;

				char * buf = (char *)malloc(256);
				strcpy(buf, keyboard_3ds_get(256, "", "Enter friendcode without \"-\""));

				if (strncmp(buf, "", 1) == 0)
				{
					free(buf);
					defaultState = old_state;
				}
				else
				{
					bool validFC = false;
					sscanf(buf, "%lld", &friendCode); 
					FRD_IsValidFriendCode(friendCode, &validFC);

					if (validFC)
					{
						FRD_FriendCodeToPrincipalId(friendCode, &principalId);
						FRD_AddFriendOnline(event, principalId);
					}

					free(buf);
					defaultState = old_state;
				}
			}
		}
		
		if (((kHeld & KEY_L) && (kDown & KEY_R)) || ((kHeld & KEY_R) && (kDown & KEY_L)))
			Screenshot_Capture();
	}

	svcSignalEvent(s_terminate);
	threadJoin(thread, U64_MAX);
	svcCloseHandle(s_terminate);
	Term_Services();
	return 0;
}