#pragma once

#include "daScript/misc/platform.h"

namespace das {
    class Module;
    DAS_API char * safe_pass_string ( const char * str );
    DAS_API void dasbind_set_library ( Module * dasbind, const char * alias, const char * libraries );
    DAS_API void dasbind_set_search_paths ( Module * dasbind, const vector<string> & paths );
}