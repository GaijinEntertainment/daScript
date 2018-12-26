#include "precomp.h"

#include "ast.h"

/*
 TODO:
    // ExprAssert
        assert(true)    ->  nop
 */

namespace yzg {
    
    class FoldingVisitor : public Visitor {
    public:
        FoldingVisitor( const ProgramPtr & prog ) : ctx(nullptr) {
            program = prog;
        }
        bool didAnything () const { return anyFolding; }
    protected:
        Context     ctx;
        ProgramPtr  program;
        bool        anyFolding = false;
    protected:
        __m128 eval ( Expression * expr ) {
            ctx.simEnd();
            ctx.restart();
            auto node = expr->simulate(ctx);
            ctx.simEnd();
            ctx.restart();
            __m128 result = node->eval(ctx);
            if ( ctx.getException() ) {
                program->error("internal error, failed to fold constant", expr->at );
                return _mm_setzero_ps();
            }
            anyFolding = true;
            return result;
        }
        ExpressionPtr evalAndFold ( Expression * expr ) {
            auto sim = Program::makeConst(expr->at, expr->type, eval(expr));
            sim->constexpression = true;
            return sim;
        }
        bool isSameFoldValue ( const TypeDeclPtr & t, __m128 a, __m128 b ) const {
            return memcmp(&a,&b,t->getSizeOf()) == 0;
        }
    };
    
    /*
        op1 constexpr = op1(constexpr)
        op2 constexpr,constexpr = op2(constexpr,constexpr)
        op3 constexpr,a,b = constexpr ? a or b
        op3 cond,a,a = a
        if ( constexpr ) a; else b;    =   constexpr ? a : b
        constexpr; = nop
     */
    class ConstFolding : public FoldingVisitor {
    public:
        ConstFolding( const ProgramPtr & prog ) : FoldingVisitor(prog) {}
    protected:
    // op1
        virtual ExpressionPtr visit ( ExprOp1 * expr ) override {
            if ( expr->type->isFoldable() && expr->subexpr->constexpression ) {
                return evalAndFold(expr);
            }
            return Visitor::visit(expr);
        }
    // op2
        virtual ExpressionPtr visit ( ExprOp2 * expr ) override {
            if ( expr->type->isFoldable() && expr->left->constexpression && expr->right->constexpression ) {
                return evalAndFold(expr);
            }
            return Visitor::visit(expr);
        }
    // op3
        virtual ExpressionPtr visit ( ExprOp3 * expr ) override {
            if ( expr->type->isFoldable() && expr->subexpr->constexpression && expr->left->constexpression && expr->right->constexpression ) {
                return evalAndFold(expr);
            } else if ( expr->type->isFoldable() && expr->subexpr->noSideEffects && expr->left->constexpression && expr->right->constexpression ) {
                __m128 left = eval(expr->left.get());
                __m128 right = eval(expr->right.get());
                if ( isSameFoldValue(expr->type, left, right) ) {
                    anyFolding = true;
                    return expr->left->clone();
                }
            } else if ( expr->subexpr->constexpression ) {
                bool res = cast<bool>::to(eval(expr->subexpr.get()));
                anyFolding = true;
                return res ? expr->left : expr->right;
            }
            return Visitor::visit(expr);
        }
    // ExprIfThenElse
        virtual ExpressionPtr visit ( ExprIfThenElse * expr ) override {
            if ( expr->cond->constexpression ) {
                bool res = cast<bool>::to(eval(expr->cond.get()));
                anyFolding = true;
                return res ? expr->if_true : expr->if_false;
            }
            return Visitor::visit(expr);
        }
    // block
        virtual ExpressionPtr visitBlockExpression (  ExprBlock * block, Expression * expr ) override {
            if ( expr->constexpression ) {  // top level constant expresson like 4;
                anyFolding = true;
                return nullptr;
            }
            return Visitor::visitBlockExpression(block, expr);
        }
    // const
        virtual ExpressionPtr visit ( ExprConst * c ) override {
            c->constexpression = true;
            return Visitor::visit(c);
        }
    };
    
    class StaticAssertFolding : public FoldingVisitor {
    public:
        StaticAssertFolding( const ProgramPtr & prog ) : FoldingVisitor(prog) {}
    protected:
        virtual ExpressionPtr visit ( ExprStaticAssert * expr ) override {
            auto cond = expr->arguments[0];
            if ( !cond->constexpression  ) {
                program->error("static assert condition is not constexpr", expr->at);
                return nullptr;
            }
            if ( !cast<bool>::to(eval(cond.get())) ) {
                string message;
                if ( expr->arguments.size()==2 ) {
                    message = cast<char *>::to(eval(expr->arguments[1].get()));
                } else {
                    message = "static assert failed";
                }
                program->error(message, expr->at);
            }
            return nullptr;
        }
    };
    
    // program
    
    bool Program::optimizationConstFolding() {
        ConstFolding context(shared_from_this());
        visit(context);
        return context.didAnything();
    }
    
    bool Program::staticAsserts() {
        StaticAssertFolding context(shared_from_this());
        visit(context);
        return context.didAnything();
    }
}

