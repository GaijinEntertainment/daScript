#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {


    class TrackVariableFlags : public Visitor {
    protected:
        // global let
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            var->access_extern = false;
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
            var->access_pass = false;
        }
        virtual void preVisitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitGlobalLetInit(var, init);
            var->access_init = true;
        }
        // let
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            var->access_extern = false;
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
            var->access_pass = false;
        }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitLetInit(let, var, init);
            var->access_init = true;
        }
        // function arguments
        virtual void preVisitArgument ( Function * fn, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitArgument(fn, var, lastArg);
            var->access_extern = true;
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
            var->access_pass = false;
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitArgumentInit(fn, var, init);
            var->access_init = true;
        }
        // block
        virtual void preVisitBlockArgument ( ExprBlock * block, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitBlockArgument(block, var, lastArg);
            var->access_extern = true;
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
            var->access_pass = false;
        }
        // for loop sources
        virtual void preVisitFor ( ExprFor *, const VariablePtr & var, bool ) override {
            var->access_init = true;
        }
        // var
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->write ) {
                expr->variable->access_ref = true;
            } else {
                if ( !expr->r2v ) {
                    expr->variable->access_pass = true;
                }
                expr->variable->access_get = true;
            }
        }
    };

    /*
     TODO:
        cond ? a : b
        a ?? b
        ?.
        ->
     */

    // here we propagate r2cr flag
    //  a.@b    ->  $a.@b
    //  a@[b]   ->  $a@[b]
    //  a.b.@c  ->  $a.$b.@c
    //  a = 5   ->  #a = 5
    //  a.b = 5 ->  #a#.b=5
    //  a[b]=3  ->  #a#[b]=3
    class TrackFieldAndAtFlags : public Visitor {
        TextWriter &            logs;
        set<const Function *>   asked;
        FunctionPtr             func;
    public:
        TrackFieldAndAtFlags ( TextWriter & l ) : logs(l) {
        }
        void MarkSideEffects ( Module & mod ) {
            for (auto & fnI : mod.functions) {
                auto & fn = fnI.second;
                if (!fn->builtIn) {
                    fn->knownSideEffects = false;
                    fn->sideEffectFlags &= ~uint32_t(SideEffects::inferedSideEffects);
                }
            }
            for (auto & fnI : mod.functions) {
                auto & fn = fnI.second;
                if (!fn->builtIn && !fn->knownSideEffects) {
                    asked.clear();
                    getSideEffects(fn);
                }
            }
        }
    protected:
        void propagateRead ( Expression * expr ) {
            if ( expr->rtti_isVar() ) {
                auto var = (ExprVar *) expr;
                var->r2cr = true;
                if ( var->variable->source ) {
                    propagateRead(var->variable->source.get());
                }
            } else if ( expr->rtti_isField() || expr->rtti_isSafeField() ) {
                auto field = (ExprField *) expr;
                field->r2cr = true;
                propagateRead(field->value.get());
            } else if ( expr->rtti_isSwizzle() ) {
                auto swiz = (ExprSwizzle *) expr;
                swiz->r2cr = true;
                propagateRead(swiz->value.get());
            }else if ( expr->rtti_isAt() ) {
                auto at = (ExprAt *) expr;
                at->r2cr = true;
                propagateRead(at->subexpr.get());
            } else if ( expr->rtti_isOp3() ) {
                auto op3 = (ExprOp3 *) expr;
                propagateRead(op3->left.get());
                propagateRead(op3->right.get());
            } else if ( expr->rtti_isNullCoalescing() ) {
                auto nc = (ExprNullCoalescing *) expr;
                propagateRead(nc->subexpr.get());
                propagateRead(nc->defaultValue.get());
            } else if ( expr->rtti_isValues() ) {
                auto vs = (ExprValues *) expr;
                propagateRead(vs->arguments[0].get());
            }
            // TODO:
            //  propagate read to call or expr-like-call???
            //  do we need to?
        }
        void propagateWrite ( Expression * expr ) {
            if ( expr->rtti_isVar() ) {
                auto var = (ExprVar *) expr;
                var->write = true;
                if ( var->variable->source ) {
                    propagateWrite(var->variable->source.get());
                }
            } else if ( expr->rtti_isField() || expr->rtti_isSafeField() ) {
                auto field = (ExprField *) expr;
                field->write = true;
                propagateWrite(field->value.get());
            } else if ( expr->rtti_isSwizzle() ) {
                auto swiz = (ExprField *) expr;
                swiz->write = true;
                propagateWrite(swiz->value.get());
            }else if ( expr->rtti_isAt() ) {
                auto at = (ExprAt *) expr;
                at->write = true;
                propagateWrite(at->subexpr.get());
            } else if ( expr->rtti_isOp3() ) {
                auto op3 = (ExprOp3 *) expr;
                propagateWrite(op3->left.get());
                propagateWrite(op3->right.get());
            } else if ( expr->rtti_isNullCoalescing() ) {
                auto nc = (ExprNullCoalescing *) expr;
                propagateWrite(nc->subexpr.get());
                propagateWrite(nc->defaultValue.get());
            } else if ( expr->rtti_isValues() ) {
                auto vs = (ExprValues *) expr;
                propagateWrite(vs->arguments[0].get());
            }
            // TODO:
            //  propagate write to call or expr-like-call???
            //  do we need to?
        }
        uint32_t getSideEffects ( const FunctionPtr & fnc ) {
            if ( fnc->builtIn || fnc->knownSideEffects ) {
                return fnc->sideEffectFlags;
            }
            if ( asked.find(fnc.get())!=asked.end() ) {
                if ( func != fnc ) {
                    logs << "optimization warning, assuming " << fnc->name << " has no side effects during the infer loop\n";
                }
                return 0;
            }
            asked.insert(fnc.get());
            auto sfn = func;
            func = fnc;
            fnc->visit(*this);
            TrackVariableFlags vaf;
            fnc->visit(vaf);
            func = sfn;
            // now, for the side-effects
            uint32_t flags = fnc->sideEffectFlags;
            if (fnc->useGlobalVariables.size()) {
                flags |= uint32_t(SideEffects::accessGlobal);
            }
            // it has side effects, if it writes to its arguments
            for ( auto & arg : fnc->arguments ) {
                if ( arg->access_ref ) {
                    flags |= uint32_t(SideEffects::modifyArgument);
                }
            }
            for ( auto & dep : fnc->useFunctions ) {
                if ( dep != fnc ) {
                    uint32_t depFlags = getSideEffects(dep);
                    depFlags &= ~uint32_t(SideEffects::modifyArgument);
                    flags |= depFlags;
                }
            }
            fnc->knownSideEffects = true;
            fnc->sideEffectFlags |= flags;
            return flags;
        }
    protected:
    // Variable initializatoin
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitLetInit(let, var, init);
            // TODO:
            //  at some point we should do better data trackng for this type of aliasing
            if ( var->type->ref ) propagateWrite(init);
        }
    // ExprField
        virtual void preVisit ( ExprField * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagateRead(expr->value.get());
        }
    // ExprSafeField
        virtual void preVisit ( ExprSafeField * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagateRead(expr->value.get());
        }
    // ExprAt
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagateRead(expr->subexpr.get());
        }
    // ExprMove
        virtual void preVisit ( ExprMove * expr ) override {
            Visitor::preVisit(expr);
            propagateWrite(expr->left.get());
            propagateWrite(expr->right.get());
        }
    // ExprCopy
        virtual void preVisit ( ExprCopy * expr ) override {
            Visitor::preVisit(expr);
            propagateWrite(expr->left.get());
        }
    // Op1
        virtual void preVisit ( ExprOp1 * expr ) override {
            Visitor::preVisit(expr);
            auto sef = getSideEffects(expr->func);
            if ( sef & uint32_t(SideEffects::modifyArgument) ) {
                propagateWrite(expr->subexpr.get());
            }
        }
    // Op2
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            auto sef = getSideEffects(expr->func);
            if ( sef & uint32_t(SideEffects::modifyArgument) ) {
                auto leftT = expr->left->type;
                if ( leftT->isRef() && !leftT->isConst() ) {
                    propagateWrite(expr->left.get());
                }
                auto rightT = expr->right->type;
                if ( rightT->isRef() && !rightT->isConst() ) {
                    propagateWrite(expr->right.get());
                }
            }
        }
    // Op3
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            auto sef = expr->func ? getSideEffects(expr->func) : 0;
            if ( sef & uint32_t(SideEffects::modifyArgument) ) {
                auto condT = expr->subexpr->type;
                if ( condT->isRef() && !condT->isConst() ) {
                    propagateWrite(expr->subexpr.get());
                }
                auto leftT = expr->left->type;
                if ( leftT->isRef() && !leftT->isConst() ) {
                    propagateWrite(expr->left.get());
                }
                auto rightT = expr->right->type;
                if ( rightT->isRef() && !rightT->isConst() ) {
                    propagateWrite(expr->right.get());
                }
            }
        }
    // Return
        virtual void preVisit ( ExprReturn * expr ) override {
            Visitor::preVisit(expr);
            // TODO:
            //  at some point we should do better data trackng for this type of aliasing
            if ( expr->returnReference ) propagateWrite(expr->subexpr.get());
        }
    // New
        virtual void preVisit ( ExprNew * expr ) override {
            Visitor::preVisit(expr);
            bool newExternal = false;
            auto NT = expr->typeexpr;
            if ( NT->baseType==Type::tHandle ) {
                newExternal = true;
            }
            if ( newExternal ) {
                func->sideEffectFlags |= uint32_t(SideEffects::modifyExternal);
            }
            if ( expr->initializer ) {
                // if modified, modify CALL
                auto sef = getSideEffects(expr->func);
                if ( sef & uint32_t(SideEffects::modifyArgument) ) {
                    for ( size_t ai=0; ai != expr->arguments.size(); ++ai ) {
                        const auto & argT = expr->func->arguments[ai]->type;
                        if ( argT->isRef() && !argT->isConst() ) {
                            if ( expr->func->knownSideEffects && !expr->func->builtIn ) {
                                if ( expr->func->arguments[ai]->access_ref ) {
                                    propagateWrite(expr->arguments[ai].get());
                                }
                            } else {
                                propagateWrite(expr->arguments[ai].get());
                            }
                        }
                    }
                }
            }
        }
    // Delete
        virtual void preVisit ( ExprDelete * expr ) override {
            Visitor::preVisit(expr);
            propagateWrite(expr->subexpr.get());
            bool deleteExternal = false;
            auto NT = expr->subexpr->type;
            if ( NT->baseType==Type::tHandle ) {
                deleteExternal = true;
            } else if ( NT->baseType==Type::tPointer && NT->firstType && NT->firstType->isHandle() ) {
                deleteExternal = true;
            }
            if ( deleteExternal ) {
                func->sideEffectFlags |= uint32_t(SideEffects::modifyExternal);
            }
        }
    // Call
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            // if modified, modify NEW
            auto sef = getSideEffects(expr->func);
            if ( sef & uint32_t(SideEffects::modifyArgument) ) {
                for ( size_t ai=0; ai != expr->arguments.size(); ++ai ) {
                    const auto & argT = expr->func->arguments[ai]->type;
                    if ( (argT->isRef() || argT->baseType==anyArgument) && !argT->isConst() ) {
                        if ( expr->func->knownSideEffects && !expr->func->builtIn ) {
                            if ( expr->func->arguments[ai]->access_ref ) {
                                propagateWrite(expr->arguments[ai].get());
                            }
                        } else {
                            propagateWrite(expr->arguments[ai].get());
                        }
                    }
                }
            }
        }
    // LooksLikeCall
        virtual void preVisit ( ExprLooksLikeCall * expr ) override {
            Visitor::preVisit(expr);
            for ( size_t ai=0; ai != expr->arguments.size(); ++ai ) {
                const auto & argT = expr->arguments[ai]->type;
                if ( argT->isRef() && !argT->isConst() ) {  // should we propagate const?
                    propagateWrite(expr->arguments[ai].get());
                }
            }
        }
    // Invoke
        virtual void preVisit(ExprInvoke * expr) override{
            Visitor::preVisit(expr);
            if ( func ) {
                func->sideEffectFlags |= uint32_t(SideEffects::invokeBloke);
            }
        }
    // Debug
        virtual void preVisit(ExprDebug * expr) override {
            Visitor::preVisit(expr);
            if (func) {
                func->sideEffectFlags |= uint32_t(SideEffects::modifyExternal);
            }
        }
    };


    class RemoveUnusedLocalVariables : public OptVisitor {
    protected:
    // ExprLet
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !var->access_get && !var->access_ref && !var->access_init && !var->access_pass ) {
                reportFolding();
                return nullptr;
            }
            if ( !var->access_ref && !var->access_pass && var->init && var->init->constexpression ) {
                reportFolding();
                return nullptr;
            }
            if ( !var->access_ref && !var->access_pass && !var->init && var->type->isFoldable() ) {
                // uninitialized read-only foldable var is const 0
                reportFolding();
                return nullptr;
            }
            if ( !var->access_ref && !var->access_pass && !var->access_get && var->init->noSideEffects ) {
                reportFolding();
                return nullptr;
            }
            return Visitor::visitLet(let,var,last);
        }
    // ExprVar
        virtual ExpressionPtr visit ( ExprVar * expr ) override {
            if ( !expr->variable->access_ref && !expr->variable->access_extern ) {
                if ( expr->r2v ) {
                    if ( expr->variable->init ) {
                        if ( expr->variable->init->constexpression ) {
                            reportFolding();
                            return expr->variable->init->clone();
                        }
                    } else {
                        if ( expr->type->isFoldable() && !expr->variable->access_init ) {
                            reportFolding();
                            auto exprV = Program::makeConst(expr->at, expr->type, v_zero());
                            exprV->type = make_shared<TypeDecl>(*expr->type);
                            return exprV;
                        }
                    }
                }
            }
            return Visitor::visit(expr);
        }
    // ExprFor
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            // TODO: how do we determine, if iteration count is not used?
            //  also, how do we determine, if native iterator has side-effect?
            /*
            auto itV = expr->iteratorVariables.begin();
            auto itS = expr->sources.begin();
            while ( itV != expr->iteratorVariables.end() ) {
                auto & var = *itV;
                if ( !var->access_ref && !var->access_get ) {
                    itV = expr->iteratorVariables.erase(itV);
                    itS = expr->sources.erase(itS);
                    anyFolding = true;
                } else {
                    itV ++;
                    itS ++;
                }
            }
            */
            return Visitor::visit(expr);
        }
    };

    // program

    void Program::buildAccessFlags(TextWriter & logs) {
        markSymbolUse(true);
        // determine function side-effects
        TrackFieldAndAtFlags faf(logs);
        faf.MarkSideEffects(*thisModule);
    }

    bool Program::optimizationUnused(TextWriter & logs) {
        buildAccessFlags(logs);
        // remove itselft
        RemoveUnusedLocalVariables context;
        visit(context);
        return context.didAnything();
    }
}

