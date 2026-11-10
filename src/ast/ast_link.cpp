#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_const_visitor.h"

namespace das {

    class CountConstStrings : public ConstVisitor {
    public:
        uint32_t bytesTotal = 0;
        das_hash_set<string> uniStr;
        virtual void preVisit ( const ExprConstString * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( !expr->text.empty() && uniStr.insert(expr->text).second ) {
                bytesTotal += (uint32_t(expr->text.length()) + 1 + 3) & ~3u;
            }
        }
    };

    void linkProgram ( Program * program, TextWriter & logs ) {
        auto & library = program->library;
        auto thisModule = program->thisModule.get();
        library.foreach([&](Module * pm) -> bool {
            for ( auto & var : pm->globals.each() ) {
                if ( program->isUsed(var) ) {
                    program->globalInitStackSize = das::max(program->globalInitStackSize, var->initStackSize);
                }
            }
            return true;
        }, "*");
        // allocate used variables and functions indices - per pass, so -1 means this pass never saw it
        program->clearIndices();
        program->totalVariables = 0;
        program->totalFunctions = 0;
        program->globalStringHeapSize = 0;
        CountConstStrings strings;
        auto log = program->options.getBoolOption("log_stack");
        if ( log ) {
            logs << "INIT STACK SIZE:\t" << program->globalInitStackSize << "\n";
            logs << "FUNCTION TABLE:\n";
        }
        library.foreach([&](Module * pm) -> bool {
            for ( auto & func : pm->functions.each() ) {
                if ( program->isUsed(func) && !func->builtIn && !func->isTemplate ) {
                    auto index = program->totalFunctions++;
                    program->setIndex(func, index);
                    if ( func->body ) func->body->visit(strings);
                    if ( log ) {
                        logs << "\t" << index << "\t" << func->totalStackSize << "\t" << func->getMangledName();
                        if ( func->init ) {
                            logs << " [init";
                            if (func->lateInit ) logs << "(late)";
                            logs << "]";
                        }
                        if ( func->shutdown ) {
                            logs << " [finalize";
                            if (func->lateShutdown ) logs << "(late)";
                            logs << "]";
                        }
                        logs << " // " << HEX << func->getMangledNameHash() << DEC;
                        logs << "\n";
                    }
                }
                else {
                    program->setIndex(func, -2);
                }
            }
            return true;
        }, "*");
        if ( log ) {
            logs << "VARIABLE TABLE:\n";
        }
        library.foreach_in_order([&](Module * pm){
            for ( auto & var : pm->globals.each() ) {
                if (program->isUsed(var)) {
                    auto index = program->totalVariables++;
                    program->setIndex(var, index);
                    if ( var->init ) var->init->visit(strings);
                    if ( log ) {
                        logs << "\t" << index << "\t"  << var->stackTop << "\t"
                            << var->type->getSizeOf() << "\t" << var->getMangledName() << "\n";
                    }
                }
                else {
                    program->setIndex(var, -2);
                }
            }
            return true;
        }, thisModule);
        program->globalStringHeapSize = strings.bytesTotal;
    }
}
