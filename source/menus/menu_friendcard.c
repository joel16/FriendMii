#include "C2D_helper.h"
#include "common.h"
#include "friend.h"

void Menu_DisplayFriendCardTop(void) {
	Draw_Rect(50, 37, 298, 80, C2D_Color32(255, 231, 213, 255));
	Draw_Rect(50, 117, 298, 57, C2D_Color32(255, 194, 148, 255));

	Draw_Text(53, 42, 0.6f, TEXT_COLOUR, "Favourite Title:");

	Draw_Image(Friend_GameIcon, 55, 64);

	Draw_Text(112, 70, 0.5f, TEXT_COLOUR, (const char *)Friend_GetGameTitle(Friend_GetFavouriteGame()));
	Draw_Text(112, 90, 0.5f, TEXT_COLOUR, Friend_GetPlatform(Friend_GetFavouriteGame()));

	Draw_Text(345 - Draw_GetTextWidth(0.7f, Friend_GetScreenName()), 176, 0.7f, C2D_Color32(126, 52, 34, 255), Friend_GetScreenName());
	Draw_Textf(345 - (Draw_GetTextWidth(0.5f, "Friend Code: ") + Draw_GetTextWidth(0.5f, "0000 - 0000 - 0000")), 
		200, 0.5f, C2D_Color32(140, 138, 138, 255), "Friend Code: %s", Friend_GetFriendKey());

	if (!(Friend_GetHideStatus()))
		Draw_Text(((400 - Draw_GetTextWidth(0.55f, "Hide online status")) / 2), 221, 0.55f, TEXT_COLOUR_LIGHT, "Hide online status");
	else if (!(Friend_GetHideTitleStatus()))
		Draw_Text(((400 - Draw_GetTextWidth(0.55f, "Hide titles being played")) / 2), 221, 0.55f, TEXT_COLOUR_LIGHT, "Hide titles being played");
}

void Menu_DisplayFriendCardBottom(void) {
	Draw_Rect(98, 106, 124, 76, C2D_Color32(255, 194, 148, 255));
	Draw_Rect(98, 106, 124, 38, C2D_Color32(255, 231, 213, 255));
	
	Draw_Text(34 + ((252 - Draw_GetTextWidth(0.6f, Friend_GetMyComment())) / 2), 60, 0.6f, TEXT_COLOUR2, Friend_GetMyComment());
	Draw_Textf(100, 106, 0.5f, TEXT_COLOUR_LIGHT2, "%s", Friend_IsOnline()? "Online" : "Offline");
	Draw_Text(220 - Draw_GetTextWidth(0.5f, Friend_GetScreenName()), 164, 0.5f, C2D_Color32(126, 52, 34, 255), Friend_GetScreenName());
}
