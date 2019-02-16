#include "daScript/misc/platform.h"

#include "daScript/ast/ast_generate.h"

namespace das {

    /* a->b(args) is short for invoke(a.b, a, args)  */
    ExprInvoke * makeInvokeMethod ( const LineInfo & at, Expression * a, const string & b ) {
        auto pInvoke = new ExprInvoke(at, "invoke");
        auto pAt = make_shared<ExprField>(at, a->clone(), b);
        pInvoke->arguments.push_back(pAt);
        pInvoke->arguments.push_back(ExpressionPtr(a));
        return pInvoke;
    }

    ExpressionPtr makeDelete ( const VariablePtr & var ) {
        auto eVar = make_shared<ExprVar>(var->at, var->name);
        auto del = make_shared<ExprDelete>(var->at, eVar);
        return del;
    }

    // return [[t()]]
    FunctionPtr makeConstructor ( Structure * str ) {
        auto fn = make_shared<Function>();
        fn->name = str->name;
        fn->at = str->at;
        fn->result = make_shared<TypeDecl>(str->shared_from_this());
        auto block = make_shared<ExprBlock>();
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

