#ifndef FRD_H
#define FRD_H

#include <3ds.h>

#define FRIENDS_SCREEN_NAME_SIZE 0x16 // 11 (0x16 because UTF-16)
#define FRIENDS_COMMENT_SIZE 0x22 // 16 (0x21 because UTF-16 + null character)
#define FRIEND_LIST_SIZE 0x64 // 100
#define FRIEND_MII_STORE_DATA_SIZE 0x60 // 96

typedef struct
{
	u32 principalId;
	u64 localFriendCode;
} FriendKey;

typedef struct
{
   u32 joinAvailabilityFlag;
   u32 matchmakeSystemType;
   u32 joinGameId;
   u32 joinGameMode;
   u32 ownerPrincipalId;
   u32 joinGroupId;
   u8 applicationArg[0x14];
} GameMode;

typedef struct
{
   GameMode gameMode;
   wchar_t modeDescription[0x80];
} MyPresence;

typedef struct
{
    union 
	{
		u8 data[FRIEND_MII_STORE_DATA_SIZE]; 
		u32 value32[FRIEND_MII_STORE_DATA_SIZE / sizeof(u32)];
	};
} miiStoreData;

typedef struct
{
    u8 region;      // The region code for the hardware region.
    u8 country;     // Country code.
    u8 area;        // Area code.
    u8 language;    // Language code.
    u8 platform;    // Platform code.
} Profile;

Result frdInit(void);
void frdExit(void);
Result FRDU_IsOnline(bool * state);
Result FRDU_HasLoggedIn(bool * state);
Result FRD_Login(Handle event);
Result FRD_Logout(void);
Result FRD_GetMyFriendKey(FriendKey * key);
Result FRD_GetMyPreference(bool * isPublicMode, bool * isShowGameName, bool * isShowPlayedGame);
Result FRD_GetMyProfile(Profile * profile);
Result FRD_GetMyScreenName(u16 * name);
Result FRD_GetMyMii(miiStoreData * mii);
Result FRD_GetMyComment(u16 * comment);
Result FRD_GetMyPlayingGame(u64 * titleId);
Result FRD_GetMyFavoriteGame(u64 * titleId);
Result FRD_IsFromFriendList(u64 friendCode, bool * isFromList);
Result FRD_UpdateGameModeDescription(const wchar_t * desc);
Result FRD_PrincipalIdToFriendCode(u32 principalId, u64 * friendCode);
Result FRD_FriendCodeToPrincipalId(u64 friendCode, u32 * principalId);
Result FRD_IsValidFriendCode(u64 friendCode, bool * isValid);

#endif