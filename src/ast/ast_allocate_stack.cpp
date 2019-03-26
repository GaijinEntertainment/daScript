#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    class VarCMRes : public Visitor {
    public:
        VarCMRes( const ProgramPtr & prog ) {
            program = prog;
        }
    protected:
        int                     inBlock = 0;
        ProgramPtr              program;
        FunctionPtr             func;
        VariablePtr             cmresVAR;
        bool                    failedToCMRES = false;
    protected:
    // function
        virtual void preVisit ( Function * f ) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
        }
        virtual FunctionPtr visit ( Function * that ) override {
            if ( cmresVAR && !failedToCMRES ) cmresVAR->aliasCMRES = true;
            func.reset();
            cmresVAR.reset();
            failedToCMRES = false;
            assert(inBlock==0);
            return Visitor::visit(that);
        }
    // ExprBlock
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            if ( block->isClosure ) inBlock ++;
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            if ( block->isClosure ) inBlock --;
            return Visitor::visit(block);
        }
    // ExprReturn
        virtual void preVisit ( ExprReturn * expr ) override {
            Visitor::preVisit(expr);
            if ( failedToCMRES ) return;
            if ( inBlock ) return;
            // we can safely skip makeLocal, invoke, or call
            // they are going to CMRES directly, and do not affect nothing
            if ( !func->result->isRefType() ) return;
            if ( expr->subexpr->rtti_isCall() ) return;
            if ( expr->subexpr->rtti_isInvoke() ) return;
            if ( expr->subexpr->rtti_isMakeLocal() ) return;
            // if its return X
            if ( expr->subexpr->rtti_isVar() ) {
                auto evar = static_pointer_cast<ExprVar>(expr->subexpr);
                if ( !evar->local ) return; // if its not local, we safely ignore
                auto var = evar->variable;
                if ( var->type->ref ) {     // we return local ref variable, so ... game over
                    failedToCMRES = true;
                } else if ( !cmresVAR ) {
                    cmresVAR = var;
                } else if ( cmresVAR!=var ) {
                    // TODO:    verify if we need to fail?
                    failedToCMRES = true;
                }
            }
        }
    };

    class AllocateStack : public Visitor {
    public:
        AllocateStack( const ProgramPtr & prog, TextWriter & ls ) : logs(ls) {
            program = prog;
            log = prog->options.getOption("logStack");
            optimize = prog->options.getOption("optimize");
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
        bool                    optimize = false;
        TextWriter &            logs;
        bool                    inStruct = false;
    protected:
        uint32_t allocateStack ( uint32_t size ) {
            auto result = stackTop;
            stackTop += (size + 0xf) & ~0xf;
            return result;
        }
    // structure
        virtual void preVisit ( Structure * var ) override {
            Visitor::preVisit(var);
            inStruct = true;
        }
        virtual StructurePtr visit ( Structure * var ) override {
            inStruct = false;
            return Visitor::visit(var);
        }
    // global variable init
        virtual void preVisitGlobalLet ( const VariablePtr & ) override {
            stackTop = 0;
        }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) override {
            program->globalInitStackSize = das::max(program->globalInitStackSize, stackTop);
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
            func->totalStackSize = das::max(func->totalStackSize, stackTop);
            // detecting fastcall
            if (!func->exports && !func->addr && func->totalStackSize == sizeof(Prologue)) {
                if (func->body->rtti_isBlock()) {
                    auto block = static_pointer_cast<ExprBlock>(func->body);
                    if ( func->result->isWorkhorseType() ) {
                        if (block->list.size()==1 && block->finalList.size()==0 && block->list.back()->rtti_isReturn()) {
                            func->fastCall = true;
                        }
                    } else if ( func->result->isVoid() ) {
                        if (block->list.size()==1 && block->finalList.size()==0 ) {
                            func->fastCall = true;
                        }
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
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( blocks.size() ) {
                auto block = blocks.back();
                expr->stackTop = block->stackTop;
                if ( log ) {
                    logs << "\t\t" << expr->stackTop << "\t\treturn, line " << expr->at.line << "\n";
                }
            } else {
                DAS_ASSERT(!expr->returnInBlock);
            }
            if ( expr->subexpr ) {
                if ( expr->subexpr->rtti_isMakeLocal() ) {
                    uint32_t sz = sizeof(void *);
                    expr->refStackTop = allocateStack(sz);
                    expr->takeOverRightStack = true;
                    if ( log ) {
                        logs << "\t" << expr->refStackTop << "\t" << sz
                        << "\treturn ref and eval, line " << expr->at.line << "\n";
                    }
                    auto mkl = static_pointer_cast<ExprMakeLocal>(expr->subexpr);
                    if ( expr->returnInBlock ) {
                        mkl->setRefSp(true, false, expr->refStackTop, 0);
                    } else {
                        mkl->setRefSp(true, true, expr->refStackTop, 0);
                        expr->returnCMRES = true;
                    }
                    mkl->doesNotNeedInit = false;
                } else if ( expr->subexpr->rtti_isCall() ) {
                    auto cll = static_pointer_cast<ExprCall>(expr->subexpr);
                    if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                        cll->doesNotNeedSp = true;
                        expr->returnCallCMRES = true;
                    }
                } else if ( expr->subexpr->rtti_isInvoke() ) {
                    auto cll = static_pointer_cast<ExprInvoke>(expr->subexpr);
                    if ( cll->isCopyOrMove() ) {
                        cll->doesNotNeedSp = true;
                        expr->returnCallCMRES = true;
                    }
                } else if ( expr->subexpr->rtti_isVar() ) {
                    auto evar = static_pointer_cast<ExprVar>(expr->subexpr);
                    if ( evar->variable->aliasCMRES ) {
                        expr->returnCMRES = true;
                    }
                }
            }
        }
    // ExprBlock
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            if ( inStruct ) return;
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
            if ( inStruct ) {
                return Visitor::visit(block);
            }
            if ( block->isClosure ) {
                blocks.pop_back();
            }
            return Visitor::visit(block);
        }
    // ExprOp1
        virtual void preVisit ( ExprOp1 * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( expr->func->copyOnReturn || expr->func->moveOnReturn ) {
                auto sz = expr->func->result->getSizeOf();
                expr->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                    << "\top1, line " << expr->at.line << "\n";
                }
            }
        }
    // ExprOp2
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( expr->func->copyOnReturn || expr->func->moveOnReturn ) {
                auto sz = expr->func->result->getSizeOf();
                expr->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                        << "\top2, line " << expr->at.line << "\n";
                }
            }
        }
    // ExprCall
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( !expr->doesNotNeedSp ) {
                if ( expr->func->copyOnReturn || expr->func->moveOnReturn ) {
                    auto sz = expr->func->result->getSizeOf();
                    expr->stackTop = allocateStack(sz);
                    if ( log ) {
                        logs << "\t" << expr->stackTop << "\t" << sz
                            << "\tcall, line " << expr->at.line << "\n";
                    }
                }
            }
        }
    // ExprInvoke
        virtual void preVisit ( ExprInvoke * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( !expr->doesNotNeedSp ) {
                if ( expr->type->isRefType() ) {
                    auto sz = expr->type->getSizeOf();
                    expr->stackTop = allocateStack(sz);
                    if ( log ) {
                        logs << "\t" << expr->stackTop << "\t" << sz
                            << "\tinvoke, line " << expr->at.line << "\n";
                    }
                }
            }
        }
    // ExprFor
        virtual void preVisitForStack ( ExprFor * expr ) override {
            Visitor::preVisitForStack(expr);
            if ( inStruct ) return;
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
        virtual void preVisitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(expr,var,last);
            if ( inStruct ) return;
            uint32_t sz = var->type->ref ? sizeof(void *) : var->type->getSizeOf();
            if ( var->aliasCMRES ) {
                if ( log ) {
                    logs << "\tCR\t" << sz
                        << "\tlet " << var->name << ", line " << var->at.line << "\n";
                }
            } else {
                var->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << var->stackTop << "\t" << sz
                        << "\tlet " << var->name << ", line " << var->at.line << "\n";
                }
            }
            if ( var->init ) {
                if ( var->init->rtti_isMakeLocal() ) {
                    auto mkl = static_pointer_cast<ExprMakeLocal>(var->init);
                    mkl->setRefSp(false, var->aliasCMRES, var->stackTop, 0);
                    mkl->doesNotNeedInit = false;
                } else if ( var->init->rtti_isCall() ) {
                    auto cll = static_pointer_cast<ExprCall>(var->init);
                    if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                        cll->doesNotNeedSp = true;
                    }
                } else if ( var->init->rtti_isInvoke() ) {
                    auto cll = static_pointer_cast<ExprInvoke>(var->init);
                    if ( cll->isCopyOrMove() ) {
                        cll->doesNotNeedSp = true;
                    }
                }
            }
        }
    // ExprMakeBlock
        virtual void preVisit ( ExprMakeBlock * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            auto sz = uint32_t(sizeof(Block));
            expr->stackTop = allocateStack(sz);
            if ( log ) {
                logs << "\t" << expr->stackTop << "\t" << sz
                << "\tmake block " << expr->type->describe() << ", line " << expr->at.line << "\n";
            }
            if ( func ) {
                func->hasMakeBlock = true;
            }
        }
    // ExprAscend
        virtual void preVisit ( ExprAscend * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( expr->subexpr->rtti_isMakeLocal() ) {
                uint32_t sz = sizeof(void *);
                expr->stackTop = allocateStack(sz);
                expr->useStackRef = true;
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                    << "\tascend, line " << expr->at.line << "\n";
                }
                auto mkl = static_pointer_cast<ExprMakeLocal>(expr->subexpr);
                mkl->setRefSp(true, false, expr->stackTop, 0);
            }
        }
    // ExprMakeStructure
        virtual void preVisit ( ExprMakeStructure * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( !expr->doesNotNeedSp ) {
                auto sz = expr->type->getSizeOf();
                uint32_t cStackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << cStackTop << "\t" << sz
                        << "\t[[" << expr->type->describe() << "]], line " << expr->at.line << "\n";
                }
                expr->setRefSp(false, false, cStackTop, 0);
                expr->doesNotNeedInit = false;
            }
        }
    // ExprMakeArray
        virtual void preVisit ( ExprMakeArray * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( !expr->doesNotNeedSp ) {
                auto sz = expr->type->getSizeOf();
                expr->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                    << "\t[[" << expr->type->describe() << "]], line " << expr->at.line << "\n";
                }
                expr->setRefSp(false, false, stackTop, 0);
                expr->doesNotNeedInit = false;
            }
        }
    // New
        virtual void preVisit ( ExprNew * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( expr->type->dim.size() ) {
                auto sz = uint32_t(expr->type->getCountOf()*sizeof(char *));
                expr->stackTop = allocateStack(sz);
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                    << "\tNEW " << expr->typeexpr->describe() << ", line " << expr->at.line << "\n";
                }
            }
        }
    // ExprCopy
        virtual void preVisit ( ExprCopy * expr ) override {
            Visitor::preVisit(expr);
            if ( inStruct ) return;
            if ( expr->right->rtti_isMakeLocal() ) {
                uint32_t sz = sizeof(void *);
                expr->stackTop = allocateStack(sz);
                expr->takeOverRightStack = true;
                if ( log ) {
                    logs << "\t" << expr->stackTop << "\t" << sz
                        << "\tcopy [[ ]], line " << expr->at.line << "\n";
                }
                auto mkl = static_pointer_cast<ExprMakeLocal>(expr->right);
                mkl->setRefSp(true, false, expr->stackTop, 0);
                mkl->doesNotNeedInit = false;
            } else if ( expr->right->rtti_isCall() ) {
                auto cll = static_pointer_cast<ExprCall>(expr->right);
                if ( cll->func->copyOnReturn || cll->func->moveOnReturn ) {
                    cll->doesNotNeedSp = true;
                }
            } else if ( expr->right->rtti_isInvoke() ) {
                auto cll = static_pointer_cast<ExprInvoke>(expr->right);
                if ( cll->isCopyOrMove() ) {
                    cll->doesNotNeedSp = true;
                }
            }
        }
    };

    // program

    void Program::allocateStack(TextWriter & logs) {
        // move some variables to CMRES
        VarCMRes vcm(shared_from_this());
        visit(vcm);
        // allocate stack for the rest of them
        AllocateStack context(shared_from_this(), logs);
        visit(context);
        // allocate used variables and functions indices
        totalVariables = 0;
        totalFunctions = 0;
        auto log = options.getOption("logStack");
        if ( log ) {
            logs << "FUNCTION TABLE:\n";
        }
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
                    if ( log ) {
                        logs << func->index << "\t" << func->getMangledName() << "\n";
                    }
                }
                else {
                    func->index = -2;
                }
            }
        }
    }
}

