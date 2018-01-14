#ifndef FS_H
#define FS_H

#include <3ds.h>

FS_Archive archive;

Result FS_OpenArchive(FS_Archive * archive, FS_ArchiveID id);
Result FS_CloseArchive(FS_Archive archive);
Result FS_MakeDir(FS_Archive archive, const char * path);
void FS_RecursiveMakeDir(FS_Archive archive, const char * dir);
bool FS_FileExists(FS_Archive archive, const char * path);
bool FS_DirExists(FS_Archive archive, const char * path);

#endif