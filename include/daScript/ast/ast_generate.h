#pragma once

#include "daScript/ast/ast.h"

namespace das {
    /*
     def STRUCT_NAME
        let t : STRUCT_TYPE
        t.field1 = init1
        t.field2 = init2
        return t
     */
    FunctionPtr makeConstructor ( Structure * str );
    
    /*
        delete var
     */
    ExpressionPtr makeDelete ( const VariablePtr & var );
}

