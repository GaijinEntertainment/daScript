#include "daScript/misc/utf8_stdio.h"
#include "daScript/misc/utf8_conv.h"
#include <string>

#if defined(_MSC_VER)
#    define NOMINMAX
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#else
#    include <limits.h>
#    include <stdlib.h>
#endif


#if !defined(DAS_NO_FILEIO)

#include <sys/stat.h>

#if defined(_MSC_VER)
#    include <io.h>
#    include <direct.h>
#else
#    include <libgen.h>
#    include <dirent.h>
#    include <unistd.h>
#    include <sys/mman.h>
#endif

namespace das {

    FILE * fopen_utf8(const char * name, const char * mode, bool share_deny_write) {
        if (!name || !mode)
            return nullptr;
    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(name);
        wstring wMode = utf8_to_wstring(mode);
        return _wfsopen(wName.c_str(), wMode.c_str(), share_deny_write ? _SH_DENYWR : _SH_DENYNO);
    #else
        (void)share_deny_write;
        FILE * f = fopen(file_name_utf8, mode);
        // ? flock(f, LOCK_EX);
        return f;
    #endif
    }


    FILE * popen_utf8(const char * command) {
        if (!command)
            return nullptr;
    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(command);
        return _wpopen(wName.c_str(), L"rt");
    #elif defined(__linux__)
        return popen(cmd, "r");
    #else
        return popen(cmd, "r+");
    #endif
    }


    //  TODO: fgets_utf8()

    const char * dirname_utf8(const char * name, Context * context) { // allocation in context->stringHeap
        string res = dirname_utf8(name);
        return context->stringHeap->allocateString(res.c_str(), uint32_t(res.length()));
    }


    string dirname_utf8(const char * name) {
        if (!name)
            return string();

    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(name);
        wchar_t full_path[_MAX_PATH];
        wchar_t dir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];
        _wsplitpath(wName.c_str(), full_path, dir, fname, ext);
        wcscat(full_path, dir);
        uint32_t len = uint32_t(wcslen(full_path));
        if (len) {
            if (full_path[len - 1] == L'/' || full_path[len - 1] == L'\\') {
                full_path[--len] = 0;
            }
        }
        return wstring_to_utf8(full_path);
    #else
        char * tempName = strdup(name);
        char * dirName = dirname(tempName);
        string res(dirName);
        free(tempName);
        return res;
    #endif
    }


    const char * basename_utf8(const char * path, Context * context) { // allocation in context->stringHeap
        string res = basename_utf8(path);
        return context->stringHeap->allocateString(res.c_str(), uint32_t(res.length()));
    }


    string basename_utf8(const char * path) {
    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(path);
        wchar_t drive[_MAX_DRIVE];
        wchar_t full_path[_MAX_PATH];
        wchar_t dir[_MAX_DIR];
        wchar_t ext[_MAX_EXT];
        _wsplitpath(wName.c_str(), drive, dir, full_path, ext);
        wcscat(full_path, ext);
        return wstring_to_utf8(full_path);
    #else
        char * tempName = strdup(path);
        char * dirName = basename(tempName);
        char * result = context->stringHeap->allocateString(dirName, strlen(dirName));
        free(tempName);
        return result;
    #endif
    }


#if defined(_MSC_VER)
    bool stat_utf8(const char * name, struct _stati64 * buffer) {
        if (!name)
            return false;
        wstring wName = utf8_to_wstring(name);
        return _wstati64(wName.c_str(), buffer) == 0;
    }
#else
    bool stat_utf8(const char * name, struct stat * buffer) {
        return stat(name, buffer) == 0;
    }
#endif



    int dir_utf8(const char * path, DirCallback callback, void * user_pointer) {
        if (!path)
            return 0;

        int count = 0;

    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(path);
        _wfinddata_t c_file;
        intptr_t hFile;
        wstring findPath = wName + L"/*";
        if ((hFile = _wfindfirst(findPath.c_str(), &c_file)) != -1L) {
            do {
                count++;
                string name = wstring_to_utf8(c_file.name);
                if (callback)
                    callback(name.c_str(), user_pointer);
            } while (_wfindnext(hFile, &c_file) == 0);
        }
        _findclose(hFile);
    #else
        DIR * dir;
        struct dirent * ent;
        if ((dir = opendir(path)) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                count++;
                if (callback)
                    callback(ent->d_name, user_pointer);
                };
            }
            closedir(dir);
        }
    #endif
        return count;
    }


    int mkdir_utf8(const char * path) {
        if (!path)
            return -1;
    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(path);
        return _wmkdir(wName.c_str());
    #else
        return mkdir(path, ACCESSPERMS);
    #endif
    }


    bool unlink_file_utf8(const char * file_name) {
        if (!file_name)
            return false;

    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(file_name);
        return DeleteFileW(wName.c_str()) != 0;
    #else
        return unlink(file_name) == 0;
    #endif
    }


    bool rename_file_utf8(const char * file_from, const char * file_dest) {
        if (!file_from || !file_dest)
            return false;

    #if defined(_MSC_VER)
        wstring wNameFrom = utf8_to_wstring(file_from);
        wstring wNameTo = utf8_to_wstring(file_dest);
        return MoveFileW(wNameFrom.c_str(), wNameTo.c_str()) != 0;
    #else
        return rename(file_from, file_dest) == 0;
    #endif
    }

}

#endif  // !defined(DAS_NO_FILEIO)


namespace das {

    const char * get_full_path_name_utf8(const char * path, Context * context) { // allocation in context->stringHeap
        string res = get_full_path_name_utf8(path);
        return context->stringHeap->allocateString(res.c_str(), uint32_t(res.length()));
    }


    string get_full_path_name_utf8(const char * path) {
        if (!path)
            return string();
    #if defined(_MSC_VER)
        wstring wName = utf8_to_wstring(path);
        wchar_t buf[512] = { 0 };
        if (GetFullPathNameW(wName.c_str(), 512, buf, nullptr) == 0)
            return string(path);
        return wstring_to_utf8(buf);
    #else
        char resolvedPath[PATH_MAX] = { 0 };
        realpath(path, resolvedPath);
        return string(resolvedPath);
    #endif
    }

}
