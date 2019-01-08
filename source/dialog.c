#include <3ds.h>

#include "C2D_helper.h"
#include "common.h"
#include "menu_friendlist.h"
#include "menu_main.h"
#include "touch.h"
#include "utils.h"

void Menu_DisplayDeleteDialog(void) {
	float text_width = 0, text2_width = 0;
	int dialog_selection = 0;
	float confirm_width = 0, confirm_height = 0;
	float cancel_width = 0, cancel_height = 0;

	bool refresh = false;

	while(aptMainLoop()) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(RENDER_BOTTOM, DEFAULT_BG);
		C2D_SceneBegin(RENDER_BOTTOM);

		Draw_GetTextSize(0.5f, &text_width, NULL, "This action cannot be undone.");
		Draw_GetTextSize(0.5f, &text2_width, NULL, "Do you wish to remove this person?");
		Draw_GetTextSize(0.5f, &confirm_width, &confirm_height, "YES");
		Draw_GetTextSize(0.5f, &cancel_width, &cancel_height, "NO");

		Draw_Rect(20, 69, 280, 102, WHITE);

		Draw_Textf(26, 75, 0.5f, C2D_Color32(126, 52, 34, 255), "Remove %s",  &friendNames[selection * 0xB]);

		Draw_Text(((320 - (text_width)) / 2), 104, 0.5f, BLACK, "This action cannot be undone.");
		Draw_Text(((320 - (text2_width)) / 2), 118, 0.5f, BLACK, "Do you wish to remove this person?");

		if (dialog_selection == 0)
			Draw_Rect((288 - cancel_width) - 5, (159 - cancel_height) - 5, cancel_width + 10, cancel_height + 10, SELECTOR_COLOUR_LIGHT);
		else if (dialog_selection == 1)
			Draw_Rect((248 - (confirm_width)) - 5, (159 - confirm_height) - 5, confirm_width + 10, confirm_height + 10, SELECTOR_COLOUR_LIGHT);

		Draw_Text(248 - (confirm_width), (159 - confirm_height), 0.5f, BLACK, "YES");
		Draw_Text(288 - cancel_width, (159 - cancel_height), 0.5f, BLACK, "NO");

		Draw_EndFrame();

		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_RIGHT)
			dialog_selection++;
		else if (kDown & KEY_LEFT)
			dialog_selection--;

		Utils_SetMax(&dialog_selection, 0, 1);
		Utils_SetMin(&dialog_selection, 1, 0);

		if (kDown & KEY_B)
			break;

		if (kDown & KEY_A) {
			if (dialog_selection == 1) {
				FRD_RemoveFriend(friendKey[selection].principalId, friendCodes[selection]);
				refresh = true;
				break;
			}
			else
				break;
		}

		if (TouchInRect((288 - cancel_width) - 5, (159 - cancel_height) - 5, ((288 - cancel_width) - 5) + cancel_width + 10, ((159 - cancel_height) - 5) + cancel_height + 10)) {
			dialog_selection = 0;

			if (kDown & KEY_TOUCH)
				break;
		}
		else if (TouchInRect((248 - (confirm_height)) - 5, (159 - confirm_height) - 5, ((248 - (confirm_height)) - 5) + confirm_height + 10, ((159 - confirm_height) - 5) + confirm_height + 10)) {
			dialog_selection = 1;

			if (kDown & KEY_TOUCH) {
				FRD_RemoveFriend(friendKey[selection].principalId, friendCodes[selection]);
				refresh = true;
				break;
			}
		}
	}

	MENU_STATE = STATE_FRIENDLIST;
	
	if (refresh)
		Menu_Main();
}
