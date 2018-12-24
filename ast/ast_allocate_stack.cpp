#include "precomp.h"

#include "ast.h"

namespace yzg {
    
    class AllocateStack : public Visitor {
    public:
        AllocateStack( const ProgramPtr & prog ) {
            program = prog;
        }
        int getFuncCount() const { return totalFunctions; }
    protected:
        ProgramPtr              program;
        FunctionPtr             func;
        uint32_t                stackTop = 0;
        vector<uint32_t>        stackTopStack;
        int                     totalFunctions = 0;
    protected:
        void pushStack()    { stackTopStack.push_back(stackTop); }
        void popStack()     { stackTop = stackTopStack.back(); stackTopStack.pop_back(); }
    protected:
    // function
        virtual void preVisit ( Function * f ) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
            func->totalStackSize = stackTop = sizeof(Prologue);
            func->index = totalFunctions ++;
        }
        virtual FunctionPtr visit ( Function * that ) override {
            func.reset();
            return Visitor::visit(that);
        }
    // ExprBlock
        virtual void preVisit ( ExprBlock * block ) override {
            pushStack();
            Visitor::preVisit(block);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            popStack();
            return Visitor::visit(block);
        }
    // ExprFor
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            pushStack();
        }
        virtual void preVisitForStack ( ExprFor * expr ) override {
            Visitor::preVisitForStack(expr);
            for ( auto & var : expr->iteratorVariables ) {
                var->stackTop = stackTop;
                stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
            }
        }
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            func->totalStackSize = max(func->totalStackSize, stackTop);
            popStack();
            return Visitor::visit(expr);
        }
    // ExprLet
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
            pushStack();
        }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            var->stackTop = stackTop;
            stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
            return Visitor::visitLet(expr,var,last);
        }
        virtual ExpressionPtr visit ( ExprLet * expr ) override {
            func->totalStackSize = max(func->totalStackSize, stackTop);
            if ( expr->scoped ) {
                popStack();
            }
            return Visitor::visit(expr);
        }
    };
    
    // program
    
    void Program::allocateStack() {
        AllocateStack context(shared_from_this());
        visit(context);
        totalFunctions = context.getFuncCount();
    }
}

