#ifndef FRIEND_H
#define FRIEND_H

#include <3ds.h>

#define TEXTURE_GAME_ICON 5

typedef enum 
{
	PLATFORM_WII,
	PLATFORM_DSI,
	PLATFORM_3DS,
	PLATFORM_WIIU,
	PLATFORM_UNKNOWN
} AppPlatform;

typedef struct {
    u32 magic;
    u16 version;
    u16 reserved;
} smdhHeader_s;

typedef struct {
    u16 shortDescription[0x40];
    u16 longDescription[0x80];
    u16 publisher[0x40];
} smdhTitle_s;

typedef struct {
    u8 gameRatings[0x10];
    u32 regionLock;
    u8 matchMakerId[0xC];
    u32 flags;
    u16 eulaVersion;
    u16 reserved;
    u32 defaultFrame;
    u32 cecId;
} smdhSettings_s;

typedef struct {
    smdhHeader_s header;
    smdhTitle_s applicationTitles[16];
    smdhSettings_s settings;
    u8 reserved[0x8];
    u8 smallIconData[0x480];
    u16 bigIconData[0x900];
} smdh_s;

bool Friend_IsOnline(void);
char * Friend_GetFriendKey(void);
bool Friend_GetHideStatus(void);
bool Friend_GetHideTitleStatus(void);
char * Friend_GetScreenName(void);
char * Friend_GetMyComment(void);
u64 Friend_GetCurrentPlayingGame(void);
u64 Friend_GetFavouriteGame(void);
bool Friend_IsFromFriendList(FriendKey * friendKeyList);
char * Friend_GetGameTitle(u64 titleId);
char * Friend_GetGameDesc(u64 titleId);
void Friend_GetGameIcon(u64 titleId);
char * Friend_GetPlatform(u64 titleId);
bool Friend_IsValidFriendCode(u64 friendCode);

#endif