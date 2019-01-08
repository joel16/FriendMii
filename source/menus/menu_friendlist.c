#include "C2D_helper.h"
#include "common.h"
#include "dialog.h"
#include "friend.h"
#include "menu_friendlist.h"
#include "status_bar.h"
#include "touch.h"
#include "utils.h"

#define DISTANCE_Y    35
#define LIST_PER_PAGE 1

int selection = 0;
static bool remove_selected = false;

void Menu_DisplayFriendListTop(void) {
	Draw_Rect(50, 37, 298, 80, C2D_Color32(224, 224, 224, 255));
	Draw_Rect(50, 117, 298, 57, C2D_Color32(176, 176, 176, 255));

	Draw_Text(53, 42, 0.6f, TEXT_COLOUR, "Favourite Title:");

	int printed = 0; // Print counter

	for (u32 i = 0x0; i < friendCount; i++) {
		if (printed == LIST_PER_PAGE)
			break;

		if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE)) {
			Draw_Textf(112, 70, 0.5f, TEXT_COLOUR, "(%016llX)", (friendPlayTIDs[selection] == 0)? friendFavTIDs[selection] : friendPlayTIDs[selection]);

			Draw_Text(345 - Draw_GetTextWidth(0.7f, strlen(&friendNames[i * 0xB]) == 0? "Unknown" : &friendNames[i * 0xB]), 176, 0.7f, 
				C2D_Color32(126, 52, 34, 255), strlen(&friendNames[i * 0xB]) == 0? "Unknown" : &friendNames[i * 0xB]);
			Draw_Textf(345 - (Draw_GetTextWidth(0.5f, "Friend Code: ") + Draw_GetTextWidth(0.5f, "0000 - 0000 - 0000")),
				200, 0.5f, C2D_Color32(140, 138, 138, 255), "Friend Code: %04llu - %04llu - %04llu", friendCodes[i]/100000000LL, 
				(friendCodes[i]/10000)%10000, friendCodes[i]%10000);
			
			printed++; // Increase printed counter
		}
	}
}

//static char friendListCount[10];

void Menu_DisplayFriendListBottom(void) {
	Draw_Rect(106, 20, (Draw_GetTextWidth(0.45, "\uE070 Remove friend") + 10), 20, remove_selected? C2D_Color32(224, 224, 224, 255) : C2D_Color32(176, 176, 176, 255));
	Draw_Rect(110 - 3, 21, (Draw_GetTextWidth(0.45, "\uE070 Remove friend") + 8), 18, WHITE);
	Draw_Rect(110 - 2, 22, (Draw_GetTextWidth(0.45, "\uE070 Remove friend") + 6), 16, remove_selected? C2D_Color32(224, 224, 224, 255) : C2D_Color32(176, 176, 176, 255));
	Draw_Text(110, 23, 0.45f, C2D_Color32(126, 52, 34, 255), "\uE070 Remove friend");

	Draw_Rect(98, 106, 124, 76, C2D_Color32(224, 224, 224, 255));
	Draw_Rect(98, 106, 124, 38, C2D_Color32(176, 176, 176, 255));

	float comment_width = isFromList[selection]? (strlen(friendComment[selection]) == 0? Draw_GetTextWidth(0.55f, "No status message.") : 
		Draw_GetTextWidth(0.55f, friendComment[selection])) : Draw_GetTextWidth(0.55f, "Provisionally registered friend.");
	
	Draw_Text(34 + ((252 - comment_width) / 2), 60, 0.55f, TEXT_COLOUR2, isFromList[selection]? 
		(strlen(friendComment[selection]) == 0? "No status message." : friendComment[selection]) : "Provisionally registered friend.");
		
	//Draw_Textf(100, 106, 0.5f, FRIEND_FILL, "%s", Friend_IsOnline()? "Online" : "Offline");
	Draw_Textf(220 - (Draw_GetTextWidth(0.5f, &friendNames[selection * 0xB])), 164, 0.5f, C2D_Color32(126, 52, 34, 255), "%s %s", &friendNames[selection * 0xB]);

	//snprintf(friendListCount, 10, "%d / %d", selection, friendCount);
	Draw_Textf((320 - (Draw_GetTextWidth(0.5f, "X / X"))) / 2, 190, 0.5f, C2D_Color32(126, 52, 34, 255), "%d / %d", selection + 1, friendCount);
}

void Menu_ControlFriendList(u32 kDown, u32 kHeld) {
	if (TouchInRect(106, 20, 106 + ((Draw_GetTextWidth(0.45, "\uE070 Remove friend") + 10)), 40)) {
		remove_selected = true;
		
		if (kDown & KEY_TOUCH)
			Menu_DisplayDeleteDialog();
	}
	else
		remove_selected = false;

	if (kDown & KEY_DRIGHT)
		selection++;
	else if (kDown & KEY_DLEFT)
		selection--;

	if (kHeld & KEY_CPAD_RIGHT) {
		wait(5);
		selection++;
	}
	else if (kHeld & KEY_CPAD_LEFT) {
		wait(5);
		selection--;
	}

	Utils_SetMax(&selection, 0, friendCount - 1);
	Utils_SetMin(&selection, friendCount - 1, 0);

	if (kDown & KEY_X)
		Menu_DisplayDeleteDialog();
}
