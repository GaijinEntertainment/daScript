#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/aot_builtin.h"
#include "daScript/simulate/sim_policy.h"

namespace das
{
    struct AnySortContext {
        vec4f *     bargs;
        SimNode *   node;
        Context *   context;
    };

    __forceinline int anySortCmp ( void * ctx, const void * x, const void * y ) {
        AnySortContext * asc = (AnySortContext *) ctx;
        asc->bargs[0] = cast<void *>::from(x);
        asc->bargs[1] = cast<void *>::from(y);
        return asc->node->evalBool(*asc->context) ? -1 : 1;
    }

    void builtin_sort_any_cblock ( void * anyData, int32_t elementSize, int32_t length, const Block & cmp, Context * context ) {
        vec4f bargs[2];
        AnySortContext asc;
        asc.context = context;
        asc.bargs = bargs;
        context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
            asc.node = code;
#if defined(_MSC_VER)
            qsort_s(anyData, length, elementSize, &anySortCmp, &asc);
#else
            qsort_r(anyData, length, elementSize, &asc, &anySortCmp);
#endif
        });
    }

    void builtin_sort_string ( void * data, int32_t length ) {
        const char ** pdata = (const char **) data;
        sort ( pdata, pdata + length, [&](const char * a, const char * b){
            return strcmp(to_rts(a), to_rts(b))<0;
        });
    }

#define xstr(a) str(a)
#define str(a) #a

#define ADD_NUMERIC_SORT(CTYPE) \
    addExtern<DAS_BIND_FUN(builtin_sort<CTYPE>)>(*this, lib, "__builtin_sort", \
        SideEffects::modifyArgument, "builtin_sort<" xstr(CTYPE) ">"); \
    addExtern<DAS_BIND_FUN(builtin_sort_cblock<CTYPE>)>(*this, lib, "__builtin_sort_cblock", \
        SideEffects::modifyArgument, "builtin_sort_cblock<" xstr(CTYPE) ">");

    void Module_BuiltIn::addRuntimeSort(ModuleLibrary & lib) {
        // numeric
        ADD_NUMERIC_SORT(int32_t);
        ADD_NUMERIC_SORT(uint32_t);
        ADD_NUMERIC_SORT(int64_t);
        ADD_NUMERIC_SORT(uint64_t);
        ADD_NUMERIC_SORT(float);
        ADD_NUMERIC_SORT(double);
        // string
        addExtern<DAS_BIND_FUN(builtin_sort_string)>(*this, lib, "__builtin_sort_string", SideEffects::modifyArgument, "builtin_sort_string");
        addExtern<DAS_BIND_FUN(builtin_sort_cblock<char *>)>(*this, lib, "__builtin_sort_cblock",
            SideEffects::modifyArgument, "builtin_sort_cblock<char *>");
        // generic sort
        addExtern<DAS_BIND_FUN(builtin_sort_any_cblock)>(*this, lib, "__builtin_sort_any_cblock",
            SideEffects::modifyArgument, "builtin_sort_any_cblock");
    }
}

