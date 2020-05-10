#pragma once

#include "daScript/ast/ast.h"

namespace das {
    char * ast_describe ( smart_ptr_raw<TypeDecl> t, bool d_extra, bool d_contracts, bool d_module, Context * context );
}
