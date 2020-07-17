#include "daScript/misc/platform.h"
#include "daScript/daScriptModule.h"
#include "daScript/simulate/fs_file_info.h"

using namespace das;

smart_ptr<das::FileAccess> get_file_access( char * pak ) {
    if ( pak ) {
        return  make_smart<FsFileAccess>(pak, make_smart<FsFileAccess>());
    } else {
        return make_smart<FsFileAccess>();
    }
}

