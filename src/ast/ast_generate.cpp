#include "daScript/misc/platform.h"

#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_expressions.h"

namespace das {

    // array comprehension
    //  invoke( $()
    //      let temp : Array<expr->subexpr->type>
    //      for .....
    //          if where ....
    //              push(temp, subexpr)
    //      return temp
    ExpressionPtr generateComprehension ( ExprArrayComprehension * expr ) {
        auto compName = "__acomp_" + to_string(expr->at.line);
        auto pClosure = make_shared<ExprBlock>();
        pClosure->at = expr->subexpr->at;
        pClosure->returnType = make_shared<TypeDecl>(Type::autoinfer);
        // temp : Array<expr->subexpr->type>
        auto pVar = make_shared<Variable>();
        pVar->at = expr->at;
        pVar->name = compName;
        pVar->type = make_shared<TypeDecl>(Type::tArray);
        pVar->type->constant = false;
        pVar->type->removeConstant = true;
        pVar->type->firstType = make_shared<TypeDecl>(*expr->subexpr->type);
        pVar->type->firstType->ref = false;
        // let temp
        auto pLet = make_shared<ExprLet>();
        pLet->at = expr->at;
        pLet->variables.push_back(pVar);
        pClosure->list.push_back(pLet);
        // push(temp, subexpr)
        auto pPushVal = make_shared<ExprVar>();
        pPushVal->at = expr->at;
        pPushVal->name = compName;
        auto pPush = make_shared<ExprCall>();
        pPush->at = expr->at;
        pPush->name = "push";
        pPush->arguments.push_back(pPushVal);
        pPush->arguments.push_back(expr->subexpr->clone());
        // for ...
        auto pForBlock = make_shared<ExprBlock>();
        pForBlock->at = expr->at;
        pForBlock->inTheLoop = true;
        if ( expr->exprWhere ) {
            // push block
            auto pPushBlock = make_shared<ExprBlock>();
            pPushBlock->at = expr->at;
            pPushBlock->list.push_back(pPush);
            // for .... if where ... push
            auto pIf = make_shared<ExprIfThenElse>();
            pIf->at = expr->at;
            pIf->cond = expr->exprWhere->clone();
            pIf->if_true = pPushBlock;
            pForBlock->list.push_back(pIf);
        } else {
            // for .... push
            pForBlock->list.push_back(pPush);
        }
        auto pFor = static_pointer_cast<ExprFor>(expr->exprFor->clone());
      pFor->subexpr = pForBlock;
        pClosure->list.push_back(pFor);
        // return temp
        auto pVal = make_shared<ExprVar>();
        pVal->at = expr->at;
        pVal->name = compName;
        auto pRet = make_shared<ExprReturn>();
        pRet->at = expr->at;
        pRet->subexpr = pVal;
        pRet->moveSemantics = true;
        pClosure->list.push_back(pRet);
        // make block
        auto pMakeBlock = make_shared<ExprMakeBlock>(expr->at,pClosure);
        // invoke
        auto pInvoke = make_shared<ExprInvoke>(expr->at, "invoke");
        pInvoke->arguments.push_back(pMakeBlock);
        return pInvoke;
    }

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
        fn->generated = true;
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

    // def clone(a,b:structure)
    //  a.f1 := b.f1
    //  a.f2 := b.f2
    //  ...
    FunctionPtr makeClone ( Structure * str ) {
        auto varA = make_shared<Variable>();
        varA->name = "a";
        varA->type = make_shared<TypeDecl>(str->shared_from_this());
        varA->at = str->at;
        auto varB = make_shared<Variable>();
        varB->name = "b";
        varB->type = make_shared<TypeDecl>(str->shared_from_this());
        varB->type->constant = true;
        varB->at = str->at;
        auto fn = make_shared<Function>();
        fn->name = "clone";
        fn->generated = true;
        fn->at = str->at;
        fn->result = make_shared<TypeDecl>();
        fn->arguments.push_back(varA);
        fn->arguments.push_back(varB);
        auto block = make_shared<ExprBlock>();
        for ( auto & fi : str->fields ) {
            auto lA = make_shared<ExprVar>(fi.at, "a");
            auto lAdotF = make_shared<ExprField>(fi.at, lA, fi.name);
            auto lB = make_shared<ExprVar>(fi.at, "b");
            auto lBdotF = make_shared<ExprField>(fi.at, lB, fi.name);
            auto cl = make_shared<ExprClone>(fi.at, lAdotF, lBdotF);
            block->list.push_back(cl);
        }
        fn->body = block;
        return fn;
    }

    FunctionPtr generateLambdaFunction ( const string & lambdaName, ExprBlock * block, const StructurePtr & ls ) {
        auto lfn = lambdaName + "__def";
        auto pFunc = make_shared<Function>();
        pFunc->generated = true;
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
            cA->marked_used = true;             // to avoid 'unsued argument' error
            pFunc->arguments.push_back(cA);
        }
        return pFunc;
    }

    StructurePtr generateLambdaStruct ( const string & lambdaName, ExprBlock * block, const set<VariablePtr> & capt ) {
        auto lsn = lambdaName;
        auto pStruct = make_shared<Structure>(lsn);
        auto btd = block->makeBlockType();
        btd->baseType = Type::tFunction;
        btd->constant = false;
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
        asc->needTypeInfo = true;
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

