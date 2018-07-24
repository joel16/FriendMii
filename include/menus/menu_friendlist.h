#ifndef FRIENDMII_MENU_FRIENDLIST_H
#define FRIENDMII_MENU_FRIENDLIST_H

#include <3ds.h>

#include "frd.h"

extern size_t friendCount;

extern FriendKey friendKey[FRIEND_LIST_SIZE];

extern char friendNames[FRIEND_LIST_SIZE * 0xB];
extern char friendAuthor[FRIEND_LIST_SIZE * 0xB];

extern MiiStoreData friendMiiList[FRIEND_LIST_SIZE];

extern wchar_t wfriendComment[FRIENDS_COMMENT_SIZE];
extern char friendComment[FRIEND_LIST_SIZE][FRIENDS_COMMENT_SIZE];

extern bool isValid[FRIEND_LIST_SIZE];
extern bool isFromList[FRIEND_LIST_SIZE];

extern u64 friendCodes[FRIEND_LIST_SIZE];

void Menu_DisplayFriendListTop(void);
void Menu_DisplayFriendListBottom(void);
void Menu_ControlFriendList(u32 kDown, u32 kHeld);

#endif