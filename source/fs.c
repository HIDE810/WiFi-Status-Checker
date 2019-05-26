#include "fs.h"
#include "utils.h"

Result FS_OpenArchive(FS_Archive *archive, FS_ArchiveID archiveID) {
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_OpenArchive(archive, archiveID, fsMakePath(PATH_EMPTY, ""))))
		return ret;

	return 0;
}

Result FS_CloseArchive(FS_Archive archive) {
	Result ret = 0;

	if (R_FAILED(ret = FSUSER_CloseArchive(archive)))
		return ret;

	return 0;
}

Result FS_OpenFile(Handle *handle, FS_Archive archive, const char *path, u32 flags, u32 attributes) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_OpenFile(handle, archive, fsMakePath(PATH_UTF16, path_u16), flags, attributes)))
		return ret;
	
	return 0;
}

Result FS_MakeDir(FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_CreateDirectory(archive, fsMakePath(PATH_UTF16, path_u16), 0)))
		return ret;
	
	return 0;
}

Result FS_CreateFile(FS_Archive archive, const char *path) {
	Result ret = 0;
	
	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_CreateFile(archive, fsMakePath(PATH_UTF16, path_u16), 0, 0)))
		return ret;
	
	return 0;
}

Result FS_RecursiveMakeDir(FS_Archive archive, const char *dir) {
	Result ret = 0;
	char buf[256];
	char *p = NULL;
	size_t len;

	snprintf(buf, sizeof(buf), "%s",dir);
	len = strlen(buf);

	if (buf[len - 1] == '/')
		buf[len - 1] = 0;

	for (p = buf + 1; *p; p++) {
		if (*p == '/') {
			*p = 0;

			if (!FS_DirExists(archive, buf))
				ret = FS_MakeDir(archive, buf);
			
			*p = '/';
		}
		
		if (!FS_DirExists(archive, buf))
			ret = FS_MakeDir(archive, buf);
	}
	
	return ret;
}

bool FS_FileExists(FS_Archive archive, const char *path) {
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_UTF16, path_u16), FS_OPEN_READ, 0)))
		return false;

	if (R_FAILED(FSFILE_Close(handle)))
		return false;

	return true;
}

bool FS_DirExists(FS_Archive archive, const char *path) {
	Handle handle;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_UTF16, path_u16))))
		return false;

	if (R_FAILED(FSDIR_Close(handle)))
		return false;

	return true;
}

Result FS_RemoveFile(FS_Archive archive, const char *path) {
	Result ret = 0;

	u16 path_u16[strlen(path) + 1];
	Utils_U8_To_U16(path_u16, (const u8 *)path, strlen(path) + 1);

	if (R_FAILED(ret = FSUSER_DeleteFile(archive, fsMakePath(PATH_UTF16, path_u16))))
		return ret;
	
	return 0;
}

Result FS_Write(FS_Archive archive, const char *path, const char *buf) {
	Result ret = 0;
	Handle handle;
	
	u32 len = strlen(buf);
	u64 size = 0;
	u32 bytes_written = 0;

	if (FS_FileExists(archive, path))
		FS_RemoveFile(archive, path);

	if (R_FAILED(ret = FS_OpenFile(&handle, archive, path, (FS_OPEN_WRITE | FS_OPEN_CREATE), 0)))
		return ret;

	if (R_FAILED(ret = FSFILE_GetSize(handle, &size))) {
		FSFILE_Close(handle);
		return ret;
	}

	if (R_FAILED(ret = FSFILE_SetSize(handle, size + len))) {
		FSFILE_Close(handle);
		return ret;
	}

	if (R_FAILED(ret = FSFILE_Write(handle, &bytes_written, size, buf, len, FS_WRITE_FLUSH))) {
		FSFILE_Close(handle);
		return ret;
	}

	if (R_FAILED(ret = FSFILE_Close(handle)))
		return ret;

	return 0;
}