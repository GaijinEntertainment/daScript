#include "daScript/misc/platform.h"

#include "daScript/ast/ast_generate.h"

namespace das {

    /* a->b(args) is short for invoke(a.b, cast<auto> a, args)  */
    ExprInvoke * makeInvokeMethod ( const LineInfo & at, Expression * a, const string & b ) {
        auto pInvoke = new ExprInvoke(at, "invoke");
        auto pAt = make_shared<ExprField>(at, a->clone(), b);
        pInvoke->arguments.push_back(pAt);
        auto pCast = make_shared<ExprCast>();
        pCast->at = at;
        pCast->castType = make_shared<TypeDecl>(Type::autoinfer);
        pCast->subexpr = ExpressionPtr(a);
        pInvoke->arguments.push_back(pCast);
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

    FunctionPtr generateLambdaFunction ( const string & lambdaName, ExprBlock * block, const StructurePtr & ls ) {
        auto lfn = lambdaName + "__def";
        auto pFunc = make_shared<Function>();
        pFunc->at = block->at;
        pFunc->name = lfn;
        auto fb = make_shared<ExprBlock>();
        auto with = make_shared<ExprWith>(block->at);
        with->with = make_shared<ExprVar>(block->at, "__this");
        with->body = block->clone();
        auto wb = static_pointer_cast<ExprBlock>(with->body);
        wb->blockFlags = 0;
        fb->list.push_back(with);
        pFunc->body = fb;
        pFunc->result = make_shared<TypeDecl>(*block->type);
        auto cTHIS = make_shared<Variable>();
        cTHIS->name = "__this";
        cTHIS->type = make_shared<TypeDecl>(ls);
        pFunc->arguments.push_back(cTHIS);
        for ( auto & arg : block->arguments ) {
            auto cA = arg->clone();
            pFunc->arguments.push_back(cA);
        }
        return pFunc;
    }

    StructurePtr generateLambdaStruct ( const string & lambdaName, ExprBlock * block, const set<VariablePtr> & capt ) {
        auto lsn = lambdaName;
        auto pStruct = make_shared<Structure>(lsn);
        auto btd = block->makeBlockType();
        btd->baseType = Type::tFunction;
        auto thisArg = make_shared<TypeDecl>(pStruct);
        btd->argTypes.insert(btd->argTypes.begin(), thisArg);
        pStruct->fields.emplace_back("__lambda", btd, nullptr, AnnotationArgumentList(), false, block->at);
        for ( auto var : capt ) {
            auto td = make_shared<TypeDecl>(*var->type);
            td->ref = false;
            td->constant = false;
            pStruct->fields.emplace_back(var->name, td, nullptr, AnnotationArgumentList(), false, var->at);
        }
        return pStruct;
    }

    ExpressionPtr generateLambdaMakeStruct ( const StructurePtr & ls, const FunctionPtr & lf, const set<VariablePtr> & capt ) {
        auto asc = new ExprAscend();
        auto makeS = make_shared<ExprMakeStructure>();
        makeS->makeType = make_shared<TypeDecl>(ls);
        auto ms = make_shared<MakeStruct>();
        auto atTHIS = make_shared<ExprAddr>(lf->at, lf->name);
        auto mTHIS = make_shared<MakeFieldDecl>(lf->at, "__lambda", atTHIS, false);
        ms->push_back(mTHIS);
        for ( auto cV : capt ) {
            auto varV = make_shared<ExprVar>(cV->at, cV->name);
            auto mV = make_shared<MakeFieldDecl>(cV->at, cV->name, varV, !cV->type->canCopy());
            ms->push_back(mV);
        }
        makeS->structs.push_back(ms);
        asc->subexpr = makeS;
        asc->ascType = make_shared<TypeDecl>(*ls->fields[0].type);
        asc->ascType->argTypes.erase(asc->ascType->argTypes.begin());
        asc->ascType->baseType = Type::tLambda;
        return ExpressionPtr(asc);
    }
}

