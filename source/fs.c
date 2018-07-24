#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "utils.h"

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID archiveID)
{
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_OpenArchive(archive, archiveID, fsMakePath(PATH_EMPTY, ""))))
		return ret;

	return 0;
}

Result FS_CloseArchive(FS_Archive archive)
{
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_CloseArchive(archive)))
		return ret;

	return 0;
}

Result FS_MakeDir(FS_Archive archive, const char *path)
{	
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_CreateDirectory(archive, fsMakePath(PATH_UTF16, path_u16), 0)))
		return ret;
	
	return 0;
}

bool FS_FileExists(FS_Archive archive, const char *path)
{
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, path, strlen(path) + 1);

	if (R_FAILED(FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_UTF16, path_u16), FS_OPEN_READ, 0)))
		return false;

	if (R_FAILED(FSFILE_Close(handle)))
		return false;

	return true;
}

bool FS_DirExists(FS_Archive archive, const char *path)
{
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, path, strlen(path) + 1);

	if (R_FAILED(FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_UTF16, path_u16))))
		return false;

	if (R_FAILED(FSDIR_Close(handle)))
		return false;

	return true;
}

Result FS_Open(Handle *handle, FS_Archive archive, const char *path, u32 flags)
{
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_OpenFile(handle, archive, fsMakePath(PATH_UTF16, path_u16), flags, 0)))
		return ret;
	
	return 0;
}
