#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

#include "daScript/simulate/aot_builtin.h"

namespace das {

    int builtin_array_size ( const Array & arr ) {
        return arr.size;
    }

    int builtin_array_capacity ( const Array & arr ) {
        return arr.capacity;
    }

    int builtin_array_lock_count ( const Array & arr ) {
        return arr.lock;
    }

    void builtin_array_resize ( Array & pArray, int newSize, int stride, Context * context ) {
        if ( newSize<0 ) context->throw_error_ex("resizing array to negative size %i", newSize);
        array_resize ( *context, pArray, newSize, stride, true );
    }

    void builtin_array_reserve ( Array & pArray, int newSize, int stride, Context * context ) {
        if ( newSize<0 ) context->throw_error_ex("reserving array to negative size %i", newSize);
        array_reserve( *context, pArray, newSize, stride );
    }

    void builtin_array_erase ( Array & pArray, int index, int stride, Context * context ) {
        if ( uint32_t(index) >= pArray.size ) {
            context->throw_error_ex("erase index out of range, %u of %u", uint32_t(index), pArray.size);
            return;
        }
        memmove ( pArray.data+index*stride, pArray.data+(index+1)*stride, (pArray.size-index-1)*stride );
        array_resize(*context, pArray, pArray.size-1, stride, false);
    }

    void builtin_array_erase_range ( Array & pArray, int index, int count, int stride, Context * context ) {
        if ( index < 0 || count < 0 || uint32_t(index + count) > pArray.size ) {
            context->throw_error_ex("erasing array range is invalid: index=%i count=%i size=%u", index, count, pArray.size);
            return;
        }
        memmove ( pArray.data+index*stride, pArray.data+(index+count)*stride, (pArray.size-index-count)*stride );
        array_resize(*context, pArray, pArray.size-count, stride, false);
    }

    void builtin_array_clear ( Array & pArray, Context * context ) {
        array_clear(*context, pArray);
    }

    void builtin_array_lock ( const Array & arr, Context * context ) {
        array_lock(*context, const_cast<Array&>(arr));
    }

    void builtin_array_unlock ( const Array & arr, Context * context ) {
        array_unlock(*context, const_cast<Array&>(arr));
    }

    void builtin_array_clear_lock ( const Array & arr, Context * ) {
        const_cast<Array&>(arr).hopeless = true;
    }

    void Module_BuiltIn::addArrayTypes(ModuleLibrary & lib) {
        // array functions
        addExtern<DAS_BIND_FUN(builtin_array_clear)>(*this, lib, "clear",
            SideEffects::modifyArgument, "builtin_array_clear")
                ->args({"array","context"});
        addExtern<DAS_BIND_FUN(builtin_array_size)>(*this, lib, "length",
            SideEffects::none, "builtin_array_size")
                ->arg("array");
        addExtern<DAS_BIND_FUN(builtin_array_capacity)>(*this, lib, "capacity",
            SideEffects::none, "builtin_array_capacity")
                ->arg("array");
        addExtern<DAS_BIND_FUN(builtin_array_lock_count)>(*this, lib, "lock_count",
            SideEffects::none, "builtin_array_lock_count")
                ->arg("array");
        // array built-in functions
        addExtern<DAS_BIND_FUN(builtin_array_resize)>(*this, lib, "__builtin_array_resize",
            SideEffects::modifyArgument, "builtin_array_resize")
                ->args({"array","newSize","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_reserve)>(*this, lib, "__builtin_array_reserve",
            SideEffects::modifyArgument, "builtin_array_reserve")
                ->args({"array","newSize","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_push)>(*this, lib, "__builtin_array_push",
            SideEffects::modifyArgument, "builtin_array_push")
                ->args({"array","index","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_push_zero)>(*this, lib, "__builtin_array_push_zero",
            SideEffects::modifyArgument, "builtin_array_push_zero")
                ->args({"array","index","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_push_back)>(*this, lib, "__builtin_array_push_back",
            SideEffects::modifyArgument, "builtin_array_push_back")
                ->args({"array","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_push_back_zero)>(*this, lib, "__builtin_array_push_back_zero",
            SideEffects::modifyArgument, "builtin_array_push_back_zero")
                ->args({"array","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_erase)>(*this, lib, "__builtin_array_erase",
            SideEffects::modifyArgument, "builtin_array_erase")
                ->args({"array","index","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_erase_range)>(*this, lib, "__builtin_array_erase_range",
            SideEffects::modifyArgument, "builtin_array_erase_range")
                ->args({"array","index","count","stride","context"});
        addExtern<DAS_BIND_FUN(builtin_array_lock)>(*this, lib, "__builtin_array_lock",
            SideEffects::modifyArgumentAndExternal, "builtin_array_lock")
                ->args({"array","context"});
        addExtern<DAS_BIND_FUN(builtin_array_unlock)>(*this, lib, "__builtin_array_unlock",
            SideEffects::modifyArgumentAndExternal, "builtin_array_unlock")
                ->args({"array","context"});
        addExtern<DAS_BIND_FUN(builtin_array_clear_lock)>(*this, lib, "__builtin_array_clear_lock",
            SideEffects::modifyArgumentAndExternal, "builtin_array_clear_lock")
                ->args({"array","context"});
    }
}
