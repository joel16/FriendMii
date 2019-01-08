#ifndef FRIENDMII_FRIEND_H
#define FRIENDMII_FRIEND_H

#include <citro2d.h>

C2D_Image Friend_GameIcon;

char *Friend_GetScreenName(void);
char *Friend_GetFriendKey(void);
u64 Friend_GetFavouriteGame(void);
u8 *Friend_GetGameTitle(u64 titleId);
char *Friend_GetPlatform(u64 titleId);
C2D_Image Friend_LoadGameIcon(u64 titleId);
bool Friend_GetHideStatus(void);
bool Friend_GetHideTitleStatus(void);
bool Friend_IsFromFriendList(FriendKey *friendKeyList);
char *Friend_GetMyComment(void);
bool Friend_IsOnline(void);
void Friend_BackupFriendList(void);

#endif
