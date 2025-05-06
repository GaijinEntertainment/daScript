#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

DAS_THREAD_LOCAL(daScriptEnvironment *, 0xa50e7db6) daScriptEnvironment::bound;
DAS_THREAD_LOCAL(daScriptEnvironment *, 0x3bb4532d) daScriptEnvironment::owned;

void daScriptEnvironment::ensure() {
    if ( !*daScriptEnvironment::bound ) {
        if ( !*daScriptEnvironment::owned ) {
            *daScriptEnvironment::owned = new daScriptEnvironment();
        }
        *daScriptEnvironment::bound = *daScriptEnvironment::owned;
    }
}

uint64_t getCancelLimit() {
    if ( !*daScriptEnvironment::bound ) return 0;
    return (*daScriptEnvironment::bound)->dataWalkerStringLimit;
}

}