#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_visitor.h"

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

    class LintVisitor : public Visitor {
    public:
        LintVisitor ( const ProgramPtr & prog ) : program(prog) {}
        void error ( const string & err, const LineInfo & at, CompilationError cerr = CompilationError::unspecified ) const {
            program->error(err,at,cerr);
        }
    protected:
        virtual void preVisit ( ExprAssert * expr ) {
            if ( !expr->isVerify && !expr->arguments[0]->noSideEffects ) {
                error("assert expressions can't have side-effects (use verify instead)", expr->at,
                      CompilationError::assert_with_side_effects);
            }
        }
    protected:
        ProgramPtr program;
    };

    void Program::lint() {
        if (!options.getOption("lint", true)) {
            return;
        }
        TextWriter logs;
        buildAccessFlags(logs);
        checkSideEffects();
        // lint it
        LintVisitor lintV(shared_from_this());
        visit(lintV);
        // all control paths return something
        for ( auto & fnT : thisModule->functions ) {
            auto fn = fnT.second;
            if ( !fn->result->isVoid() && !fn->result->isAuto() ) {
                if ( !exprReturns(fn->body) ) {
                    error("not all control paths return value", fn->at,
                          CompilationError::not_all_paths_return_value);
                }
            }
        }
    }
}

