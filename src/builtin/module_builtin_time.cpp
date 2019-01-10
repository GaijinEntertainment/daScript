#include "daScript/misc/platform.h"

#include "module_builtin.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"

#include <ctime>

namespace das {
    
    int64_t builtin_clock() {
        return time(nullptr);
    }
    
    char * builtin_clockToString( int64_t t, Context * context ) {
        stringstream ss;
        time_t T = t;
        ss << std::asctime(std::localtime(&T));
        return context->allocateName(ss.str());
    }
    
    void Module_BuiltIn::addTime(ModuleLibrary & lib) {
        addExtern<decltype(builtin_clock),builtin_clock>(*this, lib, "getClock");
        addExtern<decltype(builtin_clockToString),builtin_clockToString>(*this, lib, "string", false);
    }
}
