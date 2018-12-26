#include "precomp.h"

#include "ast.h"

namespace yzg {
    
    // this folds the following, by setting r2v flag on expressions
    //  r2v(var)            = @var
    //  r2v(expr.field)     = expr.@field
    //  r2v(expr[index])    = expr@[index]
    class RefFolding : public Visitor {
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
            } else if ( expr->subexpr->rtti_isAt() ) {
                auto eat = static_pointer_cast<ExprAt>(expr->subexpr);
                eat->r2v = true;
                eat->type->ref = false;
                return eat;
            } else {
                return Visitor::visit(expr);
            }
        }
    };
    
    class BlockFolding : public Visitor {
    public:
        bool didAnything () const { return anyFolding; }
    protected:
        bool        anyFolding = false;
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
                anyFolding = true;
            }
            return Visitor::visit(block);
        }
    // ExprLet
        virtual ExpressionPtr visit ( ExprLet * let ) {
            if ( let->variables.size()==0 ) {
                anyFolding = true;
                return let->subexpr;
            }
            return Visitor::visit(let);
        }
    };
    
    // program
    
    void Program::refFolding() {
        RefFolding context;
        visit(context);
    }
    
    bool Program::optimizationBlockFolding() {
        BlockFolding context;
        visit(context);
        return context.didAnything();
    }
}

