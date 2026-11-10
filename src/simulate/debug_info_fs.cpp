#include "daScript/misc/platform.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/misc/sysos.h"

namespace das {

    int64_t FileAccess::getFileMtime ( const string & fileName) const {
        uint64_t size; int64_t mtime; bool isDirectory;
        if ( !das_file_stat_utf8(fileName.c_str(), size, mtime, isDirectory) ) return -1;  // never key a cache on uninitialized stack
        return mtime;
    }

    int64_t FileAccess::getFileSize ( const string & fileName) const {
        uint64_t size; int64_t mtime; bool isDirectory;
        if ( !das_file_stat_utf8(fileName.c_str(), size, mtime, isDirectory) ) return -1;
        return int64_t(size);
    }
}
