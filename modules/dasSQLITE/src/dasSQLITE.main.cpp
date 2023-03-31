#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#include "dasSQLITE.h"

#include "cb_dasSQLITE.h"

namespace das {

void Module_dasSQLITE::initMain() {
    // ok, lets fix up everything returning uint8? into returning string
    functions.foreach([&](const FunctionPtr & fn) {
        if ( fn->result->isPointer() && fn->result->firstType &&
                fn->result->firstType->baseType==Type::tUInt8 && fn->result->firstType->dim.size()==0 ) {
            fn->result = make_smart<TypeDecl>(Type::tString);
            fn->result->constant = true;
            fn->result->temporary = true;
        }
    });
}

ModuleAotType Module_dasSQLITE::aotRequire ( TextWriter & tw ) const {
    tw << "#include \"../modules/dasSQLITE/src/aot_sqlite.h\"\n";
    return ModuleAotType::cpp;
}

}


