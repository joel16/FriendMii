#ifndef FRIEND_H
#define FRIEND_H

#define TEXTURE_GAME_ICON 6

typedef enum 
{
	PLATFORM_WII,
	PLATFORM_DSI,
	PLATFORM_3DS,
	PLATFORM_WIIU,
	PLATFORM_UNKNOWN
} AppPlatform;

typedef struct 
{
	u16 shortDescription[0x40];
	u16 longDescription[0x80];
	u16 publisher[0x40];
} SMDH_title;

typedef struct 
{
	char magic[0x04];
	u16 version;
	u16 reserved1;
	SMDH_title titles[0x10];
	u8 ratings[0x10];
	u32 region;
	u32 matchMakerId;
	u64 matchMakerBitId;
	u32 flags;
	u16 eulaVersion;
	u16 reserved;
	u32 optimalBannerFrame;
	u32 streetpassId;
	u64 reserved2;
	u8 smallIcon[0x480];
	u8 largeIcon[0x1200];
} SMDH;

bool Friend_IsOnline(void);
char * Friend_GetFriendKey(void);
bool Friend_GetHideStatus(void);
bool Friend_GetHideTitleStatus(void);
char * Friend_GetScreenName(void);
char * Friend_GetMyComment(void);
u64 Friend_GetCurrentPlayingGame(void);
u64 Friend_GetFavouriteGame(void);
bool Friend_IsFromFriendList(u64 friendCode);
char * Friend_GetGameTitle(u64 titleId);
char * Friend_GetGameDesc(u64 titleId);
void Friend_GetGameIcon(u64 titleId);
char * Friend_GetPlatform(u64 titleId);
bool Friend_IsValidFriendCode(u64 friendCode);

#endif