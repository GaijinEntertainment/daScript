#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das {
    void Module_BuiltIn::addMath(ModuleLibrary & lib) {
        // trigonometry functions
        addExtern<decltype(sinf),  sinf>(*this, lib, "sin", false);
        addExtern<decltype(cosf),  cosf>(*this, lib, "cos", false);
        addExtern<decltype(tanf),  tanf>(*this, lib, "tan", false);
        addExtern<decltype(atan2f), atan2f>(*this, lib, "atan2", false);

        // exp functions
        addExtern<decltype(powf),  powf >(*this, lib, "pow",  false);
        addExtern<decltype(expf),  expf >(*this, lib, "exp",  false);
        addExtern<decltype(logf),  logf >(*this, lib, "log",  false);
        addExtern<decltype(sqrtf), sqrtf>(*this, lib, "sqrt", false);

        //common
        addExtern<decltype(fabsf),  fabsf>(*this, lib,  "fabs",  false);
        addExtern<decltype(ceilf),  ceilf>(*this, lib,  "ceil",  false);
        addExtern<decltype(floorf), floorf>(*this, lib, "floor", false);
    }
}
