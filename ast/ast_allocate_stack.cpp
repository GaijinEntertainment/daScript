#include "precomp.h"

#include "ast.h"

namespace das {
    
    class AllocateStack : public Visitor {
    public:
        AllocateStack( const ProgramPtr & prog, ostream & ls ) : logs(ls) {
            program = prog;
            log = prog->options.getOption("logStack");
            if( log ) {
                logs << "\nSTACK INFORMATION:\n";
            }
        }
        int getFuncCount() const { return totalFunctions; }
    protected:
        ProgramPtr              program;
        FunctionPtr             func;
        uint32_t                stackTop = 0;
        vector<uint32_t>        stackTopStack;
        vector<ExprBlock *>     blocks;
        int                     totalFunctions = 0;
        bool                    log = false;
        ostream &               logs;
    protected:
    // function
        virtual void preVisit ( Function * f ) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
            func->totalStackSize = stackTop = sizeof(Prologue);
            func->index = totalFunctions ++;
            if ( log ) {
                logs << func->describe() << "\n";
            }
        }
        virtual FunctionPtr visit ( Function * that ) override {
            func->totalStackSize = max(func->totalStackSize, stackTop);
            if ( log ) {
                logs << func->totalStackSize << "\ttotal\n";
            }
            func.reset();
            return Visitor::visit(that);
        }
    // ExprReturn
        virtual void preVisit ( ExprReturn * expr ) override {
            if ( blocks.size() ) {
                auto block = blocks.back();
                expr->stackTop = block->stackTop;
                if ( log ) {
                    logs << "\t\t" << expr->stackTop << "\t\treturn, line " << expr->at.line << "\n";
                }
            } else {
                assert(!expr->returnInBlock);
            }
        }
    // ExprBlock
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            if ( block->isClosure ) {
                blocks.push_back(block);
            }
            if ( block->arguments.size() || block->copyOnReturn || block->moveOnReturn ) {
                block->stackTop = stackTop;
                stackTop += (sizeof(BlockArguments) + 0xf) & ~0xf;
                if ( log ) {
                    logs << "\t" << block->stackTop << "\t" << sizeof(BlockArguments)
                        << "\tblock arguments, line " << block->at.line << "\n";
                }
            }
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            if ( block->isClosure ) {
                blocks.pop_back();
            }
            return Visitor::visit(block);
        }
    // ExprCall
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->func->copyOnReturn || expr->func->moveOnReturn ) {
                auto sz = expr->func->result->getSizeOf();
                expr->stackTop = stackTop;
                stackTop += (sz + 0xf) & ~0xf;
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                        << "\tcall, line " << expr->at.line << "\n";
                }
            }
        }
    // ExprInvoke
        virtual void preVisit ( ExprInvoke * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->type->isRefType() ) {
                auto sz = expr->type->getSizeOf();
                expr->stackTop = stackTop;
                stackTop += (sz + 0xf) & ~0xf;
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                        << "\tinvoke, line " << expr->at.line << "\n";
                }
            }
        }
    // ExprFor
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
        }
        virtual void preVisitForStack ( ExprFor * expr ) override {
            Visitor::preVisitForStack(expr);
            for ( auto & var : expr->iteratorVariables ) {
                var->stackTop = stackTop;
                stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
                if ( log ) {
                    logs << "\t" << var->stackTop << "\t" << var->type->getSizeOf()
                        << "\tfor " << var->name << ", line " << var->at.line << "\n";
                }
            }
        }
    // ExprLet
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
        }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            var->stackTop = stackTop;
            stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
            if ( log ) {
                logs << "\t" << var->stackTop << "\t" << var->type->getSizeOf()
                    << "\tlet " << var->name << ", line " << var->at.line << "\n";
            }
            return Visitor::visitLet(expr,var,last);
        }
    };
    
    // program
    
    void Program::allocateStack(ostream & logs) {
        AllocateStack context(shared_from_this(), logs);
        visit(context);
        totalFunctions = context.getFuncCount();
    }
}

