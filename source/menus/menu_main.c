#include <3ds.h>

#include "C2D_helper.h"
#include "common.h"
#include "config.h"
#include "friend.h"
#include "menu_friendcard.h"
#include "menu_friendlist.h"
#include "status_bar.h"
#include "utils.h"

size_t friendCount = 0;

FriendKey friendKey[FRIEND_LIST_SIZE];

char friendNames[FRIEND_LIST_SIZE * 0xB];
char friendAuthor[FRIEND_LIST_SIZE * 0xB];

MiiStoreData friendMiiList[FRIEND_LIST_SIZE];

wchar_t wfriendComment[FRIENDS_COMMENT_SIZE];
char friendComment[FRIEND_LIST_SIZE][FRIENDS_COMMENT_SIZE];

bool isValid[FRIEND_LIST_SIZE];
bool isFromList[FRIEND_LIST_SIZE];

u64 friendCodes[FRIEND_LIST_SIZE];

void Menu_Main(void)
{
	Friend_GameIcon =  Friend_LoadGameIcon(Friend_GetFavouriteGame());

	FRD_GetFriendKeyList(friendKey, &friendCount, 0, FRIEND_LIST_SIZE);
	FRD_GetFriendMii2(friendMiiList, friendKey, friendCount);

	for (size_t i = 0x0; i < friendCount; i++) 
	{
		FRD_PrincipalIdToFriendCode(friendKey[i].principalId, &friendCodes[i]);
		
		FRD_IsValidFriendCode(friendCodes[i], &isValid[i]);
		isFromList[i] = Friend_IsFromFriendList(&friendKey[i]);
		
		Utils_U16_To_U8(&friendNames[i * 0xB], friendMiiList[i].mii_name, 0xB);
		friendNames[i * 0xB + 0xA] = 0;
		
		Utils_U16_To_U8(&friendAuthor[i * 0xB], friendMiiList[i].mii_name, 0xB);
		friendAuthor[i * 0xB + 0xA] = 0;

		FRD_GetFriendComment(wfriendComment, &friendKey[i], i);
		size_t size = utf16_to_utf8((uint8_t*)friendComment[i], (u16 *)wfriendComment, FRIENDS_COMMENT_SIZE);
		friendComment[i][size] = '\0';
	}

	while(aptMainLoop())
	{
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(RENDER_TOP, config_dark_theme? DARK_BG : DEFAULT_BG);
		C2D_TargetClear(RENDER_BOTTOM, config_dark_theme? DARK_BG : DEFAULT_BG);
		C2D_SceneBegin(RENDER_TOP);

		Draw_Rect(0, 0, 400, 20, config_dark_theme? STATUS_BAR_DARK : WHITE); // Status bar

		StatusBar_DisplayData();

		switch(MENU_STATE)
		{
			case STATE_FRIENDCARD:
				Menu_DisplayFriendCardTop();
				break;
			case STATE_FRIENDLIST:
				Menu_DisplayFriendListTop();
				break;
		}

		C2D_SceneBegin(RENDER_BOTTOM);

		switch(MENU_STATE)
		{
			case STATE_FRIENDCARD:
				Menu_DisplayFriendCardBottom();
				break;
			case STATE_FRIENDLIST:
				Menu_DisplayFriendListBottom();
				break;
		}

		Draw_EndFrame();

		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		Menu_ControlFriendList(kDown, kHeld);

		if (kDown & KEY_START)
			longjmp(exitJmp, 1);

		if (kDown & KEY_R)
			MENU_STATE++;
		if (kDown & KEY_L)
			MENU_STATE--;

		Utils_SetMax(&MENU_STATE, 0, 1);
		Utils_SetMin(&MENU_STATE, 1, 0);
	}
}