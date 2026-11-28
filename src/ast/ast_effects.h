#pragma once

#include "daScript/ast/ast.h"

namespace das {
    bool nodeRaises ( Expression * expr );
    bool callRaises ( Function * fn );
    bool droppable ( Expression * expr );
    const Annotation * macroAnnotation ( const Function * fn );
    bool hasMacroOwnedShape ( const Function * fn );
    // a global of a frozen (already compiled) module is read-only: module jobs that require it
    // must not stamp its flags. src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-workers
    inline bool isFrozenGlobal ( const Variable * var ) {
        return var->module && var->module->frozen;
    }
}
