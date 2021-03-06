#include <3ds.h>

#include "C2D_helper.h"
#include "common.h"
#include "dialog.h"
#include "friend.h"
#include "frd.h"
#include "keyboard.h"
#include "menu_friendcard.h"
#include "menu_friendlist.h"
#include "status_bar.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

u32 friendCount = 0;

FriendKey friendKey[FRIEND_LIST_SIZE];
MiiData friendMiiList[FRIEND_LIST_SIZE];
GameDescription friendPlayGameDesc[FRIEND_LIST_SIZE];
GameDescription friendFavGameDesc[FRIEND_LIST_SIZE];

char friendNames[FRIEND_LIST_SIZE * 0xB];
char friendAuthor[FRIEND_LIST_SIZE * 0xB];
wchar_t wfriendComment[FRIENDS_COMMENT_SIZE];
char friendComment[FRIEND_LIST_SIZE][FRIENDS_COMMENT_SIZE];
char friendGameDescList[FRIEND_LIST_SIZE][128];

bool isValid[FRIEND_LIST_SIZE];
bool isFromList[FRIEND_LIST_SIZE];

u64 friendCodes[FRIEND_LIST_SIZE];
u64 friendFavTIDs[FRIEND_LIST_SIZE];
u64 friendPlayTIDs[FRIEND_LIST_SIZE];

void Menu_Main(void) {
	Friend_GameIcon =  Friend_LoadGameIcon(Friend_GetFavouriteGame());

	FRD_GetFriendKeyList(friendKey, &friendCount, 0, FRIENDS_COMMENT_SIZE);
	FRD_GetFriendMii(friendMiiList, friendKey, friendCount);

	for (u32 i = 0x0; i < friendCount; i++) {
		FRD_PrincipalIdToFriendCode(friendKey[i].principalId, &friendCodes[i]);
		
		FRD_IsValidFriendCode(friendCodes[i], &isValid[i]);
		isFromList[i] = Friend_IsFromFriendList(&friendKey[i]);
		
		Utils_U16_To_U8((u8 *)&friendNames[i * 0xB], friendMiiList[i].mii_name, 0xB);
		friendNames[i * 0xB + 0xA] = 0;
		
		Utils_U16_To_U8((u8 *)&friendAuthor[i * 0xB], friendMiiList[i].mii_name, 0xB);
		friendAuthor[i * 0xB + 0xA] = 0;

		FRD_GetFriendPlayingGame(&friendPlayGameDesc[i], &friendKey[i], friendCount);
		FRD_GetFriendFavouriteGame(&friendFavGameDesc[i], &friendKey[i], friendCount);

		FRD_GetFriendComment(wfriendComment, &friendKey[i], i);
		Utils_U16_To_U8((u8 *)friendComment[i], (u16 *)wfriendComment, FRIENDS_COMMENT_SIZE);

		friendPlayTIDs[i] = friendPlayGameDesc[i].data.tid;
		//Utils_U16_To_U8((u8 *)friendGameDescList[i], friendFavGameDesc[i].desc, 128);

		friendFavTIDs[i] = friendFavGameDesc[i].data.tid;
		//Utils_U16_To_U8((u8 *)friendGameDescList[i], friendFavGameDesc[i].desc, 128);
	}

	bool close_touch = false;

	while(aptMainLoop()) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(RENDER_TOP, DEFAULT_BG);
		C2D_TargetClear(RENDER_BOTTOM, DEFAULT_BG);
		C2D_SceneBegin(RENDER_TOP);

		Draw_Rect(0, 0, 400, 20, WHITE); // Status bar

		Draw_Rect(0, 20, 400, 220, DEFAULT_BG);
		Draw_Rect(46, 33, 306, 184, WHITE);

		StatusBar_DisplayData();

		switch(MENU_STATE) {
			case STATE_FRIENDCARD:
				Menu_DisplayFriendCardTop();
				break;
			case STATE_FRIENDLIST:
				Menu_DisplayFriendListTop();
				break;
		}

		C2D_SceneBegin(RENDER_BOTTOM);

		Draw_Rect(34, 49, 252, 40, WHITE);
		Draw_Rect(96, 104, 128, 80, WHITE);
		Draw_Rect(98, 158, 124, 26, WHITE);

		switch(MENU_STATE) {
			case STATE_FRIENDCARD:
				Menu_DisplayFriendCardBottom();
				break;
			case STATE_FRIENDLIST:
				Menu_DisplayFriendListBottom();
				break;
		}

		Draw_Rect(0, 0, 320, 20, C2D_Color32(70, 70, 78, 255));

		//Draw_Image(icon_delete, 225, 0);
		Draw_Image(icon_backup, 250, 0);
		Draw_Image(icon_add, 275, 0);
		Draw_Image(icon_search, 300, 0);

		Draw_Rect(30, 210, 260, 30, close_touch == false? C2D_Color32(70, 70, 78, 255) : C2D_Color32(100, 100, 100, 255));

		Draw_Text(30 + ((260 - Draw_GetTextWidth(0.6f, "\uE071 Close")) / 2) , 210 + ((30 - Draw_GetTextHeight(0.6f, "\uE071 Close")) / 2), 0.6f, WHITE, "\uE070 Close");

		Draw_EndFrame();

		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		if (TouchInRect(245, 0, 269, 25)) {
			if (kDown & KEY_TOUCH)
				Menu_DisplayBackupDialog();
		}
		else if (TouchInRect(270, 0, 294, 25)) {
			if (kDown & KEY_TOUCH) {
				Keyboard_GetFriendCode();
				Menu_Main();
			}
		}
		else if (TouchInRect(295, 0, 320, 25)) {
			if (kDown & KEY_TOUCH) {
				Keyboard_SearchFriend("Enter a friend's name.");
				Menu_Main();
			}
		}

		switch(MENU_STATE) {
			case STATE_FRIENDLIST:
				Menu_ControlFriendList(kDown, kHeld);
				break;
		}

		if (kDown & KEY_R)
			MENU_STATE++;
		if (kDown & KEY_L)
			MENU_STATE--;

		Utils_SetMax(&MENU_STATE, 0, 1);
		Utils_SetMin(&MENU_STATE, 1, 0);

		if (TouchInRect(30, 210, 290, 240)) {
			close_touch = true;
			
			if (kDown & KEY_TOUCH)
				longjmp(exitJmp, 1);
		}
		else
			close_touch = false;

		if (kDown & KEY_START)
			longjmp(exitJmp, 1);
	}
}
