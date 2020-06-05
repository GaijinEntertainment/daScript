#include "daScript/misc/platform.h"
#include "daScript/daScriptModule.h"
#include "daScript/simulate/fs_file_info.h"

using namespace das;

void require_project_specific_modules() {
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_UriParser);
}

smart_ptr<das::FileAccess> get_file_access() {
    return make_smart<FsFileAccess>();
}

