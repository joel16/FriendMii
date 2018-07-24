#ifndef FRD_H
#define FRD_H

#include <3ds.h>

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

#define FRIENDS_SCREEN_NAME_SIZE 0x16 // 11 (0x16 because UTF-16)
#define FRIENDS_COMMENT_SIZE 0x22 // 16 (0x21 because UTF-16 + null character)
#define FRIEND_LIST_SIZE 0x64 // 100
#define FRIEND_MII_STORE_DATA_SIZE 0x60 // 96

Handle frdHandle;
int frdRefCount;

#pragma pack(1)
typedef struct 
{    
    u32 mii_id;
    u64 system_id;
    u32 cdate;

    u8 mac[0x6];
    u16 padding;
    
    u16 gender : 1; // 0 = male, 1 = female
    u16 bday_month : 4;
    u16 bday_day : 5;
    u16 colour : 4; // 0 = Red, 1 = Orange, 2 = Yellow, 3 = Lime green, 4 = Green, 5 = Blue, 6 = Neon blue, 7 = Pink, 8 = Purple, 9 = Brown, 10 = White, and 11 = Black.
    u16 favorite : 1; // 0 = No, and 1 = Yes.
    u16 _unk_0x19 : 1;
    
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

enum AttributeFlag
{
   ATTRIBUTE_FLAG_ESTABLISHED = 1 << 0, //!< a friend relationship has been established at least once.
   ATTRIBUTE_FLAG_REMOTE_ACCESSIBLE = 1 << 1 //!< local host can access the resources of friends when the friends are online.
};
typedef enum AttributeFlag AttributeFlag;

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

Result FRD_GetFriendMii2(MiiStoreData *miiDataList, const FriendKey *friendKeyList, size_t size);
Result FRD_GetFriendComment(wchar_t *comment, FriendKey *friendKeyList, size_t size);
Result FRD_GetMyPresence(MyPresence *myPresence);
Result FRD_GetFriendPresence(FriendPresence *FriendPresenceList, const FriendKey *friendKeyList, size_t size);
Result FRD_GetFriendAttributeFlags(AttributeFlag *attributeFlags, const FriendKey *friendKeyList, size_t size);
Result FRD_UpdateGameMode(const GameMode *gameMode, u16 *desc);

#endif