#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    class AllocateStack : public Visitor {
    public:
        AllocateStack( const ProgramPtr & prog, TextWriter & ls ) : logs(ls) {
            program = prog;
            log = prog->options.getOption("logStack");
            if( log ) {
                logs << "\nSTACK INFORMATION:\n";
            }
        }
    protected:
        ProgramPtr              program;
        FunctionPtr             func;
        uint32_t                stackTop = 0;
        vector<uint32_t>        stackTopStack;
        vector<ExprBlock *>     blocks;
        bool                    log = false;
        TextWriter &               logs;
    protected:
        uint32_t allocateStack ( uint32_t size ) {
            auto result = stackTop;
            stackTop += (size + 0xf) & ~0xf;
            return result;
        }
    // global variable init
        virtual void preVisitGlobalLet ( const VariablePtr & ) override {
            stackTop = 0;
        }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) override {
            program->globalInitStackSize = max(program->globalInitStackSize, stackTop);
            return Visitor::visitGlobalLet(var);
        }
    // function
        virtual void preVisit ( Function * f ) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
            func->totalStackSize = stackTop = sizeof(Prologue);
            if ( log ) {
                if (!func->used) logs << "unused ";
                logs << func->describe() << "\n";
            }
        }
        virtual FunctionPtr visit ( Function * that ) override {
            func->totalStackSize = max(func->totalStackSize, stackTop);
            // detecting fastcall
            if (!func->exports && func->result->isWorkhorseType() && func->totalStackSize == sizeof(Prologue)) {
                if (func->body->rtti_isBlock()) {
                    auto block = static_pointer_cast<ExprBlock>(func->body);
                    if (block->list.size() == 1 && block->list.back()->rtti_isReturn()) {
                        func->fastCall = true;
                    }
                }
            }
            if ( log ) {
                logs << func->totalStackSize << "\ttotal" << (func->fastCall ? ", fastcall" : "") << "\n";
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
                auto sz = uint32_t(sizeof(BlockArguments));
                block->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << block->stackTop << "\t" << sz
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
                expr->stackTop = allocateStack(sz);
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
                expr->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                        << "\tinvoke, line " << expr->at.line << "\n";
                }
            }
        }
    // ExprFor
        virtual void preVisitForStack ( ExprFor * expr ) override {
            Visitor::preVisitForStack(expr);
            for ( auto & var : expr->iteratorVariables ) {
                auto sz = var->type->getSizeOf();
                var->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << var->stackTop << "\t" << sz
                        << "\tfor " << var->name << ", line " << var->at.line << "\n";
                }
            }
        }
    // ExprLet
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            auto sz = var->type->getSizeOf();
            var->stackTop = allocateStack(sz);
            if ( log ) {
                logs << "\t" << var->stackTop << "\t" << sz
                    << "\tlet " << var->name << ", line " << var->at.line << "\n";
            }
            return Visitor::visitLet(expr,var,last);
        }
    // ExprMakeStructure
        virtual ExpressionPtr visit ( ExprMakeStructure * expr ) override {
            auto sz = expr->type->getSizeOf();
            expr->stackTop = allocateStack(sz);
            if ( log ) {
                logs << "\t" << expr->stackTop << "\t" << sz
                    << "\t[[" << expr->type->describe() << "]], line " << expr->at.line << "\n";
            }
            return Visitor::visit(expr);
        }
    // ExprMakeArray
        virtual ExpressionPtr visit ( ExprMakeArray * expr ) override {
            auto sz = expr->type->getSizeOf();
            expr->stackTop = allocateStack(sz);
            if ( log ) {
                logs << "\t" << expr->stackTop << "\t" << sz
                << "\t[[" << expr->type->describe() << "]], line " << expr->at.line << "\n";
            }
            return Visitor::visit(expr);
        }
    };

    // program

    void Program::allocateStack(TextWriter & logs) {
        AllocateStack context(shared_from_this(), logs);
        visit(context);
        // allocate used variables and functions indices
        totalVariables = 0;
        totalFunctions = 0;
        for (auto & pm : library.modules) {
            for (auto & pv : pm->globals) {
                auto & var = pv.second;
                if (var->used) {
                    var->index = totalVariables++;
                }
                else {
                    var->index = -2;
                }
            }
            for (auto & pf : pm->functions) {
                auto & func = pf.second;
                if (func->used) {
                    func->index = totalFunctions++;
                }
                else {
                    func->index = -2;
                }
            }
        }
    }
}

