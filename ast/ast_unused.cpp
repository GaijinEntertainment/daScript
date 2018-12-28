#include "precomp.h"

#include "ast.h"

namespace yzg {
    
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
            } else if ( expr->rtti_isAt() ) {
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
            } else if ( expr->rtti_isAt() ) {
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
    protected:
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
            if ( !expr->func->noSideEffects ) propagateWrite(expr->subexpr.get());
        }
    // Op2
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->func->noSideEffects ) {
                propagateWrite(expr->left.get());
            }
        }
    // Call
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->func->noSideEffects ) {
                for ( int ai=0; ai != expr->arguments.size(); ++ai ) {
                    const auto & argT = expr->func->arguments[ai]->type;
                    if ( argT->isRef() && !argT->isConst() ) {
                        propagateWrite(expr->arguments[ai].get());
                    }
                }
            }
        }
    // LooksLikeCall
        virtual void preVisit ( ExprLooksLikeCall * expr ) override {
            Visitor::preVisit(expr);
            for ( int ai=0; ai != expr->arguments.size(); ++ai ) {
                const auto & argT = expr->arguments[ai]->type;
                if ( argT->isRef() && !argT->isConst() ) {  // should we propagate const?
                    propagateWrite(expr->arguments[ai].get());
                }
            }
        }
    };
    
    class TrackVariableFlags : public Visitor {
    protected:
    // global let
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            var->access_extern = false;
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
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
        }
    // for loop sources
        virtual void preVisitFor ( ExprFor * expr, const VariablePtr & var, bool last ) override {
            var->access_init = true;
        }
    // var
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->write ) {
                expr->variable->access_ref = true;
            } else {
                expr->variable->access_get = true;
            }
        }
    };
    
    class RemoveUnusedLocalVariables : public OptVisitor {
    protected:
    // ExprLet
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !var->access_get && !var->access_ref && !var->access_init ) {
                reportFolding();
                return nullptr;
            }
            if ( !var->access_ref && var->init && var->init->constexpression ) {
                reportFolding();
                return nullptr;
            }
            if ( !var->access_ref && !var->init && var->type->isFoldable() ) {
                // uninitialized read-only foldable var is const 0
                reportFolding();
                return nullptr;
            }
            if ( !var->access_ref && !var->access_get && var->init->noSideEffects ) {
                reportFolding();
                return nullptr;
            }
            return Visitor::visitLet(let,var,last);
        }
    // ExprVar
        virtual ExpressionPtr visit ( ExprVar * expr ) override {
            if ( !expr->variable->access_ref && !expr->variable->access_extern ) {
                if ( expr->variable->init ) {
                    if ( expr->variable->init->constexpression ) {
                        reportFolding();
                        return expr->variable->init->clone();
                    }
                } else {
                    if ( expr->type->isFoldable() && !expr->variable->access_init ) {
                        reportFolding();
                        return Program::makeConst(expr->at, expr->type, _mm_setzero_ps());
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
    
    bool Program::optimizationUnused() {
        TrackFieldAndAtFlags faf;
        visit(faf);
        TrackVariableFlags vaf;
        visit(vaf);
        RemoveUnusedLocalVariables context;
        visit(context);
        return context.didAnything();
    }
}

