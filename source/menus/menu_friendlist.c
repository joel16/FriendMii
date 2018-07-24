#include "C2D_helper.h"
#include "common.h"
#include "config.h"
#include "friend.h"
#include "menu_friendlist.h"
#include "status_bar.h"
#include "utils.h"

#define DISTANCE_Y    35
#define LIST_PER_PAGE 6

static int selection = 0;

void Menu_DisplayFriendListTop(void)
{
	int printed = 0; // Print counter

	for (size_t i = 0x0; i < friendCount; i++)
	{
		if (printed == LIST_PER_PAGE)
			break;

		if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE))
		{
			if (i == selection)
				Draw_Rect(4, 20 + (DISTANCE_Y * printed), 392, 35, SELECTOR_COLOUR_LIGHT);
			
			//Draw_Texture(Friend_GetFriendColour(friendMiiList[i].colour), 10, 25 + (DISTANCE_Y * printed));
			Draw_Textf(50, 30 + (DISTANCE_Y * printed), 0.5f, i == selection? BORDER_COLOUR : BLACK, "%s : %04llu-%04llu-%04llu", 
				strlen(&friendNames[i * 0xB]) == 0? "Unknown" : &friendNames[i * 0xB],
				friendCodes[i]/100000000LL, (friendCodes[i]/10000)%10000, friendCodes[i]%10000);
			
			printed++; // Increase printed counter
		}
	}
}

void Menu_DisplayFriendListBottom(void)
{
	Draw_Rect(32, 47, 256, 44, BORDER_COLOUR);
	Draw_Rect(34, 49, 252, 40, config_dark_theme? STATUS_BAR_DARK : WHITE);
	Draw_Rect(96, 104, 128, 82, BORDER_COLOUR);
	Draw_Rect(98, 106, 124, 76, config_dark_theme? STATUS_BAR_DARK : WHITE);
	Draw_Rect(98, 158, 124, 26, config_dark_theme? DARK_BG : DEFAULT_BG);

	float width =  friendMiiList[selection].gender? Draw_GetTextWidth(0.5f, "♀ ") : Draw_GetTextWidth(0.5f, "♂ ");
	Draw_Text(34 + ((252 - Draw_GetTextWidth(0.6f, friendComment[selection])) / 2), 60, 0.6f, BORDER_COLOUR, friendComment[selection]);
		
	//Draw_Textf(100, 106, 0.5f, BORDER_COLOUR, "%s", Friend_IsOnline()? "Online" : "Offline");
	Draw_Textf(220 - (Draw_GetTextWidth(0.5f, &friendNames[selection * 0xB]) + width), 164, 0.5f, BORDER_COLOUR, "%s %s", &friendNames[selection * 0xB], friendMiiList[selection].gender? "♀" : "♂");

	if (!isFromList[selection])
		Draw_Text(((320 - Draw_GetTextWidth(0.55f, "This person does not have you added")) / 2), 190, 0.55f, config_dark_theme? WHITE : BLACK, "This person does not have you added");
}

void Menu_ControlFriendList(u32 kDown, u32 kHeld)
{
	if (kDown & KEY_DDOWN)
		selection++;
	else if (kDown & KEY_DUP)
		selection--;

	if (kHeld & KEY_CPAD_DOWN)
	{
		wait(5);
		selection++;
	}
	else if (kHeld & KEY_CPAD_UP)
	{
		wait(5);
		selection--;
	}

	Utils_SetMax(&selection, 0, friendCount - 1);
	Utils_SetMin(&selection, friendCount - 1, 0);
}
