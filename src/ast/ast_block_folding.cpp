#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {
    
    // this folds the following, by setting r2v flag on expressions
    //  r2v(var)            = @var
    //  r2v(expr.field)     = expr.@field
    //  r2v(expr[index])    = expr@[index]
    //  r2v(a ? b : c)      = a ? r2v(b) : r2v(c)
    class RefFolding : public OptVisitor {
    protected:
        virtual ExpressionPtr visit ( ExprRef2Value * expr ) override {
            if ( expr->subexpr->rtti_isVar() ) {
                auto evar = static_pointer_cast<ExprVar>(expr->subexpr);
                evar->r2v = true;
                evar->type->ref = false;
                return evar;
            } else if ( expr->subexpr->rtti_isField() ) {
                auto efield = static_pointer_cast<ExprField>(expr->subexpr);
                efield->r2v = true;
                efield->type->ref = false;
                return efield;
            } else if ( expr->subexpr->rtti_isSwizzle() ) {
                auto eswiz = static_pointer_cast<ExprSwizzle>(expr->subexpr);
                eswiz->r2v = true;
                eswiz->type->ref = false;
                return eswiz;
            }else if ( expr->subexpr->rtti_isSafeField() ) {
                auto efield = static_pointer_cast<ExprSafeField>(expr->subexpr);
                efield->r2v = true;
                efield->type->ref = false;
                return efield;
            } else if ( expr->subexpr->rtti_isAt() ) {
                auto eat = static_pointer_cast<ExprAt>(expr->subexpr);
                eat->r2v = true;
                eat->type->ref = false;
                return eat;
            } else if ( expr->subexpr->rtti_isOp3() ) {
                reportFolding();
                auto op3 = static_pointer_cast<ExprOp3>(expr->subexpr);
                op3->left = Expression::autoDereference(op3->left);
                op3->right = Expression::autoDereference(op3->right);
                op3->type->ref = false;
                return expr->subexpr;
            } else if ( expr->subexpr->rtti_isNullCoalescing() ) {
                reportFolding();
                auto nc = static_pointer_cast<ExprNullCoalescing>(expr->subexpr);
                nc->defaultValue = Expression::autoDereference(nc->defaultValue);
                nc->type->ref = false;
                return nc;
            } else {
                return Visitor::visit(expr);
            }
        }
    };
    
    class BlockFolding : public OptVisitor {
    protected:
        void collect ( vector<ExpressionPtr> & list, ExprBlock * block ) {
            for ( auto & expr : block->list ) {
                if ( expr->rtti_isBreak() || expr->rtti_isReturn() ) {
                    list.push_back(expr);
                    break;
                }
                if ( expr->rtti_isBlock() ) {
                    collect(list, static_cast<ExprBlock *>(expr.get()));
                } else {
                    list.push_back(expr);
                }
            }
        }
    protected:
    // ExprBlock
        virtual ExpressionPtr visit ( ExprBlock * block ) {
            vector<ExpressionPtr> list;
            collect(list, block);
            if ( list!=block->list ) {
                swap ( block->list, list );
                reportFolding();
            }
            return Visitor::visit(block);
        }
    // ExprLet
        virtual ExpressionPtr visit ( ExprLet * let ) {
            if ( let->variables.size()==0 ) {
                reportFolding();
                return let->subexpr;
            }
            return Visitor::visit(let);
        }
    // function
        virtual FunctionPtr visit ( Function * func ) {
            if ( func->body && func->result->isVoid() ) {   // remove trailing return on the void function
                if ( func->body->rtti_isBlock() ) {
                    auto block = static_pointer_cast<ExprBlock>(func->body);
                    if ( block->list.back()->rtti_isReturn() ) {
                        block->list.resize(block->list.size()-1);
                        reportFolding();
                        return Visitor::visit(func);
                    }
                }
            }
            return Visitor::visit(func);
        }
    };
    
    // program
    
    bool Program::optimizationRefFolding() {
        bool any = false, anything = false;
        do {
            RefFolding context;
            visit(context);
            any = context.didAnything();
            anything |= any;
        } while ( any );
        return anything;
    }
    
    bool Program::optimizationBlockFolding() {
        BlockFolding context;
        visit(context);
        return context.didAnything();
    }
}

