#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"

#include "dasClangBind.h"

#include "needClangBind.h"

#include "dasClangBind.struct.impl.inc"

namespace das {

#include "dasClangBind.enum.class.inc"
#include "dasClangBind.struct.class.inc"

char * from_CXString_to_string ( const CXString & cxs, Context * context ) {
    auto cst = clang_getCString(cxs);
    auto res = context->stringHeap->allocateString(cst);
    clang_disposeString(cxs);
    return res;
}

void peek_CXString ( const CXString & cxs, const TBlock<void,TTemporary<const char *>> & blk, Context * context, LineInfoArg * arg ) {
    auto cst = clang_getCString(cxs);
    das_invoke<void>::invoke<const char *>(context,arg,blk,cst);
    clang_disposeString(cxs);
}

struct vchGoo {
    Block           blk;
    Context *       ctx;
    LineInfoArg *   at;
};

void das_clangVisitChildren ( CXCursor cursor, const TBlock<CXChildVisitResult,CXCursor,CXCursor> & block, Context * ctx, LineInfoArg * at ) {
    vchGoo goo {block, ctx, at};
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
            vchGoo * goo = (vchGoo *) client_data;
            return das_invoke<CXChildVisitResult>::invoke<const CXCursor &,const CXCursor &>(goo->ctx,goo->at,goo->blk,c,parent);
        },
        &goo
    );
};

Module_ClangBind::Module_ClangBind() : Module("cbind") {
    ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();
    #include "dasClangBind.enum.add.inc"
    #include "dasClangBind.dummy.add.inc"
    #include "dasClangBind.struct.add.inc"
    #include "dasClangBind.struct.postadd.inc"
    #include "dasClangBind.alias.add.inc"
    #include "dasClangBind.func.reg.inc"
    // now for the custom part
    addExtern<DAS_BIND_FUN(from_CXString_to_string)>(*this, lib,  "string",
        SideEffects::worstDefault, "from_CXString_to_string")
            ->args({"CXString","context"});
    addExtern<DAS_BIND_FUN(peek_CXString)>(*this, lib,  "peek",
        SideEffects::worstDefault, "peek_CXString")
            ->args({"CXString","blk","context","line"});
    addExtern<DAS_BIND_FUN(das_clangVisitChildren)>(*this, lib,  "clang_visitChildren",
        SideEffects::worstDefault, "das_clangVisitChildren")
            ->args({"cursor","block","context","line"});
}

}

REGISTER_MODULE_IN_NAMESPACE(Module_ClangBind,das);

