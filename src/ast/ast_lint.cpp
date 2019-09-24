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
        bool checkOnlyFastAot;
        bool checkAotSideEffects;
        bool checkNoGlobalHeap;
        bool checkNoGlobalVariables;
    public:
        LintVisitor ( const ProgramPtr & prog ) : program(prog) {
            checkOnlyFastAot = program->options.getOption("only_fast_aot", false);
            checkAotSideEffects = program->options.getOption("aot_order_side_effects", false);
            checkNoGlobalHeap = program->options.getOption("no_global_heap", false);
            checkNoGlobalVariables = program->options.getOption("no_global_variables", false);
        }
        void error ( const string & err, const LineInfo & at, CompilationError cerr = CompilationError::unspecified ) const {
            program->error(err,at,cerr);
        }
    protected:
        void verifyOnlyFastAot ( Function * func, const LineInfo & at ) {
            if ( !checkOnlyFastAot ) return;
            if ( func && func->builtIn ) {
                auto bif = (BuiltInFunction *) func;
                if ( bif->cppName.empty() ) {
                    program->error(func->describe() + " has no cppName while onlyFastAot option is set", at,
                                   CompilationError::only_fast_aot_no_cpp_name );
                }
            }
        }
        virtual ExpressionPtr visitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            if ( checkNoGlobalHeap ) {
                if ( !init->type->isNoHeapType() ) {
                    program->error("variable " + var->name + " uses heap, which is disabled via option no_global_heap",
                        var->at, CompilationError::no_global_heap );
                }
            }
            if ( checkNoGlobalVariables ) {
                if ( !var->type->isConst() ) {
                    program->error("variable " + var->name + " is not a constant, which is disabled via option no_global_variables",
                        var->at, CompilationError::no_global_variables );
                }
            }
            return Visitor::visitGlobalLetInit(var, init);
        }
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
            if ( checkAotSideEffects ) {
                if ( expr->arguments.size()>1 ) {
                    for ( auto & arg : expr->arguments ) {
                        if ( !arg->noNativeSideEffects ) {
                            program->error("side effects may affect function " + expr->func->name + " evaluation order", expr->at,
                                           CompilationError::aot_side_effects );
                            break;
                        }
                    }
                }
            }
        }
        virtual void preVisit ( ExprOp1 * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
        }
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
            if ( checkAotSideEffects ) {
                if ( !expr->left->noNativeSideEffects || !expr->right->noNativeSideEffects ) {
                    program->error("side effects may affect evaluation order", expr->at,
                                   CompilationError::aot_side_effects );
                }
            }
        }
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
            if ( checkAotSideEffects ) {
                if ( !expr->subexpr->noNativeSideEffects || !expr->left->noNativeSideEffects || !expr->right->noNativeSideEffects ) {
                    program->error("side effects may affect evaluation order", expr->at,
                                   CompilationError::aot_side_effects );
                }
            }
        }
        virtual void preVisit ( ExprCopy * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
            // @E1 = E2, E2 side effects are before E1 side effects
            /*
            if ( checkAotSideEffects ) {
                if ( !expr->left->noNativeSideEffects || !expr->right->noNativeSideEffects ) {
                    program->error("side effects may affect copy evaluation order", expr->at,
                                   CompilationError::aot_side_effects );
                }
            }
            */
        }
        virtual void preVisit ( ExprMove * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
            if ( checkAotSideEffects ) {
                if ( !expr->left->noNativeSideEffects || !expr->right->noNativeSideEffects ) {
                    program->error("side effects may affect move evaluation order", expr->at,
                                   CompilationError::aot_side_effects );
                }
            }
        }
        virtual void preVisit ( ExprClone * expr ) override {
            Visitor::preVisit(expr);
            verifyOnlyFastAot(expr->func, expr->at);
            if ( checkAotSideEffects ) {
                if ( !expr->left->noNativeSideEffects || !expr->right->noNativeSideEffects ) {
                    program->error("side effects may affect clone evaluation order", expr->at,
                                   CompilationError::aot_side_effects );
                }
            }
        }
        virtual void preVisit ( ExprAssert * expr ) override {
            Visitor::preVisit(expr);
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

