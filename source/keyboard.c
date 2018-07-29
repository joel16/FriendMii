#include <stdlib.h>
#include <string.h>

#include "C2D_helper.h"
#include "keyboard.h"

const char *Keyboard_GetString(const char *initial_text, const char *hint_text)
{
	Draw_EndFrame();

	static SwkbdState swkbd;
	static SwkbdStatusData swkbdStatus;
	static SwkbdLearningData swkbdLearning;
	static bool reload = false;
	static char input_string[256];

	swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, 256);

	if (strlen(hint_text) != 0)
		swkbdSetHintText(&swkbd, hint_text);
	
	if (strlen(initial_text) != 0)
		swkbdSetInitialText(&swkbd, initial_text);

	swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
	swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Confirm", true);

	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_HOME);
	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_RESET);
	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_POWER);
	swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);

	swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);

	SwkbdDictWord words[7];
	swkbdSetDictWord(&words[0], ".3dsx", ".3dsx");
	swkbdSetDictWord(&words[1], ".cia", ".cia");
	swkbdSetDictWord(&words[2], "github", "github");
	swkbdSetDictWord(&words[3], "http://", "http://");
	swkbdSetDictWord(&words[4], "https://", "https://");
	swkbdSetDictWord(&words[5], "releases", "releases");
	swkbdSetDictWord(&words[6], "/3ds/", "/3ds/");
	swkbdSetDictionary(&swkbd, words, 7);

	swkbdSetStatusData(&swkbd, &swkbdStatus, reload, true);
	swkbdSetLearningData(&swkbd, &swkbdLearning, reload, true);
	reload = true;

	swkbdInputText(&swkbd, input_string, sizeof(input_string));

	return input_string;
}

static SwkbdCallbackResult FRD_AddFriendCallback(void* user, const char** ppMessage, const char* text, size_t textlen)
{
	u64 friend_code = 0;
	u32 principal_id = 0;
	bool isInputValid = false;

	sscanf(text, "%lld", &friend_code); 

	if (R_SUCCEEDED(FRD_IsValidFriendCode(friend_code, &isInputValid)))
	{
		if (isInputValid)
		{
			if (R_SUCCEEDED(FRD_FriendCodeToPrincipalId(friend_code, &principal_id)))
			{
				if (R_SUCCEEDED(FRD_AddFriendOnline(0, principal_id)))
				{
					*ppMessage = "Friend added successfully.";
					return SWKBD_CALLBACK_CLOSE;
				}
				else
				{
					*ppMessage = "Failed to add friend.";
					return SWKBD_CALLBACK_CLOSE;
				}
			}
			else
			{
				*ppMessage = "Failed to convert friend code to principal ID.";
				return SWKBD_CALLBACK_CLOSE;
			}
		}
		else
		{
			*ppMessage = "Invalid friend code.";
			return SWKBD_CALLBACK_CLOSE;
		}
	}
	else
	{
		*ppMessage = "Failed to check friend code validity.";
		return SWKBD_CALLBACK_CLOSE;
	}

	return SWKBD_CALLBACK_OK;
}

void Keyboard_GetFriendCode(void)
{
	Draw_EndFrame();

	static SwkbdState swkbd;
	char input_string[13];
	u64 input = 0;

	swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 12);

	swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
	swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
	swkbdSetFilterCallback(&swkbd, FRD_AddFriendCallback, NULL);
	swkbdInputText(&swkbd, input_string, sizeof(input_string));
	sscanf(input_string, "%lld", &input); 

	return;
}
