#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

thread_local daScriptEnvironment * daScriptEnvironment::bound = nullptr;
thread_local daScriptEnvironment * daScriptEnvironment::owned = nullptr;

void daScriptEnvironment::ensure() {
    if ( !daScriptEnvironment::bound ) {
        if ( !daScriptEnvironment::owned ) {
            daScriptEnvironment::owned = new daScriptEnvironment();
        }
        daScriptEnvironment::bound = daScriptEnvironment::owned;
    }
}

}