#ifndef FRIENDMII_FS_H
#define FRIENDMII_FS_H

#include <3ds.h>

FS_Archive archive;

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID id);
Result FS_CloseArchive(FS_Archive archive);
Result FS_MakeDir(FS_Archive archive, const char *path);
bool FS_FileExists(FS_Archive archive, const char *path);
bool FS_DirExists(FS_Archive archive, const char *path);
Result FS_Open(Handle *handle, FS_Archive archive, const char *path, u32 flags);

#endif
