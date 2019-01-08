#ifndef FRD_H
#define FRD_H

#include <3ds.h>

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

#define FRIENDS_SCREEN_NAME_SIZE 0x16 // 11 (0x16 because UTF-16)
#define FRIENDS_COMMENT_SIZE 0x22 // 16 (0x21 because UTF-16 + null character)
#define FRIEND_LIST_SIZE 0x64 // 100
#define FRIEND_MII_STORE_DATA_SIZE 0x60 // 96

extern Handle frdHandle;
extern int frdRefCount;

enum AttributeFlag {
   ATTRIBUTE_FLAG_ESTABLISHED = 1 << 0, //!< a friend relationship has been established at least once.
   ATTRIBUTE_FLAG_REMOTE_ACCESSIBLE = 1 << 1 //!< local host can access the resources of friends when the friends are online.
};

typedef enum AttributeFlag AttributeFlag;

typedef struct {
   u32 joinAvailabilityFlag;
   u32 matchmakeSystemType;
   u32 joinGameId;
   u32 joinGameMode;
   u32 ownerPrincipalId;
   u32 joinGroupId;
   u8 applicationArg[0x14];
} GameMode;

typedef struct {
   GameMode gameMode;
   u16 modeDescription[0x80];
} MyPresence;

typedef struct {
   GameMode gameMode;
   bool isOnline;
   bool isInvitingMe;
   bool isValid;
   u32 padding;                       
} FriendPresence;

Result FRD_GetFriendComment(wchar_t *comment, FriendKey *friendKeyList, size_t size);
Result FRD_GetMyPresence(MyPresence *myPresence);
Result FRD_GetFriendPresence(FriendPresence *FriendPresenceList, const FriendKey *friendKeyList, size_t size);
Result FRD_GetFriendAttributeFlags(AttributeFlag *attributeFlags, const FriendKey *friendKeyList, size_t size);
Result FRD_UpdateGameMode(const GameMode *gameMode, u16 *desc);

#endif
