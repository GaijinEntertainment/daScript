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
        vector<VariablePtr>     local;
        uint32_t                stackTop = 0;
        vector<uint32_t>        stackTopStack;
        vector<size_t>          varStack;
        int                     totalFunctions = 0;
    protected:
        void pushStack()    { stackTopStack.push_back(stackTop); }
        void popStack()     { stackTop = stackTopStack.back(); stackTopStack.pop_back(); }
        void pushVarStack() { varStack.push_back(local.size()); }
        void popVarStack()  { local.resize(varStack.back()); varStack.pop_back(); }
        void error ( const string & err, const LineInfo & at, CompilationError cerr = CompilationError::unspecified ) {
            program->error(err,at,cerr);
        }
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
            Visitor::preVisit(block);
            pushVarStack();
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            popVarStack();
            return Visitor::visit(block);
        }
    // ExprFor
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            pushVarStack();
            pushStack();
        }
        virtual void preVisitForStack ( ExprFor * expr ) override {
            Visitor::preVisitForStack(expr);
            for ( auto & var : expr->iteratorVariables ) {
                local.push_back(var);
                var->stackTop = stackTop;
                stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
            }
        }
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            func->totalStackSize = max(func->totalStackSize, stackTop);
            popStack();
            popVarStack();
            return Visitor::visit(expr);
        }
    // ExprLet
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
            pushVarStack();
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
                popVarStack();
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

