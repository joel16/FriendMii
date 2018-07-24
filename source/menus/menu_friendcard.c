#include "C2D_helper.h"
#include "common.h"
#include "config.h"
#include "friend.h"
#include "menu_friendcard.h"

void Menu_DisplayFriendCardTop(void)
{
	Draw_Rect(46, 33, 306, 184, BORDER_COLOUR);
	Draw_Rect(48, 35, 302, 180, config_dark_theme? DARK_BG : DEFAULT_BG);
	Draw_Rect(48, 35, 302, 137, config_dark_theme? STATUS_BAR_DARK : WHITE);
			
	// Game icon border
	Draw_Rect(53, 62, 52, 52, BORDER_COLOUR);
	Draw_Image(Friend_GameIcon, 55, 64);

	Draw_Text(53, 42, 0.6f, config_dark_theme? WHITE : BLACK, "Favourite Title:");
	Draw_Text(112, 70, 0.5f, BORDER_COLOUR, Friend_GetGameTitle(Friend_GetFavouriteGame()));
	Draw_Text(112, 90, 0.5f, BORDER_COLOUR, Friend_GetPlatform(Friend_GetFavouriteGame()));

	Draw_Text(345 - Draw_GetTextWidth(0.7f, Friend_GetScreenName()), 176, 0.7f, BORDER_COLOUR, Friend_GetScreenName());
	Draw_Textf(345 - (Draw_GetTextWidth(0.5f, "Friend Code: ") + Draw_GetTextWidth(0.5f, "0000 - 0000 - 0000")), 
		200, 0.5f, BORDER_COLOUR, "Friend Code: %s", Friend_GetFriendKey());

	if (!(Friend_GetHideStatus()))
		Draw_Text(((400 - Draw_GetTextWidth(0.55f, "Hide online status")) / 2), 221, 0.55f, config_dark_theme? WHITE : BLACK, "Hide online status");
	else if (!(Friend_GetHideTitleStatus()))
		Draw_Text(((400 - Draw_GetTextWidth(0.55f, "Hide titles being played")) / 2), 221, 0.55f, config_dark_theme? WHITE : BLACK, "Hide titles being played");
}

void Menu_DisplayFriendCardBottom(void)
{
	Draw_Rect(32, 47, 256, 44, BORDER_COLOUR);
	Draw_Rect(34, 49, 252, 40, config_dark_theme? STATUS_BAR_DARK : WHITE);
	Draw_Rect(96, 104, 128, 82, BORDER_COLOUR);
	Draw_Rect(98, 106, 124, 76, config_dark_theme? STATUS_BAR_DARK : WHITE);
	Draw_Rect(98, 158, 124, 26, config_dark_theme? DARK_BG : DEFAULT_BG);

	Draw_Text(34 + ((252 - Draw_GetTextWidth(0.6f, Friend_GetMyComment())) / 2), 60, 0.6f, BORDER_COLOUR, Friend_GetMyComment());
	Draw_Textf(100, 106, 0.5f, config_dark_theme? WHITE : BLACK, "%s", Friend_IsOnline()? "Online" : "Offline");
	Draw_Text(220 - Draw_GetTextWidth(0.5f, Friend_GetScreenName()), 164, 0.5f, BORDER_COLOUR, Friend_GetScreenName());
}