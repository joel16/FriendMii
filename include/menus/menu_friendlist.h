#ifndef FRIENDMII_MENU_FRIENDLIST_H
#define FRIENDMII_MENU_FRIENDLIST_H

#include <3ds.h>

#include "frd.h"

extern u32 friendCount;

extern FriendKey friendKey[FRIEND_LIST_SIZE];
extern MiiData friendMiiList[FRIEND_LIST_SIZE];
extern GameDescription friendPlayGameDesc[FRIEND_LIST_SIZE];
extern GameDescription friendFavGameDesc[FRIEND_LIST_SIZE];

extern char friendNames[FRIEND_LIST_SIZE * 0xB];
extern char friendAuthor[FRIEND_LIST_SIZE * 0xB];
extern wchar_t wfriendComment[FRIENDS_COMMENT_SIZE];
extern char friendComment[FRIEND_LIST_SIZE][FRIENDS_COMMENT_SIZE];
extern char friendGameDescList[FRIEND_LIST_SIZE][128];

extern bool isValid[FRIEND_LIST_SIZE];
extern bool isFromList[FRIEND_LIST_SIZE];

extern u64 friendCodes[FRIEND_LIST_SIZE];
extern u64 friendPlayTIDs[FRIEND_LIST_SIZE];
extern u64 friendFavTIDs[FRIEND_LIST_SIZE];

extern int selection;

void Menu_DisplayFriendListTop(void);
void Menu_DisplayFriendListBottom(void);
void Menu_ControlFriendList(u32 kDown, u32 kHeld);

#endif
