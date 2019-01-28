#include "daScript/misc/platform.h"

#include "module_builtin.h"

namespace das {
    #include "builtin.das.inc"
    bool Module_BuiltIn::appendCompiledFunctions() {
        return compileBuiltinModule(builtin_das, builtin_das_len);
    }
}
