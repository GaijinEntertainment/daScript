#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/aot_builtin.h"

namespace das
{

    template <typename TT>
    void builtin_sort_cblock ( TT * data, int32_t length, const TBlock<bool,TT,TT> & cmp, Context * context ) {
        vec4f bargs[2];
        context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
            sort ( data, data+length, [&](TT x, TT y) -> bool {
                bargs[0] = cast<TT>::from(x);
                bargs[1] = cast<TT>::from(y);
                return code->evalBool(*context);
            });
        });
    }

    void Module_BuiltIn::addRuntimeSort(ModuleLibrary & lib) {
        addExtern<DAS_BIND_FUN(builtin_sort<int32_t>)>(*this, lib, "builtin_sort", SideEffects::modifyArgument, "builtin_sort");

        addExtern<DAS_BIND_FUN(builtin_sort_cblock<int32_t>)>(*this, lib, "builtin_sort_cblock", SideEffects::modifyArgument, "builtin_sort_cblock");
    }
}

