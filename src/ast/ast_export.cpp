#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    class ClearUnusedSymbols : public Visitor {
    public:
        virtual void preVisit(ExprAddr * expr) override {
            Visitor::preVisit(expr);
            if ( expr->func && !expr->func->used && !expr->func->builtIn ) {
                expr->func = nullptr;
            }
        }
        virtual void preVisitExpression(Expression * expr) override {
            Visitor::preVisitExpression(expr);
            if ( expr->rtti_isCallFunc() ) {
                auto cfe = static_cast<ExprCallFunc *>(expr);
                if ( cfe->func && !cfe->func->used && !cfe->func->builtIn ) {
                    cfe->func = nullptr;
                }
            }
        }
    };

    class MarkSymbolUse : public Visitor {
    public:
        MarkSymbolUse ( bool bid ) : builtInDependencies(bid) {
        }
        void propageteVarUse(const VariablePtr & var) {
            assert(var);
            if (var->used) return;
            var->used = true;
            for (const auto & gv : var->useGlobalVariables) {
                propageteVarUse(gv);
            }
            for (const auto & it : var->useFunctions) {
                propagateFunctionUse(it);
            }
        }
        void propagateFunctionUse(const FunctionPtr & fn) {
            assert(fn);
            if (fn->used) return;
            if (fn->builtIn) return;
            fn->used = true;
            for (const auto & gv : fn->useGlobalVariables) {
                propageteVarUse(gv);
            }
            for (const auto & it : fn->useFunctions) {
                propagateFunctionUse(it);
            }
        }
        void markVarsUsed( ModuleLibrary & lib, bool forceAll ){
            lib.foreach([&](Module * pm) {
                for (const auto & it : pm->globals) {
                    auto & var = it.second;
                    if ( forceAll || var->used ) {
                        var->used = false;
                        propageteVarUse(var);
                    }
                }
                return true;
            }, "*");
        }
        void markUsedFunctions( ModuleLibrary & lib, bool forceAll ){
            lib.foreach([&](Module * pm) {
                for (const auto & it : pm->functions) {
                    auto fn = it.second;
                    if (forceAll || fn->exports || fn->init) {
                        propagateFunctionUse(fn);
                    }
                }
                return true;
            }, "*");
        }
        void RemoveUnusedSymbols ( Module & mod ) {
            das_safe_map<string,FunctionPtr> functions;
            vector<VariablePtr> globalsInOrder;
            swap(functions,mod.functions);
            swap(globalsInOrder, mod.globalsInOrder);
            mod.functionsByName.clear();
            mod.globals.clear();
            for ( auto & fn : functions ) {
                if ( fn.second->used ) {
                    if ( !mod.addFunction(fn.second, true) ) {
                        program->error("internal error, failed to add function " + fn.first,"","", fn.second->at );
                    }
                }
            }
            for ( auto & var : globalsInOrder ) {
                if ( var->used ) {
                    if ( !mod.addVariable(var, true) ) {
                        program->error("internal error, failed to add variable " + var->name,"","", var->at );
                    }
                }
            }
        }
    protected:
        ProgramPtr  program;
        FunctionPtr func;
        Variable *  gVar = nullptr;
        bool        builtInDependencies;
    protected:
        // global variable declaration
        virtual void preVisitGlobalLet(const VariablePtr & var) override {
            Visitor::preVisitGlobalLet(var);
            gVar = var.get();
            var->useFunctions.clear();
            var->useGlobalVariables.clear();
            var->used = false;
        }
        virtual VariablePtr visitGlobalLet(const VariablePtr & var) override {
            gVar = nullptr;
            return Visitor::visitGlobalLet(var);
        }
        // function
        virtual void preVisit(Function * f) override {
            Visitor::preVisit(f);
            func = f;
            func->useFunctions.clear();
            func->useGlobalVariables.clear();
            func->used = false;
            DAS_ASSERTF(!func->builtIn, "visitor should never call 'visit' on builtin function at top level.");
        }
        virtual FunctionPtr visit(Function * that) override {
            func.reset();
            return Visitor::visit(that);
        }
        // variable
        virtual void preVisit(ExprVar * expr) override {
            Visitor::preVisit(expr);
            if (!expr->local && !expr->argument && !expr->block) {
                if (func) {
                    func->useGlobalVariables.insert(expr->variable.get());
                } else if (gVar) {
                    gVar->useGlobalVariables.insert(expr->variable.get());
                }
            }
        }
        // function address
        virtual void preVisit(ExprAddr * addr) override {
            Visitor::preVisit(addr);
            if (builtInDependencies || !addr->func->builtIn) {
                assert(addr->func);
                if (func) {
                    func->useFunctions.insert(addr->func);
                } else if (gVar) {
                    gVar->useFunctions.insert(addr->func);
                }
            }
        }
        // function call
        virtual void preVisit(ExprCall * call) override {
            Visitor::preVisit(call);
            if (builtInDependencies || !call->func->builtIn) {
                assert(call->func);
                if (func) {
                    func->useFunctions.insert(call->func);
                } else if (gVar) {
                    gVar->useFunctions.insert(call->func);
                }
            }
        }
        // new
        virtual void preVisit(ExprNew * call) override {
            Visitor::preVisit(call);
            if ( call->initializer ) {
                if (builtInDependencies || !call->func->builtIn) {
                    assert(call->func);
                    if (func) {
                        func->useFunctions.insert(call->func);
                    } else if (gVar) {
                        gVar->useFunctions.insert(call->func);
                    }
                }
            }
        }
        // Op1
        virtual void preVisit(ExprOp1 * expr) override {
            Visitor::preVisit(expr);
            if (builtInDependencies || !expr->func->builtIn) {
                assert(expr->func);
                if (func) {
                    func->useFunctions.insert(expr->func);
                } else if (gVar) {
                    gVar->useFunctions.insert(expr->func);
                }
            }
        }
        // Op2
        virtual void preVisit(ExprOp2 * expr) override {
            Visitor::preVisit(expr);
            if (builtInDependencies || !expr->func->builtIn) {
                assert(expr->func);
                if (func) {
                    func->useFunctions.insert(expr->func);
                } else if (gVar) {
                    gVar->useFunctions.insert(expr->func);
                }
            }
        }
        // Op3
        virtual void preVisit(ExprOp3 * expr) override {
            Visitor::preVisit(expr);
            if ( expr->func && (builtInDependencies || !expr->func->builtIn) ) {
                assert(expr->func);
                if (func) {
                    func->useFunctions.insert(expr->func);
                } else if (gVar) {
                    gVar->useFunctions.insert(expr->func);
                }
            }
        }
    };

    void Program::clearSymbolUse() {
        for (auto & pm : library.modules) {
            for (auto & var : pm->globalsInOrder) {
                var->used = false;
            }
            for (auto & pf : pm->functions) {
                auto & func = pf.second;
                func->used = false;
            }
        }
    }

    void Program::markSymbolUse(bool builtInSym) {
        clearSymbolUse();
        MarkSymbolUse vis(builtInSym);
        visit(vis);
        vis.markUsedFunctions(library, false);
        vis.markVarsUsed(library, false);
    }

    void Program::markOrRemoveUnusedSymbols(bool forceAll) {
        forceAll |=  !options.getBoolOption("remove_unused_symbols",true);
        clearSymbolUse();
        MarkSymbolUse vis(false);
        visit(vis);
        vis.markUsedFunctions(library, forceAll);
        vis.markVarsUsed(library, forceAll);
        if ( options.getBoolOption("remove_unused_symbols",true) ) {
            ClearUnusedSymbols cvis;
            visit(cvis);
            vis.RemoveUnusedSymbols(*thisModule);
        }
    }
}
