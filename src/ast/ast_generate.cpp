#include "daScript/misc/platform.h"

#include "daScript/ast/ast_generate.h"

namespace das {

    ExpressionPtr makeDelete ( const VariablePtr & var ) {
        auto eVar = make_shared<ExprVar>(var->at, var->name);
        auto del = make_shared<ExprDelete>(var->at, eVar);
        return del;
    }

    FunctionPtr makeConstructor ( Structure * str ) {
        auto fn = make_shared<Function>();
        fn->name = str->name;
        fn->at = str->at;
        fn->result = make_shared<TypeDecl>(str->shared_from_this());
        auto block = make_shared<ExprBlock>();
        // return [[t()]]
        auto makeT = make_shared<ExprMakeStructure>(str->at);
        makeT->useInitializer = true;
        makeT->makeType = make_shared<TypeDecl>(str->shared_from_this());
        makeT->structs.push_back(make_shared<MakeStruct>());
        auto returnDecl = make_shared<ExprReturn>(str->at,makeT);
        returnDecl->moveSemantics = !str->canCopy();
        block->list.push_back(returnDecl);
        fn->body = block;
        return fn;
    }
}

