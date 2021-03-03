#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/aot_builtin.h"
#include "daScript/simulate/sim_policy.h"
#include "das_qsort_r.h"

namespace das
{
    struct AnySortContext {
        vec4f *     bargs;
        SimNode *   node;
        Context *   context;
    };

    void builtin_sort_any_cblock ( void * anyData, int32_t elementSize, int32_t length, const Block & cmp, Context * context ) {
        if ( length<=1 ) return;
        vec4f bargs[2];
        context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
            das_qsort_r(anyData, length, elementSize, [&](const void * x, const void * y){
              bargs[0] = cast<void *>::from(x);
              bargs[1] = cast<void *>::from(y);
              return code->evalBool(*context);
            });
        });
    }

    void builtin_sort_any_ref_cblock ( void * anyData, int32_t elementSize, int32_t length, const Block & cmp, Context * context ) {
        if ( length<=1 ) return;
        vec4f bargs[2];
        if ( elementSize <= 4 ) {
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                das_qsort_r(anyData, length, elementSize, [&](const void * x, const void * y){
                    bargs[0] = v_ldu_x((const float *)x);
                    bargs[1] = v_ldu_x((const float *)y);
                    return code->evalBool(*context);
                });
            });
        } else if ( elementSize <= 8 ) {
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                das_qsort_r(anyData, length, elementSize, [&](const void * x, const void * y){
                    bargs[0] = v_ldu_half(x);
                    bargs[1] = v_ldu_half(y);
                    return code->evalBool(*context);
                });
            });
        } else {
            context->invokeEx(cmp, bargs, nullptr, [&](SimNode * code) {
                das_qsort_r(anyData, length, elementSize, [&](const void * x, const void * y){
                    bargs[0] = v_ldu((const float *)x);
                    bargs[1] = v_ldu((const float *)y);
                    return code->evalBool(*context);
                });
            });
        }
    }

    void builtin_sort_string ( void * data, int32_t length ) {
        if ( length<=1 ) return;
        const char ** pdata = (const char **) data;
        sort ( pdata, pdata + length, [&](const char * a, const char * b){
            return strcmp(to_rts(a), to_rts(b))<0;
        });
    }

#define xstr(a) str(a)
#define str(a) #a

#define ADD_NUMERIC_SORT(CTYPE) \
    addExtern<DAS_BIND_FUN(builtin_sort<CTYPE>)>(*this, lib, "__builtin_sort", \
        SideEffects::modifyArgumentAndExternal, "builtin_sort<" xstr(CTYPE) ">"); \
    addExtern<DAS_BIND_FUN(builtin_sort_cblock<CTYPE>)>(*this, lib, "__builtin_sort_cblock", \
        SideEffects::modifyArgumentAndExternal, "builtin_sort_cblock<" xstr(CTYPE) ">");

#define ADD_VECTOR_SORT(CTYPE) \
    addExtern<DAS_BIND_FUN(builtin_sort_cblock<CTYPE>)>(*this, lib, "__builtin_sort_cblock", \
        SideEffects::modifyArgumentAndExternal, "builtin_sort_cblock<" xstr(CTYPE) ">");

    void Module_BuiltIn::addRuntimeSort(ModuleLibrary & lib) {
        // numeric
        ADD_NUMERIC_SORT(int32_t);
        ADD_NUMERIC_SORT(uint32_t);
        ADD_NUMERIC_SORT(int64_t);
        ADD_NUMERIC_SORT(uint64_t);
        ADD_NUMERIC_SORT(float);
        ADD_NUMERIC_SORT(double);
        // vector
        ADD_VECTOR_SORT(range);
        ADD_VECTOR_SORT(urange);
        ADD_VECTOR_SORT(int2);
        ADD_VECTOR_SORT(int3);
        ADD_VECTOR_SORT(int4);
        ADD_VECTOR_SORT(uint2);
        ADD_VECTOR_SORT(uint3);
        ADD_VECTOR_SORT(uint4);
        ADD_VECTOR_SORT(float2);
        ADD_VECTOR_SORT(float3);
        ADD_VECTOR_SORT(float4);
        // string
        addExtern<DAS_BIND_FUN(builtin_sort_string)>(*this, lib, "__builtin_sort_string",
            SideEffects::modifyArgumentAndExternal, "builtin_sort_string");
        addExtern<DAS_BIND_FUN(builtin_sort_cblock<char *>)>(*this, lib, "__builtin_sort_cblock",
            SideEffects::modifyArgumentAndExternal, "builtin_sort_cblock<char *>");
        // generic sort
        addExtern<DAS_BIND_FUN(builtin_sort_any_cblock)>(*this, lib, "__builtin_sort_any_cblock",
            SideEffects::modifyArgumentAndExternal, "builtin_sort_any_cblock");
        addExtern<DAS_BIND_FUN(builtin_sort_any_ref_cblock)>(*this, lib, "__builtin_sort_any_ref_cblock",
            SideEffects::modifyArgumentAndExternal, "builtin_sort_any_ref_cblock");
    }
}

