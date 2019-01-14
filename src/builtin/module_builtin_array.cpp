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
    
    void Module_BuiltIn::addArrayTypes(ModuleLibrary & lib) {
        // array functions
        addExtern<decltype(builtin_array_size), builtin_array_size>(*this, lib, "length", false);
        addExtern<decltype(builtin_array_capacity), builtin_array_capacity>(*this, lib, "capacity", false);
        // array built-in functions
        addExtern<decltype(builtin_array_resize), builtin_array_resize>(*this, lib, "__builtin_array_resize", true);
        addExtern<decltype(builtin_array_reserve), builtin_array_reserve>(*this, lib, "__builtin_array_reserve", true);
    }
}
