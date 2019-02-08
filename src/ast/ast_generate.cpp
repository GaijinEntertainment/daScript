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
        // let t : STRUCT_TYPE
        auto varDecl = make_shared<ExprLet>();
        varDecl->scoped = false;
        auto varT = make_shared<Variable>();
        varT->name = "t";
        varT->type = make_shared<TypeDecl>(str->shared_from_this());
        varDecl->variables.push_back(varT);
        block->list.push_back(varDecl);
        // for each initialized field
        for ( auto & fd : str->fields ) {
            if ( fd.init ) {
                // t.field = init
                auto getT = make_shared<ExprVar>(fd.at,"t");
                auto getTField = make_shared<ExprField>(fd.at,getT,fd.name);
                shared_ptr<ExprOp2> opEq = fd.type->canCopy() ?
                    static_pointer_cast<ExprOp2>(make_shared<ExprCopy>()) :
                    static_pointer_cast<ExprOp2>(make_shared<ExprMove>());
                opEq->left = getTField;
                opEq->right = fd.init->clone();
                block->list.push_back(opEq);
            }
        }
        // return t
        auto getT = make_shared<ExprVar>(str->at,"t");
        auto returnDecl = make_shared<ExprReturn>(str->at,getT);
        returnDecl->moveSemantics = !str->canCopy();
        block->list.push_back(returnDecl);
        fn->body = block;
        return fn;
    }
}

