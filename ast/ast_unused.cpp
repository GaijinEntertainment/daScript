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
    class TrackFieldAndAtFlags : public Visitor {
        void propagate ( Expression * expr ) {
            if ( expr->rtti_isVar() ) {
                auto var = (ExprVar *) expr;
                var->r2cr = true;
            } else if ( expr->rtti_isField() ) {
                auto field = (ExprField *) expr;
                field->r2cr = true;
                propagate(field->value.get());
            } else if ( expr->rtti_isAt() ) {
                auto at = (ExprAt *) expr;
                at->r2cr = true;
                propagate(at->subexpr.get());
            } else if ( expr->rtti_isOp3() ) {
                auto op3 = (ExprOp3 *) expr;
                propagate(op3->left.get());
                propagate(op3->right.get());
            }
        }
    protected:
        // field
        virtual void preVisit ( ExprField * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagate(expr->value.get());
        }
        // at
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagate(expr->subexpr.get());
        }
    };
    
    class TrackVariableFlags : public Visitor {
    protected:
    // global let
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
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
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitArgumentInit(fn, var, init);
            var->access_init = true;
        }
    // for loop sources
        virtual void preVisitFor ( ExprFor * expr, const VariablePtr & var, bool last ) override {
            var->access_init = true;
        }
    // var
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v || expr->r2cr ) {
                expr->variable->access_get = true;
            } else {
                expr->variable->access_ref = true;
            }
        }
    };
    
    class RemoveUnusedLocalVariables : public Visitor {
    public:
        bool didAnything () const { return anyFolding; }
    protected:
        bool        anyFolding = false;
    protected:
    // ExprLet
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !var->access_get && !var->access_ref && !var->access_init ) {
                anyFolding = true;
                return nullptr;
            }
            if ( !var->access_ref && var->init && var->init->constexpression ) {
                anyFolding = true;
                return nullptr;
            }
            if ( !var->access_ref && !var->init && var->type->isFoldable() ) {  // uninitialized read-only foldable var is const 0
                anyFolding = true;
                return nullptr;
            }
            return Visitor::visitLet(let,var,last);
        }
    // ExprVar
        virtual ExpressionPtr visit ( ExprVar * expr ) override {
            if ( !expr->variable->access_ref ) {
                if ( expr->variable->init ) {
                    if ( expr->variable->init->constexpression ) {
                        anyFolding = true;
                        return expr->variable->init->clone();
                    }
                } else {
                    if ( expr->type->isFoldable() && !expr->variable->access_init ) {
                        anyFolding = true;
                        return Program::makeConst(expr->at, expr->type, _mm_setzero_ps());
                    }
                }
            }
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

