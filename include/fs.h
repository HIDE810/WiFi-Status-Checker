#include <3ds.h>

#ifndef FS_H
#define FS_H

FS_Archive archive, sdmc_archive;

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID id);
Result FS_CloseArchive(FS_Archive archive);
Result FS_OpenFile(Handle *handle, FS_Archive archive, const char *path, u32 flags, u32 attributes);
Result FS_MakeDir(FS_Archive archive, const char *path);
Result FS_CreateFile(FS_Archive archive, const char *path);
Result FS_RecursiveMakeDir(FS_Archive archive, const char *dir);
bool FS_FileExists(FS_Archive archive, const char *path);
bool FS_DirExists(FS_Archive archive, const char *path);
Result FS_RemoveFile(FS_Archive archive, const char *path);
Result FS_Write(FS_Archive archive, const char *path, const char *buf);

#endif