#ifndef FRD_H
#define FRD_H

#include <3ds.h>

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

#define FRIENDS_SCREEN_NAME_SIZE 0x16 // 11 (0x16 because UTF-16)
#define FRIENDS_COMMENT_SIZE 0x22 // 16 (0x21 because UTF-16 + null character)
#define FRIEND_LIST_SIZE 0x64 // 100
#define FRIEND_MII_STORE_DATA_SIZE 0x60 // 96

enum AttributeFlag
{
   ATTRIBUTE_FLAG_ESTABLISHED = 1 << 0, //!< a friend relationship has been established at least once.
   ATTRIBUTE_FLAG_REMOTE_ACCESSIBLE = 1 << 1 //!< local host can access the resources of friends when the friends are online.
};
typedef enum AttributeFlag AttributeFlag;

typedef struct
{
   u32 principalId;
   u32 padding;
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
   u16 modeDescription[0x80];
} MyPresence;

typedef struct
{
   GameMode gameMode;
   bool isOnline;
   bool isInvitingMe;
   bool isValid;
   u32 padding;                       
} FriendPresence;

typedef struct
{
   u64 titleId;
   u32 unk;
   u32 unk2;
} FriendGameKey;

typedef struct
{
   FriendGameKey key;
   wchar_t desc[128];
} FriendGameDescription;

typedef union MiiEyebrow_t 
{
   u32 raw;
   struct 
   {
        u32 rotation : 4;
        u32 _unk_bit_4 : 1;
        u32 xspacing : 4;
        u32 ypos : 5;
        u32 _unk_bit_14_15 : 2;
        u32 style : 5;
        u32 color : 3;
        u32 xscale : 4;
        u32 yscale : 4;
   };
} MiiEyebrow;

#pragma pack(1)
typedef struct 
{    
    u32 mii_id;
    u64 system_id;
    union {
        // This unsigned 32bit integer is stored in big-endian and holds the
        // date of creation in its lower 28 bit:
        //
        // seconds since 01/01/2010 00:00:00
        //   = (date_of_creation[bit 0 .. bit 27]) * 2
        u32 date_of_creation;

        // Non special Miis have bit 31 of aforementioned big-endian word set,
        // which corresponds to bit 8 in little endian, which the 3DS uses.
        struct {
            u32 : 7;
            u32 specialness : 1;
            u32 : 24;
        };
    };

    u8 mac[0x6];
    u16 padding;
    
    u16 gender : 1; // 0 = male, 1 = female
    u16 bday_month : 4;
    u16 bday_day : 5;
    u16 colour : 4; // 0 = Red, 1 = Orange, 2 = Yellow, 3 = Lime green, 4 = Green, 5 = Blue, 6 = Neon blue, 7 = Pink, 8 = Purple, 9 = Brown, 10 = White, and 11 = Black.
    u16 favorite : 1; // 0 = No, and 1 = Yes.
    u16 unk0 : 1;
    
    u16 mii_name[0xB];
    
    u8 width;
    u8 height;
    u32 misc2;
    u32 unknown1;
    u32 misc3;
    u32 unknown2;
    u8 allow_copy;
    u8 unknown3[0x7];
    u16 author[0xB];
} MiiStoreData;
#pragma pop

typedef struct
{
    u8 region;      // The region code for the hardware region.
    u8 country;     // Country code.
    u8 area;        // Area code.
    u8 language;    // Language code.
    u8 platform;    // Platform code.
    u32 padding;
} Profile;

typedef enum 
{
  self_online = 1,
  self_offline,
  friend_online,
  friend_presence,
  friend_mii,
  friend_profile,
  friend_offline,
  friend_became_friend,
  friend_invitaton
} FriendNotifType;

typedef struct
{
  u8 type;
  u8 padding3[3];
  u32 padding;
  FriendKey key;
} FriendNotifEvent;

Result frdInit(void);
void frdExit(void);
Result FRDU_HasLoggedIn(bool * state);
Result FRDU_IsOnline(bool * state);
Result FRD_Login(Handle event);
Result FRD_Logout(void);
Result FRD_GetMyFriendKey(FriendKey * key);
Result FRD_GetMyPreference(bool * isPublicMode, bool * isShowGameName, bool * isShowPlayedGame);
Result FRD_GetMyProfile(Profile * profile);
Result FRD_GetMyPresence(MyPresence * myPresence);
Result FRD_GetMyScreenName(u16 * name);
Result FRD_GetMyMii(MiiStoreData * mii);
Result FRD_GetMyPlayingGame(u64 * titleId);
Result FRD_GetMyFavoriteGame(u64 * titleId);
Result FRD_GetMyComment(u16 * comment);
Result FRD_GetFriendKeyList(FriendKey * friendKeyList, size_t * num, size_t offset, size_t size);
Result FRD_GetFriendPresence(FriendPresence* FriendPresenceList, const FriendKey* friendKeyList, size_t size);
Result FRD_GetFriendMii(MiiStoreData * miiDataList, const FriendKey * friendKeyList, size_t size);
Result FRD_GetFriendProfile(Profile * profileList, const FriendKey * friendKeyList, size_t size);
Result FRD_GetFriendAttributeFlags(AttributeFlag* attributeFlags, const FriendKey* friendKeyList, size_t size);
Result FRD_GetFriendPlayingGame(FriendGameDescription *desc, FriendKey * friendKeyList, size_t size);
Result FRD_GetFriendFavoriteGame(const FriendKey * friendKeyList, FriendGameKey * key, size_t size);
Result FRD_IsFromFriendList(FriendKey * friendKeyList, bool * isFromList);
Result FRD_UpdateGameModeDescription(u16 * desc);
Result FRD_UpdateGameMode(const GameMode *gameMode, u16 * desc);
Result FRD_PrincipalIdToFriendCode(u32 principalId, u64 * friendCode);
Result FRD_FriendCodeToPrincipalId(u64 friendCode, u32 * principalId);
Result FRD_IsValidFriendCode(u64 friendCode, bool * isValid);
Result FRD_GetFriendComment(wchar_t * comment, FriendKey *friendKeyList, size_t size);
Result FRD_SetClientSdkVersion(u32 sdkVer);
Result FRD_GetEventNotification(FriendNotifEvent *event, size_t size, u32 *count);
Result FRD_AttachToEventNotification(Handle event);
Result FRD_RemoveFriend(u32 principalId, u64 friendCode);
Result FRD_AddFriendOnline(Handle event, u32 principalId);

#endif