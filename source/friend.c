#include <stdio.h>
#include <stdlib.h>

#include "frd.h"
#include "friend.h"
#include "pp2d.h"
#include "utils.h"

bool Friend_IsOnline(void)
{
	bool isOnline = false;
	
	if (R_SUCCEEDED(FRDU_IsOnline(&isOnline)))
		return isOnline;
	
	return false;
}

char * Friend_GetFriendKey(void)
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

Profile Friend_GetProfile(void)
{
	Profile profile;
	
	if (R_SUCCEEDED(FRD_GetMyProfile(&profile)))
		return profile;
	
	return profile;
}

char * Friend_GetScreenName(void)
{
	u16 unsigned_screenName[FRIENDS_SCREEN_NAME_SIZE];
	static char screenName[FRIENDS_SCREEN_NAME_SIZE];
	
	if (R_SUCCEEDED(FRD_GetMyScreenName(unsigned_screenName)))
	{
		u16_to_u8(screenName, unsigned_screenName, FRIENDS_SCREEN_NAME_SIZE);
		return screenName;
	}
	
	return NULL;
}

char * Friend_GetMyComment(void)
{
	u16 unsigned_comment[FRIENDS_COMMENT_SIZE];
	static char comment[FRIENDS_COMMENT_SIZE];
	
	if (R_SUCCEEDED(FRD_GetMyComment(unsigned_comment)))
	{
		u16_to_u8(comment, unsigned_comment, FRIENDS_COMMENT_SIZE);
		return comment;
	}
	
	return NULL;
}

u64 Friend_GetCurrentPlayingGame(void)
{
	u64 titleId = 0;
	
	if (R_SUCCEEDED(FRD_GetMyPlayingGame(&titleId)))
		return titleId;
	
	return 0;
}

u64 Friend_GetFavouriteGame(void)
{
	u64 titleId = 0;
	
	if (R_SUCCEEDED(FRD_GetMyFavoriteGame(&titleId)))
		return titleId;
	
	return 0;
}

bool Friend_IsFromFriendList(FriendKey * friendKeyList)
{
	bool isFromList = false;
	
	if (R_SUCCEEDED(FRD_IsFromFriendList(friendKeyList, &isFromList)))
		return isFromList;
	
	return false;
}

static Result parseSMDH(u64 titleId, bool isGame, smdh_s * smdh) 
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

static CFG_Language getLanguage()
{
	CFG_Language language;
	CFGU_GetSystemLanguage(&language);
	
	return language;
}

char * Friend_GetGameTitle(u64 titleId)
{
	smdh_s smdh;
	
	static char shortDesc[0x41] = {'\0'};
	
	if (R_SUCCEEDED(parseSMDH(titleId, false, &smdh)))
		u16_to_u8(shortDesc, smdh.applicationTitles[getLanguage()].shortDescription, 0x41);
	
	return shortDesc;
}

char * Friend_GetGameDesc(u64 titleId)
{
	smdh_s smdh;
	
	static char longDesc[0x81] = {'\0'};
	
	if (R_SUCCEEDED(parseSMDH(titleId, false, &smdh)))
		u16_to_u8(longDesc, smdh.applicationTitles[getLanguage()].longDescription, 0x81);
	
	return longDesc;
}

void Friend_GetGameIcon(u64 titleId)
{
	smdh_s smdh;
	
	if (R_SUCCEEDED(parseSMDH(titleId, false, &smdh)))
	{
		static const u32 width = 48, height = 48;
		u32 *image = (u32*)malloc(width*height*sizeof(u32));

		for (u32 x = 0; x < width; x++)
		{
			for (u32 y = 0; y < height; y++)
			{
				unsigned int dest_pixel = (x + y*width);
				unsigned int source_pixel = (((y >> 3) * (width >> 3) + (x >> 3)) << 6) + ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3));

				u8 r = ((smdh.bigIconData[source_pixel] >> 11) & 0b11111) << 3;
				u8 g = ((smdh.bigIconData[source_pixel] >> 5) & 0b111111) << 2;
				u8 b = (smdh.bigIconData[source_pixel] & 0b11111) << 3;
				u8 a = 0xFF;

				image[dest_pixel] = (r << 24) | (g << 16) | (b << 8) | a;
			}
		}

		pp2d_load_texture_memory(TEXTURE_GAME_ICON, (u8*)image, (u32)width, (u32)height);

		free(image);
	}
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

char * Friend_GetPlatform(u64 titleId)
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

bool Friend_IsValidFriendCode(u64 friendCode)
{
	bool isValid = false;
	
	if (R_SUCCEEDED(FRD_IsValidFriendCode(friendCode, &isValid)))
		return isValid;
	
	return false;
}

size_t Friend_GetFriendColour(u16 colour)
{
	size_t id = -1;

	switch (colour)
	{
		case 0:
			id = TEXTURE_COLOUR_RED;
			break;
		case 1:
			id = TEXTURE_COLOUR_ORANGE;
			break;
		case 2:
			id = TEXTURE_COLOUR_YELLOW;
			break;
		case 3:
			id = TEXTURE_COLOUR_LIME_GREEN;
			break;
		case 4:
			id = TEXTURE_COLOUR_GREEN;
			break;
		case 5:
			id = TEXTURE_COLOUR_BLUE;
			break;
		case 6:
			id = TEXTURE_COLOUR_LIGHT_BLUE;
			break;
		case 7:
			id = TEXTURE_COLOUR_PINK;
			break;
		case 8:
			id = TEXTURE_COLOUR_PURPLE;
			break;
		case 9:
			id = TEXTURE_COLOUR_BROWN;
			break;
		case 10:
			id = TEXTURE_COLOUR_WHITE;
			break;
		case 11:
			id = TEXTURE_COLOUR_BLACK;
			break;
	}

	return id;
}