#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>

#include "friend.h"
#include "keyboard.h"
#include "log.h"
#include "utils.h"

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

char *Friend_GetScreenName(void)
{
	static char screen_name[FRIENDS_SCREEN_NAME_SIZE + 1];

	if (R_SUCCEEDED(FRD_GetMyScreenName(screen_name, FRIENDS_SCREEN_NAME_SIZE)))
		return screen_name;
	
	return NULL;
}

char *Friend_GetFriendKey(void)
{
	FriendKey key;
	u64 localFriendCode = 0;
	int bytes[0x3];
	static char friendKey[0x13];
	
	if (R_SUCCEEDED(FRD_GetMyFriendKey(&key)))
	{
		if (R_SUCCEEDED(FRD_PrincipalIdToFriendCode(key.principalId, &localFriendCode)))
		{
			bytes[0] = localFriendCode / 100000000;
			bytes[1] = (localFriendCode % 100000000) / 10000;
			bytes[2] = localFriendCode % 10000;
			
			snprintf(friendKey, 0x13, "%04d - %04d - %04d", bytes[0], bytes[1], bytes[2]);
		}
	}
	
	return friendKey;
}

u64 Friend_GetFavouriteGame(void)
{
	u64 titleId = 0;
	
	if (R_SUCCEEDED(FRD_GetMyFavoriteGame(&titleId)))
		return titleId;
	
	return 0;
}

static Result Friend_ParseSMDH(u64 titleId, bool isGame, smdh_s *smdh) 
{
	Handle handle;
	Result ret = 0;

	FS_MediaType mediaType = 0;
	
	if (((titleId / 1000000000) % 10) == 9)
		mediaType = MEDIATYPE_SD;
	else if (((titleId / 1000000000) % 10) == 8)
		mediaType = MEDIATYPE_NAND;

	if (isGame)
		mediaType = MEDIATYPE_GAME_CARD;

	u32 archPath[] = {(u32)titleId, (u32)(titleId >> 32), mediaType, 0x0};
	static const u32 filePath[] = {0x0, 0x0, 0x2, 0x6E6F6369, 0x0};

	FS_Path binArchPath = {PATH_BINARY, 0x10, archPath};
	FS_Path binFilePath = {PATH_BINARY, 0x14, filePath};

	if (R_SUCCEEDED(ret = FSUSER_OpenFileDirectly(&handle, ARCHIVE_SAVEDATA_AND_CONTENT, binArchPath, binFilePath, FS_OPEN_READ, 0)))
	{
		u32 bytesRead = 0;

		if (R_SUCCEEDED(ret = FSFILE_Read(handle, &bytesRead, 0, smdh, sizeof(smdh_s))))
			return FSFILE_Close(handle);
		else 
			return ret;
	}
	else
		return ret;

	return 0;
}

static CFG_Language Friend_GetLanguage()
{
	CFG_Language language;
	CFGU_GetSystemLanguage(&language);
	
	return language;
}

char *Friend_GetGameTitle(u64 titleId)
{
	smdh_s smdh;
	
	static char shortDesc[0x41] = {'\0'};
	
	if (R_SUCCEEDED(Friend_ParseSMDH(titleId, false, &smdh)))
		Utils_U16_To_U8(shortDesc, smdh.applicationTitles[Friend_GetLanguage()].shortDescription, 0x41);
	
	return shortDesc;
}

static AppPlatform platformFromId(u16 id)
{
	switch(id) 
	{
		case 1:
			return PLATFORM_WII;
		case 3:
			return PLATFORM_DSI;
		case 4:
			return PLATFORM_3DS;
		case 5:
			return PLATFORM_WIIU;
		default:
			return PLATFORM_UNKNOWN;
    }
}

char *Friend_GetPlatform(u64 titleId)
{
	AppPlatform platform = platformFromId(((u16*) &titleId)[3]);
	
	switch(platform)
	{
		case PLATFORM_WII:
			return "for Nintendo Wii";
		case PLATFORM_DSI:
			return "for Nintendo DSi";
		case PLATFORM_3DS:
			return "for Nintendo 3DS";
		case PLATFORM_WIIU:
			return "for Nintendo Wii U";
		default:
			return "Unknown";
	}
}

// This function is written by bernardo: https://github.com/BernardoGiordano/Checkpoint/blob/master/3ds/source/title.cpp#L644
C2D_Image Friend_LoadGameIcon(u64 titleId)
{
	smdh_s smdh;
	C2D_Image image;

	if (R_SUCCEEDED(Friend_ParseSMDH(titleId, false, &smdh)))
	{
		C3D_Tex* tex = (C3D_Tex*)malloc(sizeof(C3D_Tex));
		static const Tex3DS_SubTexture subt3x = { 48, 48, 0.0f, 48 / 64.0f, 48 / 64.0f, 0.0f };
		image = (C2D_Image){ tex, &subt3x };
		C3D_TexInit(image.tex, 64, 64, GPU_RGB565);
		u16 *dest = (u16*)image.tex->data + (64 - 48) * 64;
		u16 *src = smdh.bigIconData;

		for (int j = 0; j < 48; j += 8)
		{
			memcpy(dest, src, 48 * 8 * sizeof(u16));
			src += 48 * 8;
			dest += 64 * 8;
		}
	}

	return image;
}

bool Friend_GetHideStatus(void)
{
	bool isPublicMode = false, isShowGameName = false, isShowPlayedGame = false;
	
	if (R_SUCCEEDED(FRD_GetMyPreference(&isPublicMode, &isShowGameName, &isShowPlayedGame)))
		return isPublicMode;
	
	return false;
}

bool Friend_GetHideTitleStatus(void)
{
	bool isPublicMode = false, isShowGameName = false, isShowPlayedGame = false;
	
	if (R_SUCCEEDED(FRD_GetMyPreference(&isPublicMode, &isShowGameName, &isShowPlayedGame)))
		return isShowGameName;
	
	return false;
}

bool Friend_IsFromFriendList(FriendKey *friendKeyList)
{
	bool isFromList = false;

	if (R_SUCCEEDED(FRD_IsFromFriendList(friendKeyList, &isFromList)))
		return isFromList;

	return false;
}

char *Friend_GetMyComment(void)
{
	static char comment[FRIENDS_COMMENT_SIZE];

	if (R_SUCCEEDED(FRD_GetMyComment(comment, FRIENDS_COMMENT_SIZE)))
		return comment;

	return NULL;
}

bool Friend_IsOnline(void)
{
	bool isOnline = false;
	
	if (R_SUCCEEDED(FRDU_IsOnline(&isOnline)))
		return isOnline;
	
	return false;
}
