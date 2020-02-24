#include "daScript/misc/platform.h"

#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    bool isExpressionVariable(const ExpressionPtr & expr, const string & name) {
        if (expr->rtti_isVar()) {
            auto var = static_pointer_cast<ExprVar>(expr);
            return var->name == name;
        }
        return false;
    }

    bool isExpressionVariableDeref(const ExpressionPtr & expr, const string & name) {
        if (expr->rtti_isVar()) {
            auto var = static_pointer_cast<ExprVar>(expr);
            return var->name == name;
        } else if (expr->rtti_isR2V()) {
            auto r2v = static_pointer_cast<ExprRef2Value>(expr);
            if (r2v->subexpr->rtti_isVar()) {
                auto var = static_pointer_cast<ExprVar>(r2v->subexpr);
                return var->name == name;
            }
        }
        return false;
    }

    bool isExpressionNull(const ExpressionPtr & expr) {
        if (expr->rtti_isConstant() && expr->type->isPointer()) {
            auto cptr = static_pointer_cast<ExprConstPtr>(expr);
            return cptr->getValue() == nullptr;
        }
        return false;
    }

#define VERIFY_GENERATED    0
#define LOG_GENERATED       0

    struct CheckLineInfoVisitor : Visitor {
        virtual void preVisitExpression ( Expression * expr ) override {
            Visitor::preVisitExpression(expr);
            if ( expr->rtti_isFakeContext() ) return;
            DAS_ASSERT(expr->at.column && expr->at.line);
        }
    };

    void verifyGenerated ( const ExpressionPtr & expr ) {
#if LOG_GENERATED
        TextPrinter pp;
        pp << "VERIFY:\n" << *expr << "\n";
#endif
#if VERIFY_GENERATED
        CheckLineInfoVisitor vis;
        expr->visit(vis);
#endif
    }

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
      pFor->body = pForBlock;
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

    FunctionPtr generateLambdaFunction ( const string & lambdaName, ExprBlock * block,
                                        const StructurePtr & ls, bool needYield, bool isUnsafe ) {
        auto lfn = lambdaName + "__def";
        auto pFunc = make_shared<Function>();
        pFunc->generated = true;
        pFunc->at = block->at;
        pFunc->name = lfn;
        pFunc->unsafe = isUnsafe;
        auto fb = make_shared<ExprBlock>();
        fb->at = block->at;
        auto with = make_shared<ExprWith>(block->at);
        with->with = make_shared<ExprVar>(block->at, "__this");
        with->body = block->clone();
        if ( needYield ) {
            pFunc->generator = true;
            auto bbl = static_pointer_cast<ExprBlock>(with->body);
            // goto __yeild
            auto gvar = make_shared<ExprVar>(block->at, "__yield");
            auto gexpr = make_shared<ExprGoto>(block->at, gvar);
            bbl->list.insert(bbl->list.begin(), gexpr);
            // label "0"
            auto lzero = make_shared<ExprLabel>(block->at, pFunc->totalGenLabel);
            bbl->list.insert(bbl->list.begin() + 1, lzero);
            pFunc->totalGenLabel ++;
        }
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
        verifyGenerated(pFunc->body);
        return pFunc;
    }

    StructurePtr generateLambdaStruct ( const string & lambdaName, ExprBlock * block,
                                       const das_set<VariablePtr> & capt, bool needYield ) {
        auto lsn = lambdaName;
        auto pStruct = make_shared<Structure>(lsn);
        auto btd = block->makeBlockType();
        btd->baseType = Type::tFunction;
        btd->constant = false;
        auto thisArg = make_shared<TypeDecl>(pStruct);
        btd->argTypes.insert(btd->argTypes.begin(), thisArg);
        pStruct->fields.emplace_back("__lambda", btd, nullptr, AnnotationArgumentList(), false, block->at);
        if ( needYield ) {
            auto yt = make_shared<TypeDecl>(Type::tInt);
            pStruct->fields.emplace_back("__yield", yt, nullptr, AnnotationArgumentList(), false, block->at);
        }
        for ( auto var : capt ) {
            auto td = make_shared<TypeDecl>(*var->type);
            td->ref = false;
            td->constant = false;
            pStruct->fields.emplace_back(var->name, td, nullptr, AnnotationArgumentList(), false, var->at);
        }
        return pStruct;
    }

    ExpressionPtr generateLambdaMakeStruct ( const StructurePtr & ls, const FunctionPtr & lf,
                                            const das_set<VariablePtr> & capt, const LineInfo & at ) {
        auto asc = new ExprAscend();
        asc->at = at;
        asc->needTypeInfo = true;
        auto makeS = make_shared<ExprMakeStructure>();
        makeS->at = at;
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
        auto res = ExpressionPtr(asc);
        verifyGenerated(res);
        return res;
    }

    // rename variable to unique name variable

    class RenameVar : public Visitor {
    public:
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            scopes.push_back(block);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            scopes.pop_back();
            return Visitor::visit(block);
        }
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
            if ( scopes.size()==1 ) {   // only top level block
                for ( auto & var : expr->variables ) {
                    if ( var->name[0]!='_' || var->name[1]!='_' ) {
                        string newName = "__" + var->name + "_rename_at_" + to_string(var->at.line);
                        rename[var->name] = newName;
                        var->name = newName;
                    }
                }
            }
        }
        virtual void preVisit ( ExprVar * expr ) override {
            if ( !scopes.size() ) return;
            auto it = rename.find(expr->name);
            if ( it != rename.end() ) {
                expr->name = it->second;
            }
        }
    protected:
        vector<ExprBlock *> scopes;
        das_map<string,string> rename;
    };

    void giveBlockVariablesUniqueNames  ( const ExpressionPtr & expr ) {
        RenameVar rename;
        expr->visit(rename);
    }

    // replace ref to ptr

    class Ref2PtrVisitor : public Visitor {
    public:
        Ref2PtrVisitor ( const string & n ) : varName(n) {}
        virtual ExpressionPtr visit ( ExprVar * expr ) override {
            if  ( expr->name==varName ) {
                return make_shared<ExprPtr2Ref>(expr->at, expr->shared_from_this());
            }
            return Visitor::visit(expr);
        }
    protected:
        string varName;
    };

    void replaceRef2Ptr ( const ExpressionPtr & expr, const string & name ) {
        Ref2PtrVisitor r2ptr(name);
        expr->visit(r2ptr);
    }

    // replace break and continue with 'goto label' for the specific loop

    class BreakAndContinueVisitor : public Visitor {
    public:
        BreakAndContinueVisitor ( int32_t bg, int32_t cg )
            : breakGoto(bg), continueGoto(cg) {
        }
        virtual void preVisit ( ExprWhile * expr ) override {
            Visitor::preVisit(expr);
            depth ++;
        }
        virtual ExpressionPtr visit(ExprWhile *expr) override {
            depth --;
            return Visitor::visit(expr);
        }
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            depth ++;
        }
        virtual ExpressionPtr visit(ExprFor *expr) override {
            depth --;
            return Visitor::visit(expr);
        }
        virtual ExpressionPtr visit(ExprBreak *expr) override {
            if ( depth ) return Visitor::visit(expr);
            return make_shared<ExprGoto>(expr->at, breakGoto);
        }
        virtual ExpressionPtr visit(ExprContinue *expr) override {
            if ( depth ) return Visitor::visit(expr);
            return make_shared<ExprGoto>(expr->at, continueGoto);
        }
    protected:
        int32_t breakGoto;
        int32_t continueGoto;
        int     depth = 0;
    };

    void replaceBreakAndContinue ( Expression * expr, int32_t bg, int32_t cg ) {
        BreakAndContinueVisitor rbnc(bg, cg);
        expr->visit(rbnc);
    }

    ExpressionPtr generateYield( ExprYield * expr, const FunctionPtr & func ) {
        const auto & yarg = func->arguments[1];
        // TODO: verify yield type so that error is 'yield' error, not copy or move error
        auto LabelX = func->totalGenLabel ++;
        auto blk = make_shared<ExprBlock>();
        blk->isCollapseable = true;
        blk->at = expr->at;
        bool makeRef = false;
        if ( func->arguments.size()==2 ) { // starts with _ryield
            const auto & argn = func->arguments[1]->name;
            if ( argn.length()>=7 ) {
                makeRef = memcmp ( argn.c_str(), "_ryield", 7 ) ==  0;
            }
        }
        if ( expr->moveSemantics ) {
            // TODO: error on makeRef + moveSemantics
            // result <- a
            auto mto = make_shared<ExprVar>(expr->at, yarg->name);
            auto mfr = expr->subexpr->clone();
            auto mve = make_shared<ExprMove>(expr->at, mto, mfr);
            blk->list.push_back(mve);
        } else {
            // result = a
            auto cto = make_shared<ExprVar>(expr->at, yarg->name);
            auto cfr = expr->subexpr->clone();
            if ( makeRef ) {
                cfr = make_shared<ExprRef2Ptr>(expr->at, cfr);
                cfr->alwaysSafe = true;
            }
            auto cpy = make_shared<ExprCopy>(expr->at, cto, cfr);
            blk->list.push_back(cpy);
        }
        // yield = X
        auto yyx = make_shared<ExprVar>(expr->at, "__yield");
        auto clx = make_shared<ExprConstInt>(expr->at, LabelX);
        auto cpy = make_shared<ExprCopy>(expr->at, yyx, clx);
        blk->list.push_back(cpy);
        // return true
        auto btr = make_shared<ExprConstBool>(expr->at, true);
        auto rex = make_shared<ExprReturn>(expr->at, btr);
        rex->fromYield = true;
        blk->list.push_back(rex);
        auto lbx = make_shared<ExprLabel>(expr->at, LabelX,
                                          "yield at line " + to_string(expr->at.line));
        blk->list.push_back(lbx);
        verifyGenerated(blk);
        return blk;
    }

    ExpressionPtr replaceGeneratorLet ( ExprLet * expr, const FunctionPtr & func, ExprBlock * scope ) {
        auto blk = make_shared<ExprBlock>();
        blk->at = expr->at;
        blk->isCollapseable = true;
        auto capture = func->arguments[0]->type->structType;
        DAS_ASSERT(capture && "generator first argument is lambda capture");
        for ( auto & var : expr->variables ) {
            auto vtd = make_shared<TypeDecl>(*var->type);
            bool isRef = vtd->ref;
            if ( isRef ) {
                auto pvtd = make_shared<TypeDecl>(Type::tPointer);
                pvtd->firstType = vtd;
                pvtd->constant = vtd->constant;
                vtd->ref = false;
                vtd = pvtd;
                replaceRef2Ptr(scope->shared_from_this(), var->name);
            }
            capture->fields.emplace_back(
                                         var->name,
                                         vtd,
                                         nullptr,
                                         AnnotationArgumentList(),
                                         false,
                                         expr->at);
            auto lvar = make_shared<ExprVar>(var->at, var->name);
            if ( var->init ) {
                auto rini = var->init->clone();
                if ( isRef ) {
                    auto arini = make_shared<ExprRef2Ptr>(expr->at, rini);
                    arini->alwaysSafe = true;
                    rini = arini;
                }
                if ( var->init_via_clone ) {
                    auto cln = make_shared<ExprClone>(var->at, lvar, rini);
                    blk->list.push_back(cln);
                } else if ( var->init_via_move ) {
                    auto mve = make_shared<ExprMove>(var->at, lvar, rini);
                    blk->list.push_back(mve);
                } else {
                    auto cpy = make_shared<ExprCopy>(var->at, lvar, rini);
                    blk->list.push_back(cpy);
                }
            } else {
                auto mz = make_shared<ExprMemZero>(var->at, "memzero");
                mz->arguments.push_back(lvar);
                blk->list.push_back(mz);
            }
        }
        verifyGenerated(blk);
        return blk;
    }

    ExpressionPtr replaceGeneratorIfThenElse ( ExprIfThenElse * expr, const FunctionPtr & func ) {
        auto blk = make_shared<ExprBlock>();
        blk->at = expr->at;
        blk->isCollapseable = true;
        if ( expr->if_false ) {
            auto else_label = func->totalGenLabel ++;
            auto end_label = func->totalGenLabel ++;
            auto gtel = make_shared<ExprGoto>(expr->at, else_label);
            auto btel = make_shared<ExprBlock>();
            btel->at = expr->at;
            btel->list.push_back(gtel);
            auto ncnd = make_shared<ExprOp1>(expr->cond->at, "!", expr->cond->clone());
            auto ifnc = make_shared<ExprIfThenElse>(expr->at, ncnd, btel, nullptr);
            blk->list.push_back(ifnc);
            auto ift = expr->if_true->clone();
            if ( ift->rtti_isBlock() ){
                auto iftb = static_pointer_cast<ExprBlock>(ift);
                iftb->isCollapseable = true;
                giveBlockVariablesUniqueNames(ift);
            }
            blk->list.push_back(ift);
            auto gten = make_shared<ExprGoto>(expr->at, end_label);
            blk->list.push_back(gten);
            auto elsel = make_shared<ExprLabel>(expr->at, else_label,
                                                "else if at line " + to_string(expr->at.line));
            blk->list.push_back(elsel);
            auto iff = expr->if_false->clone();
            if ( iff->rtti_isBlock() ){
                auto iffb = static_pointer_cast<ExprBlock>(iff);
                iffb->isCollapseable = true;
                giveBlockVariablesUniqueNames(iff);
            }
            blk->list.push_back(iff);
            auto enddl = make_shared<ExprLabel>(expr->at, end_label,
                                                "end if at line " + to_string(expr->at.line));
            blk->list.push_back(enddl);
        } else {
            auto end_label = func->totalGenLabel ++;
            auto gtel = make_shared<ExprGoto>(expr->at, end_label);
            auto btel = make_shared<ExprBlock>();
            btel->at = expr->at;
            btel->list.push_back(gtel);
            auto ncnd = make_shared<ExprOp1>(expr->cond->at, "!", expr->cond->clone());
            auto ifnc = make_shared<ExprIfThenElse>(expr->at, ncnd, btel, nullptr);
            blk->list.push_back(ifnc);
            auto ift = expr->if_true->clone();
            if ( ift->rtti_isBlock() ){
                auto iftb = static_pointer_cast<ExprBlock>(ift);
                iftb->isCollapseable = true;
                giveBlockVariablesUniqueNames(ift);
            }
            blk->list.push_back(ift);
            auto enddl = make_shared<ExprLabel>(expr->at, end_label,
                                                "end if at line " + to_string(expr->at.line));
            blk->list.push_back(enddl);
        }
        verifyGenerated(blk);
        return blk;
    }

    ExpressionPtr replaceGeneratorWhile ( ExprWhile * expr, const FunctionPtr & func ) {
        auto begin_loop_label = func->totalGenLabel ++;
        auto end_loop_label = func->totalGenLabel ++;
        shared_ptr<ExprBlock> bodyBlock;
        if ( expr->body->rtti_isBlock() ) {
            bodyBlock = static_pointer_cast<ExprBlock>(expr->body->clone());
            giveBlockVariablesUniqueNames(bodyBlock);
            replaceBreakAndContinue(bodyBlock.get(), end_loop_label, begin_loop_label);
        }
        auto blk = make_shared<ExprBlock>();
        blk->at = expr->at;
        blk->isCollapseable = true;
        auto bll = make_shared<ExprLabel>(expr->at, begin_loop_label,
                                          "begin while at line " + to_string(expr->at.line));
        blk->list.push_back(bll);
        auto gtel = make_shared<ExprGoto>(expr->at, end_loop_label);
        auto btel = make_shared<ExprBlock>();
        btel->at = expr->at;
        btel->list.push_back(gtel);
        auto ncnd = make_shared<ExprOp1>(expr->cond->at, "!", expr->cond->clone());
        auto ifnc = make_shared<ExprIfThenElse>(expr->at, ncnd, btel, nullptr);
        blk->list.push_back(ifnc);
        if ( bodyBlock ) {
            for ( auto & bse : bodyBlock->list ) {
                blk->list.push_back(bse->clone());
            }
        } else {
            blk->list.push_back(expr->body->clone());
        }
        auto gbeg = make_shared<ExprGoto>(expr->at, begin_loop_label);
        blk->list.push_back(gbeg);
        auto ell = make_shared<ExprLabel>(expr->at, end_loop_label,
                                          "end while at line " + to_string(expr->at.line));
        blk->list.push_back(ell);
        if ( bodyBlock && !bodyBlock->finalList.empty() ) { // finally, if we have it
            for ( auto & fse : bodyBlock->finalList ) {
                blk->list.push_back(fse->clone());
            }
        }
        verifyGenerated(blk);
        return blk;
    }

    ExpressionPtr replaceGeneratorFor ( ExprFor * expr, const FunctionPtr & func ) {
        auto begin_loop_label = func->totalGenLabel ++;
        auto mid_loop_label = func->totalGenLabel ++;
        auto end_loop_label = func->totalGenLabel ++;
        shared_ptr<ExprBlock> bodyBlock;
        if ( expr->body->rtti_isBlock() ) {
            bodyBlock = static_pointer_cast<ExprBlock>(expr->body->clone());
            giveBlockVariablesUniqueNames(bodyBlock);
            replaceBreakAndContinue(bodyBlock.get(), end_loop_label, mid_loop_label);
        }
        auto blk = make_shared<ExprBlock>();
        blk->at = expr->at;
        blk->isCollapseable = true;
        auto gtel = make_shared<ExprGoto>(expr->at, end_loop_label);
        auto btel = make_shared<ExprBlock>();
        btel->at = expr->at;
        btel->list.push_back(gtel);
        // names
        string loopVar = "_loop_at_" + to_string(expr->at.line);
        vector<string> srcNames, pVarNames;
        for ( size_t si=0; si!=expr->sources.size(); ++si ) {
            srcNames.push_back("_source_" + to_string(si) + "_at_" + to_string(expr->at.line));
            pVarNames.push_back("_pvar_" + to_string(si) + "_at_" + to_string(expr->at.line));
        }
        auto leqt = make_shared<ExprLet>();
        leqt->at = expr->at;
        auto lvar = make_shared<Variable>();
        lvar->at = expr->at;
        lvar->name = loopVar;
        lvar->type = make_shared<TypeDecl>(Type::tBool);
        lvar->init = make_shared<ExprConstBool>(expr->at, true);
        leqt->variables.push_back(lvar);
        blk->list.push_back(leqt);
        // sources
        for ( size_t si=0; si!=expr->sources.size(); ++si ) {
            const string & srcName = srcNames[si];
            const string & pVarName = pVarNames[si];
            const string & srcVarName = expr->iterators[si];
            const auto & src = expr->sources[si];
            const auto & iterv = expr->iteratorVariables[si];
            // let src0 = each(blah) or let src0 = blah if its iterator
            auto seqt = make_shared<ExprLet>();
            seqt->at = expr->at;
            auto svar = make_shared<Variable>();
            svar->at = expr->at;
            svar->name = srcName;
            svar->type = make_shared<TypeDecl>(Type::autoinfer);
            svar->init_via_move = true;
            if ( src->type->isGoodIteratorType() ) {
                svar->init = src->clone();
            } else {
                auto ceach = make_shared<ExprCall>(expr->at, "each");
                ceach->alwaysSafe = true;
                ceach->arguments.push_back(src->clone());
                svar->init = ceach;
            }
            seqt->variables.push_back(svar);
            blk->list.push_back(seqt);
            // let it0 : type_of_iterable
            auto srci = make_shared<ExprLet>();
            srci->at = expr->at;
            auto srcv = make_shared<Variable>();
            srcv->at = expr->at;
            srcv->name = srcVarName;
            if ( iterv->type->isRef() ) {
                srcv->type = make_shared<TypeDecl>(Type::tPointer);
                srcv->type->firstType = make_shared<TypeDecl>(*iterv->type);
                srcv->type->firstType->ref = false;
                if ( bodyBlock ) {
                    replaceRef2Ptr(bodyBlock, iterv->name);
                } else {
                    replaceRef2Ptr(expr->shared_from_this(), iterv->name);
                }
            } else {
                srcv->type = make_shared<TypeDecl>(*iterv->type);
            }
            srcv->type->constant = false;
            srci->variables.push_back(srcv);
            blk->list.push_back(srci);
            // let pvar0 = reinterpret_cast<void?>(addr(it0))
            auto vit0 = make_shared<ExprVar>(expr->at, srcVarName);
            auto adri = make_shared<ExprRef2Ptr>(expr->at, vit0);
            adri->alwaysSafe = true;
            auto pvoid = make_shared<TypeDecl>(Type::tPointer);
            pvoid->firstType = make_shared<TypeDecl>(Type::tVoid);
            auto rein = make_shared<ExprCast>(expr->at, adri, pvoid);
            rein->reinterpret = true;
            auto veqt = make_shared<ExprLet>();
            veqt->at = expr->at;
            auto vvar = make_shared<Variable>();
            vvar->at = expr->at;
            vvar->name = pVarName;
            vvar->type = make_shared<TypeDecl>(*pvoid);
            vvar->init = rein;
            veqt->variables.push_back(vvar);
            blk->list.push_back(veqt);
            // loop &= _builtin_iterator_first(it0,pvar0)
            auto cbif = make_shared<ExprCall>(expr->at, "_builtin_iterator_first");
            cbif->arguments.push_back(make_shared<ExprVar>(expr->at, srcName));
            cbif->arguments.push_back(make_shared<ExprVar>(expr->at, pVarName));
            auto lande = make_shared<ExprOp2>(expr->at,"&=",
                                              make_shared<ExprVar>(expr->at,loopVar),cbif);
            blk->list.push_back(lande);
        }
        auto bll = make_shared<ExprLabel>(expr->at, begin_loop_label,
                                          "begin for at line " + to_string(expr->at.line));
        blk->list.push_back(bll);
        auto ncnd = make_shared<ExprOp1>(expr->at, "!", make_shared<ExprVar>(expr->at,loopVar));
        auto ifnc = make_shared<ExprIfThenElse>(expr->at, ncnd, btel, nullptr);
        blk->list.push_back(ifnc);
        if ( bodyBlock ) {
            for ( auto & bse : bodyBlock->list ) {
                blk->list.push_back(bse->clone());
            }
        } else {
            blk->list.push_back(expr->body->clone());
        }
        auto mll = make_shared<ExprLabel>(expr->at, mid_loop_label,
                                          "continue for at line " + to_string(expr->at.line));
        blk->list.push_back(mll);
        // loop &= _builtin_iterator_next(it0,pvar0)
        for ( size_t si=0; si!=expr->sources.size(); ++si ) {
            const string & srcName = srcNames[si];
            const string & pVarName = pVarNames[si];
            auto cbif = make_shared<ExprCall>(expr->at, "_builtin_iterator_next");
            cbif->arguments.push_back(make_shared<ExprVar>(expr->at, srcName));
            cbif->arguments.push_back(make_shared<ExprVar>(expr->at, pVarName));
            auto lande = make_shared<ExprOp2>(expr->at,"&=",
                                              make_shared<ExprVar>(expr->at,loopVar),cbif);
            blk->list.push_back(lande);
        }
        auto gbeg = make_shared<ExprGoto>(expr->at, begin_loop_label);
        blk->list.push_back(gbeg);
        auto ell = make_shared<ExprLabel>(expr->at, end_loop_label,
                                          "end for at line " + to_string(expr->at.line));
        blk->list.push_back(ell);
        if ( bodyBlock && !bodyBlock->finalList.empty() ) { // finally, if we have it
            for ( auto & fse : bodyBlock->finalList ) {
                blk->list.push_back(fse->clone());
            }
        }
        // loop &= _builtin_iterator_close(it0,pvar0)
        for ( size_t si=0; si!=expr->sources.size(); ++si ) {
            const string & srcName = srcNames[si];
            const string & pVarName = pVarNames[si];
            auto cbif = make_shared<ExprCall>(expr->at, "_builtin_iterator_close");
            cbif->arguments.push_back(make_shared<ExprVar>(expr->at, srcName));
            cbif->arguments.push_back(make_shared<ExprVar>(expr->at, pVarName));
            blk->list.push_back(cbif);
        }
        verifyGenerated(blk);
        return blk;
    }
}

