#pragma once

#include "daScript/ast/ast.h"

namespace das {
    char * ast_describe_typedecl ( smart_ptr_raw<TypeDecl> t, bool d_extra, bool d_contracts, bool d_module, Context * context );
    char * ast_describe_expression ( smart_ptr_raw<Expression> t, Context * context );
}
