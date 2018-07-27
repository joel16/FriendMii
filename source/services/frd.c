#include <string.h>

#include "frd.h"

Handle frdHandle;
int frdRefCount;

Result FRD_GetFriendMii2(MiiStoreData *miiDataList, const FriendKey *friendKeyList, size_t size)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x14, 1, 4); // 0x140044
	cmdbuf[1] = size;
	cmdbuf[2] = (size << 18)|2;
	cmdbuf[3] = (u32)friendKeyList;
	cmdbuf[4] = 0x600 *size | 0xC;
	cmdbuf[5] = (u32)miiDataList;

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result FRD_GetFriendComment(wchar_t *comment, FriendKey *friendKeyList, size_t size)
{
    Result ret = 0;
    u32 *cmdbuf = getThreadCommandBuffer();
    cmdbuf[0] = 0x310082;
    cmdbuf[1] = 17 * size;
    cmdbuf[2] = size;
    cmdbuf[3] = ((size) << 18)|2;
    cmdbuf[4] = (u32)friendKeyList;
    
    u32 *staticbuf = getThreadStaticBuffers();
    staticbuf[0] = ((17 * size) << 15)|2;
    staticbuf[1] = (u32)comment;
    
    if (R_FAILED(ret = svcSendSyncRequest(frdHandle))) 
    	return ret;
    
    return (Result)cmdbuf[1];
}

Result FRD_GetMyPresence(MyPresence *myPresence)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();
	u32 *statbuf = getThreadStaticBuffers();

	cmdbuf[0] = IPC_MakeHeader(0x08, 0, 0); // 0x00080000
	statbuf[0] = 0x4B0002;
	statbuf[1] = (u32)myPresence;
  
	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result FRD_GetFriendPresence(FriendPresence* friendPresenceList, const FriendKey* friendKeyList, size_t size)
{
	Result ret;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x12, 1, 2); // 0x00120042
	cmdbuf[1] = size;
	cmdbuf[2] = (size << 18) | 2;
	cmdbuf[3] = (u32)friendKeyList;
	cmdbuf[64] = IPC_Desc_Buffer((size *sizeof(FriendPresence)), IPC_BUFFER_W);
	cmdbuf[65] = (u32)friendPresenceList;

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))return ret;

	return (Result)cmdbuf[1];
}

Result FRD_GetFriendAttributeFlags(AttributeFlag *attributeFlags, const FriendKey *friendKeyList, size_t size)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x17, 1, 2); // 0x00170042
	cmdbuf[1] = 0;
	cmdbuf[2] = (size << 18) | 2;
	cmdbuf[3] = (u32)friendKeyList;
	cmdbuf[4] = IPC_Desc_Buffer(size, IPC_BUFFER_W);
	cmdbuf[65] = (u32)attributeFlags;

	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result FRD_UpdateGameMode(const GameMode *gameMode, u16 *desc)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x1E, 0xB, 2); // 0x001E02C2
	memcpy(&cmdbuf[1], gameMode, 0x2C);
	cmdbuf[12] = 0x400802;
	cmdbuf[13] = (uintptr_t)desc;
  
	if (R_FAILED(ret = svcSendSyncRequest(frdHandle)))
		return ret;

	return (Result)cmdbuf[1];
}
