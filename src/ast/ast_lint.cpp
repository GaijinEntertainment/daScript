#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_generate.h"

namespace das {

    bool exprReturns ( const ExpressionPtr & expr ) {
        if ( expr->rtti_isReturn() ) {
            return true;
        } else if ( expr->rtti_isBlock() ) {
            auto block = static_pointer_cast<ExprBlock>(expr);
            for ( auto & be : block->list ) {
                if ( be->rtti_isBreak() ) {
                    break;
                }
                if ( exprReturns(be) ) {
                    return true;
                }
            }
        } else if ( expr->rtti_isIfThenElse() ) {
            auto ite = static_pointer_cast<ExprIfThenElse>(expr);
            if ( ite->if_false ) {
                return exprReturns(ite->if_true) && exprReturns(ite->if_false);
            }
        } else if ( expr->rtti_isWith() ) {
            auto wth = static_pointer_cast<ExprWith>(expr);
            return exprReturns(wth->body);
        } else if ( expr->rtti_isWhile() ) {
            auto wh = static_pointer_cast<ExprWhile>(expr);
            return exprReturns(wh->body);
        } else if ( expr->rtti_isFor() ) {
            auto fr = static_pointer_cast<ExprFor>(expr);
            return exprReturns(fr->subexpr);
        }
        return false;
    }

    void Program::lint() {
        if (!options.getOption("lint", true)) {
            return;
        }
        // all control paths return something
        for ( auto & fnT : thisModule->functions ) {
            auto fn = fnT.second;
            if ( !fn->result->isVoid() && !fn->result->isAuto() ) {
                if ( !exprReturns(fn->body) ) {
                    error("not all control paths return value", fn->at, CompilationError::not_all_paths_return_value);
                }
            }
        }
    }
}

