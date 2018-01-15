#include <3ds.h>

#include <stdio.h>

#include "draw.h"
#include "frd.h"
#include "friend.h"
#include "fs.h"
#include "mii.h"
#include "power.h"
#include "pp2d.h"
#include "screenshot.h"
#include "status_bar.h"
#include "utils.h"

#define STATE_FRIENDCARD 0
#define STATE_FRIENDLIST 1
#define STATE_MII        2

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
	pp2d_load_texture_png(TEXTURE_FRIENDS_LIST, "romfs:/ic_material_light_filesystem.png");
	pp2d_load_texture_png(TEXTURE_FRIENDS_MII, "romfs:/ic_fso_type_contact.png");

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

	pp2d_free_texture(TEXTURE_FRIENDS_MII);
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
	
	Friend_GetGameIcon(Friend_GetFavouriteGame());
	
	size_t friendCount = 0;
	FriendKey friendKey[FRIEND_LIST_SIZE];
	Profile profileList[FRIEND_LIST_SIZE];
	FriendPresence friendPresenceList[FRIEND_LIST_SIZE];
	FRD_GetFriendKeyList(friendKey, &friendCount, 0, FRIEND_LIST_SIZE);
	FRD_GetFriendProfile(profileList, friendKey, FRIEND_LIST_SIZE);
	//FRD_GetFriendPresence(friendPresenceList, friendKey, FRIEND_LIST_SIZE);
	
	MiiStoreData friendMii[FRIEND_LIST_SIZE];
    char friendNames[FRIEND_LIST_SIZE * 0xB];
	char friendAuthor[FRIEND_LIST_SIZE * 0xB];
	FRD_GetFriendMii(friendMii, friendKey, FRIEND_LIST_SIZE); 
	
	bool isValid[FRIEND_LIST_SIZE];
	bool isFromList[FRIEND_LIST_SIZE];
	
	u64 friendCodes[FRIEND_LIST_SIZE];
	for (size_t i = 0x0; i < friendCount; i++) 
	{
		FRD_PrincipalIdToFriendCode(friendKey[i].principalId, &friendCodes[i]);
		FRD_IsValidFriendCode(friendCodes[i], &isValid[i]);
		isFromList[i] = Friend_IsFromFriendList(&friendKey[i]);
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
	
	int selection = 0;

	while (aptMainLoop()) 
	{
		hidScanInput();
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
			pp2d_draw_rectangle(0, 0, 400, 20, RGBA8(164, 164, 164, 255));
			StatusBar_DisplayBar();
			pp2d_draw_textf(5, 3, 0.5f, 0.5f, RGBA8(0, 0, 0, 255), "FriendMii v%d.%02d 0x%lx", VERSION_MAJOR, VERSION_MINOR);

			switch(defaultState)
			{
				case STATE_FRIENDCARD:
					drawTopScreen(STATE_FRIENDCARD);
					pp2d_draw_texture(TEXTURE_GAME_ICON, 55, 64);
					pp2d_draw_text(348 - pp2d_get_text_width(Friend_GetScreenName(), 0.7f, 0.7f), 176, 0.7f, 0.7f, RGBA8(170, 170, 170, 255), Friend_GetScreenName());
					pp2d_draw_textf(348 - (pp2d_get_text_width("Friend Code: ", 0.5f, 0.5f) + pp2d_get_text_width("0000 - 0000 - 0000", 0.5f, 0.5f)), 
						200, 0.5f, 0.5f, RGBA8(148, 148, 148, 255), "Friend Code: %s", Friend_GetFriendKey());
		
					pp2d_draw_text(53, 42, 0.6f, 0.6f, RGBA8(82, 82, 82, 255), "Favourite Title:");
					pp2d_draw_text(112, 70, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetGameTitle(Friend_GetFavouriteGame()));
					pp2d_draw_text(112, 90, 0.5f, 0.5f, RGBA8(82, 82, 82, 255), Friend_GetPlatform(Friend_GetFavouriteGame()));
		
					if (!(Friend_GetHideStatus()))
						pp2d_draw_text(((400 - pp2d_get_text_width("Hide online status", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(53, 119, 151, 255), "Hide online status");
					else if (!(Friend_GetHideTitleStatus()))
						pp2d_draw_text(((400 - pp2d_get_text_width("Hide titles being played", 0.55f, 0.55f)) / 2), 221, 0.55f, 0.55f, RGBA8(53, 119, 151, 255), "Hide titles being played");
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
								pp2d_draw_rectangle(4, 22 + (selector_yDistance * printed), 392, 35, RGBA8(255, 255, 255, 255));

							pp2d_draw_texture(Friend_GetFriendColour(friendMii[i].colour), 10, 25 + (selector_yDistance * printed));

							pp2d_draw_textf(50, 30 + (selector_yDistance * printed), 0.5f, 0.5f, i == selection? RGBA8(82, 82, 82, 255) : RGBA8(255, 255, 255, 255), "%s : %04llu-%04llu-%04llu", &friendNames[i * 0xB], friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000);

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
				
				case STATE_MII:
					drawTopScreen(STATE_MII);
					/*pp2d_draw_rectangle(10 + pp2d_get_text_width("Mii color: ", 0.6f, 0.6f), 91, 16, 16, RGBA8(53, 119, 151, 255));
					pp2d_draw_rectangle(10 + pp2d_get_text_width("Mii color: ", 0.6f, 0.6f) + 1, 92, 14, 14, MII_GetMiiColour(miiData.color));
					pp2d_draw_textf(10, 30, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii name: %s", mii_name);
					pp2d_draw_textf(10, 50, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii mac address: %02X:%02X:%02X:%02X:%02X:%02X", miiData.mac[0], miiData.mac[1], miiData.mac[2], miiData.mac[3], miiData.mac[4], miiData.mac[5]);
					pp2d_draw_textf(10, 70, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii gender: %s", miiData.gender? "Female" : "Male");
					pp2d_draw_text(10, 90, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii color: ");
					pp2d_draw_textf(10, 110, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii Birthday: %d/%d", miiData.bday_month, miiData.bday_day);
					pp2d_draw_textf(10, 130, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii favorite: %s", miiData.favorite? "Yes" : "No");
					pp2d_draw_textf(10, 150, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii copying: %s", miiData.copyable? "Enabled" : "Disabled");
					pp2d_draw_textf(10, 170, 0.6f, 0.6f, RGBA8(53, 119, 151, 255), "Mii sharing: %s", miiData.disable_sharing? "Disabled" : "Enabled");*/
					break;
			}
		pp2d_end_draw();

		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
			switch(defaultState)
			{
				case STATE_FRIENDCARD:
					drawBottomScreen();

					pp2d_draw_text(34 + ((252 - pp2d_get_text_width(Friend_GetMyComment(), 0.6f, 0.6f)) / 2), 60, 0.6f, 0.6f, RGBA8(82, 82, 82, 255), Friend_GetMyComment());
		
					pp2d_draw_textf(100, 106, 0.5f, 0.5f, RGBA8(53, 119, 151, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
					pp2d_draw_text(220 - pp2d_get_text_width(Friend_GetScreenName(), 0.5f, 0.5f), 164, 0.5f, 0.5f, RGBA8(148, 148, 148, 255), Friend_GetScreenName());
					break;

				case STATE_FRIENDLIST:
					drawBottomScreen();

					float width =  friendMii[selection].gender? pp2d_get_text_width("♀ ", 0.5f, 0.5f) : pp2d_get_text_width("♂ ", 0.5f, 0.5f);
					//pp2d_draw_text(34 + ((252 - pp2d_get_text_width(Friend_GetMyComment(), 0.6f, 0.6f)) / 2), 60, 0.6f, 0.6f, RGBA8(82, 82, 82, 255), Friend_GetMyComment());
		
					//pp2d_draw_textf(100, 106, 0.5f, 0.5f, RGBA8(53, 119, 151, 255), "%s", Friend_IsOnline()? "Online" : "Offline");
					pp2d_draw_textf(220 - (pp2d_get_text_width(&friendNames[selection * 0xB], 0.5f, 0.5f) + width), 164, 0.5f, 0.5f, RGBA8(148, 148, 148, 255), "%s %s", &friendNames[selection * 0xB], friendMii[selection].gender? "♀" : "♂");

					if (!isFromList[selection])
						pp2d_draw_text(((320 - pp2d_get_text_width("This person does not have you added", 0.55f, 0.55f)) / 2), 190, 0.55f, 0.55f, RGBA8(53, 119, 151, 255), "This person does not have you added");
					break;
			}

			pp2d_draw_rectangle((34 * defaultState), 0, 34, 26, RGBA8(53, 119, 151, 255));
			pp2d_draw_texture(TEXTURE_FRIENDS_HOME, 4, 0);
			pp2d_draw_texture(TEXTURE_FRIENDS_LIST, 38, 0);
			//pp2d_draw_texture(TEXTURE_FRIENDS_MII, 72, 0);
		pp2d_end_draw();
		
		if (kDown & KEY_START)
			break;
		
		if ((kDown & KEY_TOUCH))
		{
			if (touchInRect(4, 34, 0, 26))
				defaultState = STATE_FRIENDCARD;
			else if (touchInRect(38, 68, 0, 26))
				defaultState = STATE_FRIENDLIST;
			/*else if (touchInRect(72, 102, 0, 26))
				defaultState = STATE_MII;*/
		}
		
		if (((kHeld & KEY_L) && (kDown & KEY_R)) || ((kHeld & KEY_R) && (kDown & KEY_L)))
			Screenshot_Capture();
	}

	Term_Services();
	return 0;
}