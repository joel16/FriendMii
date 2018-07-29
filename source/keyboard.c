#include <stdlib.h>
#include <string.h>

#include "C2D_helper.h"
#include "log.h"
#include "menu_friendlist.h"
#include "keyboard.h"

static SwkbdCallbackResult SearchFriendCallback(void* user, const char** ppMessage, const char* text, size_t textlen)
{
	for (int i = 0; i < friendCount; i++)
	{
		if (strlen(text) != 0)
		{
			DEBUG("Friend %d: %s\n", i, &friendNames[i * 0xB]);
			if (strstr(text, &friendNames[i * 0xB]))
			{
				selection = i;
				return SWKBD_CALLBACK_OK;
			}
		}
	}

	*ppMessage = "  Could not find anyone with   that name.";
	return SWKBD_CALLBACK_CLOSE;
}

void Keyboard_SearchFriend(const char *hint_text)
{
	Draw_EndFrame();

	static SwkbdState swkbd;
	static SwkbdStatusData swkbdStatus;
	char input_string[0xC];

	swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, 0xC);

	if (strlen(hint_text) != 0)
		swkbdSetHintText(&swkbd, hint_text);

	swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
	swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Confirm", true);

	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_HOME);
	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_RESET);
	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_POWER);
	swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);

	swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);

	swkbdSetFilterCallback(&swkbd, SearchFriendCallback, NULL);
	swkbdInputText(&swkbd, input_string, sizeof(input_string));

	return;
}

static SwkbdCallbackResult AddFriendCallback(void* user, const char** ppMessage, const char* text, size_t textlen)
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

	swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 12);

	swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
	swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
	swkbdSetFilterCallback(&swkbd, AddFriendCallback, NULL);
	swkbdInputText(&swkbd, input_string, sizeof(input_string));

	return;
}
