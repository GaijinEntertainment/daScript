#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_expressions.h"
#include "ast_can_raise.h"

namespace das {

    // The listed classes are the ones that are side-effect-free and still throw: `a[i]`
    // bounds-checks, `as` on a variant checks the tag, a dereference and a field read
    // through a pointer check for null, `/` and `%` check for a zero divisor, and a
    // call's body is not analyzed - a C++-bound function's sideEffectFlags say nothing
    // about throwing. The safe spellings (ExprSafeAt, ExprSafeField, ExprSafeAsVariant)
    // have their own visitor hooks, so they do not inherit these answers.
    namespace {
        class CanRaiseVisitor : public Visitor {
        public:
            bool raises = false;
        protected:
            virtual void preVisit ( ExprAt * expr ) override {
                Visitor::preVisit(expr);
                raises = true;
            }
            virtual void preVisit ( ExprAsVariant * expr ) override {
                Visitor::preVisit(expr);
                raises = true;
            }
            virtual void preVisit ( ExprPtr2Ref * expr ) override {
                Visitor::preVisit(expr);
                raises = true;
            }
            virtual void preVisit ( ExprCall * expr ) override {
                Visitor::preVisit(expr);
                raises = true;
            }
            virtual void preVisit ( ExprField * expr ) override {
                Visitor::preVisit(expr);
                if ( expr->value->type && expr->value->type->isPointer() ) raises = true;
            }
            virtual void preVisit ( ExprOp2 * expr ) override {
                Visitor::preVisit(expr);
                if ( expr->op=="/" || expr->op=="%" || expr->op=="/=" || expr->op=="%=" ) raises = true;
            }
        };
    }

    bool canRaise ( Expression * expr ) {
        if ( !expr ) return false;
        CanRaiseVisitor vis;
        expr->visit(vis);
        return vis.raises;
    }

    bool droppable ( Expression * expr ) {
        return expr && expr->noSideEffects && !canRaise(expr);
    }
}
