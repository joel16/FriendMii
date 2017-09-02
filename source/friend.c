#include "frd.h"
#include "friend.h"
#include "screen.h"
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

static Result parseSMDH(u64 titleId, SMDH * smdh) 
{
	Handle handle;
	
	static const u32 filePath[] = {0x00000000, 0x00000000, 0x00000002, 0x6E6F6369, 0x00000000};
	u32 archivePath[] = {(u32) (titleId & 0xFFFFFFFF), (u32) ((titleId >> 32) & 0xFFFFFFFF), MEDIATYPE_SD, 0x00000000};
	FS_Path archiveBinPath = {PATH_BINARY, sizeof(archivePath), archivePath};
	FS_Path fileBinPath = {PATH_BINARY, sizeof(filePath), filePath};
	
	if (R_SUCCEEDED(FSUSER_OpenFileDirectly(&handle, ARCHIVE_SAVEDATA_AND_CONTENT, archiveBinPath, fileBinPath, FS_OPEN_READ, 0))) 
	{
		u32 bytesRead = 0;
		
		if (R_SUCCEEDED(FSFILE_Read(handle, &bytesRead, 0, smdh, sizeof(SMDH))) && bytesRead == sizeof(SMDH)) 
		{
			if (smdh->magic[0] == 'S' && smdh->magic[1] == 'M' && smdh->magic[2] == 'D' && smdh->magic[3] == 'H') 
				return FSFILE_Close(handle);
		}
		
		return FSFILE_Close(handle);
	}	
	
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
	SMDH smdh;
	
	static char shortDesc[0x41] = {'\0'};
	
	if (R_SUCCEEDED(parseSMDH(titleId, &smdh)))
		u16_to_u8(shortDesc, smdh.titles[getLanguage()].shortDescription, 0x41);
	
	return shortDesc;
}

char * Friend_GetGameDesc(u64 titleId)
{
	SMDH smdh;
	
	static char longDesc[0x81] = {'\0'};
	
	if (R_SUCCEEDED(parseSMDH(titleId, &smdh)))
		u16_to_u8(longDesc, smdh.titles[getLanguage()].longDescription, 0x81);
	
	return longDesc;
}

void Friend_GetGameIcon(u64 titleId)
{
	Handle handle;

	static const u32 filePath[5] = {0x00000000, 0x00000000, 0x00000002, 0x6E6F6369, 0x00000000};
	u32 archivePath[4] = {(u32) (titleId & 0xFFFFFFFF), (u32) ((titleId >> 32) & 0xFFFFFFFF), MEDIATYPE_SD, 0x00000000};
	FS_Path archiveBinPath = {PATH_BINARY, sizeof(archivePath), archivePath};
	FS_Path fileBinPath = {PATH_BINARY, sizeof(filePath), filePath};
	
	if (R_SUCCEEDED(FSUSER_OpenFileDirectly(&handle, ARCHIVE_SAVEDATA_AND_CONTENT, archiveBinPath, fileBinPath, FS_OPEN_READ, 0))) 
	{
		SMDH * smdh = (SMDH*) calloc(1, sizeof(SMDH));
		
		if (smdh != NULL) 
		{
			u32 bytesRead = 0;
		
			if (R_SUCCEEDED(FSFILE_Read(handle, &bytesRead, 0, smdh, sizeof(SMDH))) && bytesRead == sizeof(SMDH)) 
			{
				if (smdh->magic[0] == 'S' && smdh->magic[1] == 'M' && smdh->magic[2] == 'D' && smdh->magic[3] == 'H') 
				{
					screen_load_texture_tiled(TEXTURE_GAME_ICON, smdh->largeIcon, sizeof(smdh->largeIcon), 48, 48, GPU_RGB565, false);
				}
			}
		}	
		
		free(smdh);
	}
	
	FSFILE_Close(handle);
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