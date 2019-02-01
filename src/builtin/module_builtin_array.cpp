#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

#include "daScript/simulate/runtime_array.h"

namespace das {

    int builtin_array_size ( const Array * arr ) {
        return arr->size;
    }

    int builtin_array_capacity ( const Array * arr ) {
        return arr->capacity;
    }

    void builtin_array_resize ( Array * pArray, int newSize, int stride, Context * context ) {
        array_resize ( *context, *pArray, newSize, stride, true );
    }

    void builtin_array_reserve ( Array * pArray, int newSize, int stride, Context * context ) {
        array_reserve( *context, *pArray, newSize, stride );
    }

    int builtin_array_push ( Array * pArray, int index, int stride, Context * context ) {
        uint32_t idx = pArray->size;
        array_resize(*context, *pArray, idx + 1, stride, false);
        if ( context->stopFlags & EvalFlags::stopForThrow ) {
            return 0;
        }
        if ( index >=0 ) {
            if ( uint32_t(index) >= pArray->size ) {
                context->throw_error("insert index out of range");
                return 0;
            }
            memmove ( pArray->data+(index+1)*stride, pArray->data+index*stride, (idx-index)*stride );
            idx = index;
        }
        return idx;
    }

    void builtin_array_erase ( Array * pArray, int index, int stride, Context * context ) {
        if ( uint32_t(index) >= pArray->size ) {
            context->throw_error("erase index out of range");
            return;
        }
        memmove ( pArray->data+index*stride, pArray->data+(index+1)*stride, (pArray->size-index-1)*stride );
        array_resize(*context, *pArray, pArray->size-1, stride, false);
    }

    void builtin_array_clear ( Array * pArray, Context * context ) {
        array_clear(*context, *pArray);
    }

    void Module_BuiltIn::addArrayTypes(ModuleLibrary & lib) {
        // array functions
        addExtern<DAS_BIND_FUN(builtin_array_clear)>(*this, lib, "clear", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_array_size)>(*this, lib, "length", SideEffects::none);
        addExtern<DAS_BIND_FUN(builtin_array_capacity)>(*this, lib, "capacity", SideEffects::none);
        // array built-in functions
        addExtern<DAS_BIND_FUN(builtin_array_resize)>(*this, lib, "__builtin_array_resize", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_array_reserve)>(*this, lib, "__builtin_array_reserve", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_array_push)>(*this, lib, "__builtin_array_push", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_array_erase)>(*this, lib, "__builtin_array_erase", SideEffects::modifyExternal);
    }
}
