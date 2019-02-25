#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    class MarkSymbolUse : public Visitor {
    public:
        MarkSymbolUse ( bool bid ) : builtInDependencies(bid) {
        }
        void propageteVarUse(const VariablePtr & var) {
            if (var->used) return;
            var->used = true;
            auto mn = var->getMangledName();
            for (const auto & gv : gVarVarUse[mn]) {
                propageteVarUse(gv);
            }
            for (const auto & fn : gVarFuncUse[mn]) {
                propagateFunctionUse(fn);
            }
        }
        void propagateFunctionUse(const FunctionPtr & fn) {
            if (fn->used) return;
            if (fn->builtIn) return;
            fn->used = true;
            for (const auto & gv : fn->useGlobalVariables) {
                propageteVarUse(gv->shared_from_this());
            }
            for (const auto & it : fn->useFunctions) {
                propagateFunctionUse(it->shared_from_this());
            }
        }
        void markUsedFunctions( ModuleLibrary & lib, bool forceAll ){
            lib.foreach([&](Module * pm) {
                for (const auto & it : pm->functions) {
                    auto fn = it.second;
                    if (forceAll || fn->exports) {
                        propagateFunctionUse(fn);
                    }
                }
                return true;
            }, "*");
        }
        void RemoveUnusedSymbols ( Module & mod ) {
            map<string,FunctionPtr> functions;
            map<string,VariablePtr> globals;
            swap(functions,mod.functions);
            swap(globals,mod.globals);
            mod.functionsByName.clear();
            for ( auto & fn : functions ) {
                if ( fn.second->used ) {
                    if ( !mod.addFunction(fn.second, true) ) {
                        program->error("internal error, failed to add function " + fn.first, fn.second->at );
                    }
                }
            }
            for ( auto & var : globals ) {
                if ( var.second->used ) {
                    if ( !mod.addVariable(var.second, true) ) {
                        program->error("internal error, failed to add variable " + var.first, var.second->at );
                    }
                }
            }
        }
    protected:
        ProgramPtr                                program;
        FunctionPtr                               func;
        map<string, vector<FunctionPtr>>          gVarFuncUse;
        map<string, vector<VariablePtr>>          gVarVarUse;
        string                                    gVar;
        bool                                      builtInDependencies;
    protected:
        // global variable declaration
        virtual void preVisitGlobalLet(const VariablePtr & var) override {
            Visitor::preVisitGlobalLet(var);
            gVar = var->getMangledName();
            var->used = false;
        }
        virtual VariablePtr visitGlobalLet(const VariablePtr & var) override {
            gVar.clear();
            return Visitor::visitGlobalLet(var);
        }
        // function
        virtual void preVisit(Function * f) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
            func->useFunctions.clear();
            func->useGlobalVariables.clear();
            func->used = false;
            assert(!func->builtIn && "visitor should never call 'visit' on builtin function at top level.");
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
                } else {
                    gVarVarUse[gVar].push_back(expr->variable);
                }
            }
        }
        // function address
        virtual void preVisit(ExprAddr * addr) override {
            Visitor::preVisit(addr);
            if (builtInDependencies || !addr->func->builtIn) {
                if (func) {
                    func->useFunctions.insert(addr->func);
                } else {
                    gVarFuncUse[gVar].push_back(addr->func->shared_from_this());
                }
            }
        }
        // function call
        virtual void preVisit(ExprCall * call) override {
            Visitor::preVisit(call);
            if (builtInDependencies || !call->func->builtIn) {
                if (func) {
                    func->useFunctions.insert(call->func);
                } else {
                    gVarFuncUse[gVar].push_back(call->func->shared_from_this());
                }
            }
        }
        // new
        virtual void preVisit(ExprNew * call) override {
            Visitor::preVisit(call);
            if ( call->initializer ) {
                if (builtInDependencies || !call->func->builtIn) {
                    if (func) {
                        func->useFunctions.insert(call->func);
                    } else {
                        gVarFuncUse[gVar].push_back(call->func->shared_from_this());
                    }
                }
            }
        }
        // Op1
        virtual void preVisit(ExprOp1 * expr) override {
            Visitor::preVisit(expr);
            if (builtInDependencies || !expr->func->builtIn) {
                if (func) {
                    func->useFunctions.insert(expr->func);
                } else {
                    gVarFuncUse[gVar].push_back(expr->func->shared_from_this());
                }
            }
        }
        // Op2
        virtual void preVisit(ExprOp2 * expr) override {
            Visitor::preVisit(expr);
            if (builtInDependencies || !expr->func->builtIn) {
                if (func) {
                    func->useFunctions.insert(expr->func);
                } else {
                    gVarFuncUse[gVar].push_back(expr->func->shared_from_this());
                }
            }
        }
        // Op3
        virtual void preVisit(ExprOp3 * expr) override {
            Visitor::preVisit(expr);
            if ( expr->func && (builtInDependencies || !expr->func->builtIn) ) {
                if (func) {
                    func->useFunctions.insert(expr->func);
                } else {
                    gVarFuncUse[gVar].push_back(expr->func->shared_from_this());
                }
            }
        }
    };

    void Program::clearSymbolUse() {
        for (auto & pm : library.modules) {
            for (auto & pv : pm->globals) {
                auto & var = pv.second;
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
    }

    void Program::markOrRemoveUnusedSymbols(bool forceAll) {
        clearSymbolUse();
        MarkSymbolUse vis(false);
        visit(vis);
        vis.markUsedFunctions(library, forceAll);
        if ( options.getOption("removeUnusedSymbols",true) ) {
            vis.RemoveUnusedSymbols(*thisModule);
        }
    }
}
