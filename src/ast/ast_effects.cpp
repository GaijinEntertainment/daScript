#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
#include "ast_effects.h"

namespace das {

    bool nodeRaises ( Expression * expr ) {
        if ( expr->rtti_isAsVariant() || expr->rtti_isInvoke() || expr->rtti_isAssert() ) return true;
        if ( expr->rtti_isAt() ) {
            return !static_cast<ExprAt *>(expr)->noBoundCheck;
        }
        if ( expr->rtti_isPtr2Ref() ) {
            return !static_cast<ExprPtr2Ref *>(expr)->unsafeDeref;
        }
        if ( expr->rtti_isField() ) {
            auto field = static_cast<ExprField *>(expr);
            return !field->unsafeDeref && field->value->type && field->value->type->isPointer();
        }
        return false;
    }

    bool callRaises ( Function * fn ) {
        return fn && (fn->mayRaise() || (!fn->builtIn && !fn->knownSideEffects));
    }

    bool droppable ( Expression * expr ) {
        return expr && expr->noSideEffects && !expr->mayRaise;
    }
}
