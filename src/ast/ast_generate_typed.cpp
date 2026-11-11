#include "daScript/misc/platform.h"

#include "ast_generate_typed.h"

namespace das {

    Function * findBuiltinOperator ( const ModuleLibrary & library, const char * opName, TypeDecl * t0, TypeDecl * t1 ) {
        if ( !t0 ) return nullptr;
        Function * result = nullptr;
        uint64_t hName = hash64z(opName);
        size_t nArgs = t1 ? 2 : 1;
        library.foreach([&](Module * mod) -> bool {
            if ( auto kv = mod->functionsByName.find(hName) ) {
                for ( auto fn : kv->second ) {
                    if ( !fn->builtIn || fn->hasObservableEffects() || fn->isTemplate ) continue;
                    if ( fn->arguments.size()!=nArgs ) continue;
                    if ( !fn->arguments[0]->type->isSameType(*t0, RefMatters::no, ConstMatters::no, TemporaryMatters::no) ) continue;
                    if ( t1 && !fn->arguments[1]->type->isSameType(*t1, RefMatters::no, ConstMatters::no, TemporaryMatters::no) ) continue;
                    result = fn;
                    return false;
                }
            }
            return true;
        }, "*");
        return result;
    }

    ExpressionPtr makeTypedConst ( const LineInfo & at, TypeDecl * type, vec4f value ) {
        auto res = Program::makeConst(at, type, value);
        if ( !res ) return nullptr;
        res->type = type;
        res->type->ref = false;
        return res;
    }

    ExprOp1 * makeTypedOp1 ( const LineInfo & at, const char * op, Function * func, Expression * sub, TypeDecl * type ) {
        auto res = new ExprOp1(at, op, sub);
        res->func = func;
        res->type = type;
        res->type->ref = false;
        return res;
    }

    ExprVar * makeLocalRead ( const LineInfo & at, Variable * var ) {
        auto res = new ExprVar(at, var->name);
        res->variable = var;
        res->local = true;
        res->type = new TypeDecl(*var->type);
        res->type->ref = true;
        return res;
    }

    TypeDecl * inferLetInitType ( TypeDecl * autoT, TypeDecl * initT, bool viaClone, bool relaxedPointerConst ) {
        auto varT = TypeDecl::inferGenericInitType(autoT, initT);
        if ( !varT || varT->isAlias() ) return nullptr;
        varT->ref = false;
        if ( viaClone && varT->isString() ) varT->temporary = false;
        TypeDecl::applyAutoContracts(varT, autoT);
        if ( !relaxedPointerConst && varT->isPointer() && varT->firstType && !varT->constant && initT->constant ) {
            varT->firstType->constant = true;
        }
        return varT;
    }

    bool markSafeTableLookupInit ( Expression * init ) {
        if ( init->rtti_isR2V() ) init = static_cast<ExprRef2Value *>(init)->subexpr;
        if ( !init->rtti_isAt() ) return false;
        auto at = static_cast<ExprAt *>(init);
        if ( !at->subexpr->type || !at->subexpr->type->isGoodTableType() || at->alwaysSafe ) return false;
        at->alwaysSafe = true;
        return true;
    }

    Expression * makeTypeWitnessZero ( const LineInfo & at, TypeDecl * witnessT ) {
        auto vt = new TypeDecl(*witnessT);
        vt->ref = false;
        if ( vt->isEnum() ) {
            auto anyEnumValue = vt->enumType->list.size() ? vt->enumType->list[0].name : "";
            auto res = new ExprConstEnumeration(at, anyEnumValue, vt);
            res->type = vt;
            return res;
        }
        if ( vt->isWorkhorseType() ) return makeTypedConst(at, vt, v_zero());
        return nullptr;
    }

    void markUnderClone ( Expression * left ) {
        if ( left->rtti_isField() ) static_cast<ExprField *>(left)->underClone = true;
        else if ( left->rtti_isVar() ) static_cast<ExprVar *>(left)->underClone = true;
        else if ( left->rtti_isAt() ) static_cast<ExprAt *>(left)->underClone = true;
    }

    void markScopesEarlyOut ( const vector<ExprBlock *> & scopes ) {
        for ( auto i = scopes.size(); i>0; ) {
            --i;
            scopes[i]->hasEarlyOut = true;
            if ( scopes[i]->isClosure ) break;
        }
    }

    void resetBlockFlow ( ExprBlock * block ) {
        block->hasEarlyOut = false;
        block->hasReturn = false;
        block->forLoop = false;
    }

    void setLetVisibility ( ExprLet * let, ExprBlock * scope ) {
        let->visibility.fileInfo = let->at.fileInfo;
        let->visibility.column = let->atInit.last_column;
        let->visibility.line = let->atInit.last_line;
        let->visibility.last_column = scope->at.last_column;
        let->visibility.last_line = scope->at.last_line;
    }

    void setLetEarlyOut ( ExprLet * let, bool hasEarlyOut ) {
        let->hasEarlyOut = hasEarlyOut;
        for ( auto & var : let->variables ) var->early_out = hasEarlyOut;
    }

    Expression * makeValue ( Expression * e ) {
        return Expression::autoDereference(e);
    }

    ExprConstBool * makeConstBool ( const LineInfo & at, bool value ) {
        auto c = new ExprConstBool(at, value);
        c->type = new TypeDecl(Type::tBool, at);
        c->type->constant = true;
        return c;
    }

    ExprOp1 * makeNot ( const LineInfo & at, Function * notFn, Expression * cond ) {
        cond->markNoDiscard();
        return makeTypedOp1(at, "!", notFn, makeValue(cond), new TypeDecl(Type::tBool, at));
    }

    ExprIfThenElse * makeIf ( const LineInfo & at, Expression * cond, Expression * ifTrue, Expression * ifFalse ) {
        cond->markNoDiscard();
        return new ExprIfThenElse(at, makeValue(cond), ifTrue, ifFalse);
    }

    ExprCopy * makeCopy ( const LineInfo & at, Expression * left, Expression * right ) {
        right->markNoDiscard();
        auto c = new ExprCopy(at, left, right, true);
        c->type = new TypeDecl();
        return c;
    }

    ExprMove * makeMove ( const LineInfo & at, Expression * left, Expression * right ) {
        right->markNoDiscard();
        auto m = new ExprMove(at, left, right, true);
        m->type = new TypeDecl();
        return m;
    }
}
