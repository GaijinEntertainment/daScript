#include "precomp.h"

#include "ast.h"

namespace yzg {
    
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
    };
    
    // program
    
    bool Program::optimizationBlockFolding() {
        BlockFolding context;
        visit(context);
        return context.didAnything();
    }
}

