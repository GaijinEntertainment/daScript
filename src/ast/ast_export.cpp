#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/simulate/hash.h"
#include "daScript/simulate/aot_builtin_ast.h"

#if 1
#define debug_aot_hash(...)
#else
#define debug_aot_hash  printf
#endif

namespace das {

    class ClearUnusedSymbols : public Visitor {
    public:
        ClearUnusedSymbols ( Program * prog, Module * tm ) : program(prog), thisModule(tm) {}
        virtual bool canVisitFunction ( Function * fun ) override {
            return !fun->stub && !fun->isTemplate;    // we don't do a thing with templates
        }
        virtual bool canVisitStructureFieldInit ( Structure * ) override { return true; }
        // default-argument initializers are the same dangling-ref story as field inits:
        // MarkSymbolUse never visits them (canVisitArgumentInit is false there), so a
        // function/global whose ONLY anchor is `def f(x = helper())` is never marked used,
        // removeUnusedSymbols frees it, and the surviving f keeps the raw pointer in
        // arg->init - serialization then reads it (SIGSEGV in getMangledName under --ser).
        // infer already inlined every default into its call sites, so nulling the husk here
        // costs nothing
        virtual bool canVisitArgumentInit ( Function *, const VariablePtr &, Expression * ) override { return true; }
        virtual void preVisit(ExprAddr * expr) override {
            Visitor::preVisit(expr);
            if ( expr->func && !program->isUsed(expr->func) && !expr->func->builtIn ) {
                expr->func = nullptr;
            }
        }
        // `WithInit()` in a struct field initializer infers to ExprMakeStruct, whose
        // constructor ref is NOT an ExprCallFunc - left unnulled, removeUnusedSymbols frees
        // the ctor while the surviving structure's field-init expression keeps the raw
        // pointer, and AST serialization reads a dangling Function* (nondeterministic
        // SIGSEGV in getMangledName under --ser)
        virtual void preVisit(ExprMakeStruct * expr) override {
            Visitor::preVisit(expr);
            if ( expr->constructor && !program->isUsed(expr->constructor) && !expr->constructor->builtIn ) {
                expr->constructor = nullptr;
            }
        }
        // same dangling-pointer story for a global referenced only from a field initializer
        // (`s : int = g_seed`). only this module's globals get freed (RemoveUnusedSymbols
        // runs on thisModule alone), so cross-module refs - e.g. folded builtin constants
        // like math::PI - stay untouched and keep their valid metadata
        virtual void preVisit(ExprVar * expr) override {
            Visitor::preVisit(expr);
            if ( expr->variable && expr->variable->module == thisModule && !program->isUsed(expr->variable) ) {
                expr->variable = nullptr;
            }
        }
        virtual void preVisitExpression(Expression * expr) override {
            Visitor::preVisitExpression(expr);
            if ( expr->rtti_isCallFunc() ) {
                auto cfe = static_cast<ExprCallFunc *>(expr);
                if ( cfe->func && !program->isUsed(cfe->func) && !cfe->func->builtIn ) {
                    cfe->func = nullptr;
                }
            }
        }
    protected:
        Program * program = nullptr;
        Module * thisModule = nullptr;
    };

    class MarkSymbolUse : public Visitor {
    public:
        MarkSymbolUse ( Program * prog, bool bid ) : program(prog), builtInDependencies(bid) {
        }
        __forceinline void push ( Variable* pvar ) {
            if ( !tw ) return;
            *tw << string(logTab,'\t') << pvar->getMangledName() << "\n";
            logTab ++;
        }
        __forceinline void push ( Function* pfun ) {
            if ( !tw ) return;
            *tw << string(logTab,'\t') << pfun->getMangledName() << "\n";
            logTab ++;
        }
        __forceinline void pop () {
            if ( !tw ) return;
            logTab --;
        }
        void propageteVarUse(Variable * var) {
            DAS_ASSERT(var);
            if (program->isUsed(var)) return;
            push(var);
            program->setUsed(var, true);
            for (const auto & gv : var->useGlobalVariables) {
                propageteVarUse(gv);
            }
            for (const auto & it : var->useFunctions) {
                propagateFunctionUse(it);
            }
            pop();
        }
        void propagateFunctionUse(Function * fn) {
            DAS_ASSERT(fn);
            if (fn->isTemplate) return;
            if (program->isUsed(fn)) return;
            if (fn->builtIn) return;
            push(fn);
            program->setUsed(fn, true);
            for (const auto & gv : fn->useGlobalVariables) {
                propageteVarUse(gv);
            }
            for (const auto & it : fn->useFunctions) {
                propagateFunctionUse(it);
            }
            pop();
        }
        bool isVarExported ( const VariablePtr & var ) const {
            return var->annotation.getBoolOption("export",false);
        }
        void markVarsUsed( ModuleLibrary & lib, bool forceAll ){
            lib.foreach([&](Module * pm) {
                for ( auto & var : pm->globals.each() ) {
                    if ( forceAll || program->isUsed(var) || isVarExported(var) ) {
                        program->setUsed(var, false);
                        propageteVarUse(var);
                    }
                }
                return true;
            }, "*");
        }
        void markUsedFunctions( ModuleLibrary & lib, bool forceAll, bool initThis, Module * macroModule ) {
            lib.foreach([&](Module * pm) {
                if ( initThis && macroModule && pm!=macroModule ) return true;
                for ( auto & fn : pm->functions.each() ) {
                    if ( (forceAll && !fn->macroInit) || fn->exports || fn->init || fn->shutdown || (fn->macroInit && initThis) ) {
                        propagateFunctionUse(fn);
                    }
                }
                return true;
            }, "*");
        }
        void exportPublicFunctions( Module * thisModule ) {
            for ( auto & fn : thisModule->functions.each() ) {
                if ( fn->privateFunction || fn->builtIn || fn->generated || fn->isTemplate ) continue;
                if ( fn->macroInit || fn->macroFunction || fn->init || fn->shutdown ) continue;
                if ( fn->isClassMethod || fn->lambda || fn->generator || fn->fromGeneric ) continue;
                fn->exports = true;
            }
        }
        void markModuleVarsUsed( ModuleLibrary &, Module * inWhichModule ) {
            for ( auto & var : inWhichModule->globals.each() ) {
                program->setUsed(var, false);
                propageteVarUse(var);
            }
        }
        void markModuleUsedFunctions( ModuleLibrary &, Module * inWhichModule ) {
            for ( auto & fn : inWhichModule->functions.each() ) {
                if ( fn->isTemplate ) continue;
                if ( fn->builtIn || fn->macroInit || fn->macroFunction  ) continue;
                if ( fn->privateFunction && fn->generated && fn->fromGeneric ) continue;    // instances of templates are never roots
                if ( fn->isClassMethod && fn->classParent->macroInterface ) continue;       // methods of macro interfaces
                propagateFunctionUse(fn);
            }
        }
        void RemoveUnusedSymbols ( Module & mod ) {
            auto functions = das::move(mod.functions);
            auto globals = das::move(mod.globals);
            mod.functionsByName.clear();
            // mod.globals.clear();
            for ( auto & fn : functions.each() ) {
                if ( fn->builtIn || program->isUsed(fn) ) {
                    if ( !mod.addFunction(fn, true) ) {
                        program->error("internal error, failed to add function " + fn->name,"","", fn->at, CompilationError::internal_function );
                    }
                }
            }
            for ( auto & var : globals.each() ) {
                if ( program->isUsed(var) ) {
                    if ( !mod.addVariable(var, true) ) {
                        program->error("internal error, failed to add variable " + var->name,"","", var->at, CompilationError::internal_variable );
                    }
                }
            }
        }
    protected:
        ProgramPtr  program;
        FunctionPtr func = nullptr;
        Variable *  gVar = nullptr;
        bool        builtInDependencies;
        int         logTab = 0;
    public:
        TextWriter * tw = nullptr;
    protected:
        virtual bool canVisitFunction ( Function * fun ) override {
            return !fun->stub && !fun->isTemplate;    // we don't do a thing with templates
        }
        virtual bool canVisitStructure ( Structure * st ) override {
            return !st->isTemplate;    // we don't do a thing with templates
        }
        virtual bool canVisitStructureFieldInit ( Structure * ) override { return false; }
        virtual bool canVisitArgumentInit ( Function *, const VariablePtr &, Expression * ) override { return false; }
        // global variable declaration
        virtual void preVisitGlobalLet(const VariablePtr & var) override {
            Visitor::preVisitGlobalLet(var);
            gVar = var;
            var->useFunctions.clear();
            var->useGlobalVariables.clear();
            program->setUsed(var, false);
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
            program->setUsed(f, false);
            func->callCaptureString = false;
            func->hasStringBuilder = false;
            DAS_ASSERTF(!func->builtIn, "visitor should never call 'visit' on builtin function at top level.");
        }
        virtual FunctionPtr visit(Function * that) override {
            func = nullptr;
            return Visitor::visit(that);
        }
        // string builder
        virtual void preVisit ( ExprStringBuilder * expr ) override {
            Visitor::preVisit(expr);
            if (func) func->hasStringBuilder = true;
        }
        // variable
        virtual void preVisit(ExprVar * expr) override {
            Visitor::preVisit(expr);
            if (!expr->variable) return;
            if (!expr->local && !expr->argument && !expr->block) {
                if (func) {
                    func->useGlobalVariables.insert(expr->variable);
                } else if (gVar) {
                    gVar->useGlobalVariables.insert(expr->variable);
                }
            }
        }
        void useFunction ( Function * fn ) {
            if (func) {
                func->useFunctions.insert(fn);
            } else if (gVar) {
                gVar->useFunctions.insert(fn);
            }
        }
        // function address
        virtual void preVisit(ExprAddr * addr) override {
            Visitor::preVisit(addr);
            if ( !addr->func ) return;
            if (builtInDependencies || !addr->func->builtIn) {
                useFunction(addr->func);
            }
        }
        void markInitializers ( TypeDecl * type, das_set<Structure *> & seen ) {
            if ( !type ) return;
            if ( type->baseType==Type::tStructure && type->structType && seen.insert(type->structType).second ) {
                if ( auto init = type->structType->findInitializer() ) {
                    useFunction(init);
                }
                for ( auto & fd : type->structType->fields ) {
                    markInitializers(fd.type, seen);
                }
            }
            markInitializers(type->firstType, seen);
            markInitializers(type->secondType, seen);
            for ( auto & argT : type->argTypes ) {
                markInitializers(argT, seen);
            }
        }
        // function call
        virtual void preVisit(ExprCall * call) override {
            Visitor::preVisit(call);
            if ( !call->func ) return;
            if ( func && call->func->captureString ) {
                func->callCaptureString = true;
            }
            if (builtInDependencies || !call->func->builtIn) {
                useFunction(call->func);
            }
            if ( call->func->builtIn && call->func->name=="sscan_json" && call->arguments.size()==2 ) {
                das_set<Structure *> seen;
                markInitializers(call->arguments[1]->type, seen);
            }
        }
        // new
        virtual void preVisit(ExprNew * call) override {
            Visitor::preVisit(call);
            if ( call->func && call->initializer ) {
                if (builtInDependencies || !call->func->builtIn) {
                    useFunction(call->func);
                }
            }
        }
        virtual void preVisit(ExprMakeStruct * call) override {
            Visitor::preVisit(call);
            if ( call->constructor ) {
                useFunction(call->constructor);
            }
        }
        // Op1
        virtual void preVisit(ExprOp1 * expr) override {
            Visitor::preVisit(expr);
            if ( !expr->func ) return;
            if (builtInDependencies || !expr->func->builtIn) {
                useFunction(expr->func);
            }
        }
        // Op2
        virtual void preVisit(ExprOp2 * expr) override {
            Visitor::preVisit(expr);
            if ( !expr->func ) return;
            if (builtInDependencies || !expr->func->builtIn) {
                useFunction(expr->func);
            }
        }
        // Op3
        virtual void preVisit(ExprOp3 * expr) override {
            Visitor::preVisit(expr);
            if ( !expr->func ) return;
            if (builtInDependencies || !expr->func->builtIn) {
                useFunction(expr->func);
            }
        }
    };

    void Program::clearSymbolUse() {
        usedFunctions.clear();
        usedVariables.clear();
    }

    void Program::markModuleSymbolUse(TextWriter * logs) {
        // this module public, this module export, this module init\shutdown
        clearSymbolUse();
        MarkSymbolUse vis(this, false);
        vis.tw = logs;
        visit(vis);
        vis.markModuleUsedFunctions(library, thisModule.get());
        vis.markModuleVarsUsed(library, thisModule.get());
    }

    void Program::markMacroSymbolUse(TextWriter * logs) {
        // this module macro init
        clearSymbolUse();
        MarkSymbolUse vis(this, false);
        vis.tw = logs;
        visit(vis);
        vis.markUsedFunctions(library, false, true, thisModule.get());
        vis.markVarsUsed(library, false);
    }

    void Program::markExecutableSymbolUse(TextWriter * logs) {
        clearSymbolUse();
        MarkSymbolUse vis(this, false);
        vis.tw = logs;
        visit(vis);
        if ( policies.export_public_functions ) vis.exportPublicFunctions(thisModule.get());
        vis.markUsedFunctions(library, false, false, nullptr);
        vis.markVarsUsed(library, false);
    }

    void Program::markFoldingSymbolUse(const vector<Function *> & needRun, TextWriter * logs) {
        clearSymbolUse();
        MarkSymbolUse vis(this, false);
        vis.tw = logs;
        visit(vis);
        for ( auto fun : needRun ) {
            vis.propagateFunctionUse(fun);
        }
    }

    void Program::markFunctionDependencies(Function * fun) {
        // refresh use-edges over the closure reachable from fun only: a macro asking for one
        // function's dependencies must not pay for a walk of every module in the program
        MarkSymbolUse vis(this, false);
        Visitor & bvis = vis;
        das_hash_set<Function *> seenFn;
        das_hash_set<Variable *> seenVar;
        vector<Function *> workFn;
        vector<Variable *> workVar;
        auto pushFn = [&](Function * f) {
            if ( f->builtIn || !bvis.canVisitFunction(f) ) return;
            if ( seenFn.insert(f).second ) workFn.push_back(f);
        };
        auto pushVar = [&](Variable * v) {
            if ( seenVar.insert(v).second ) workVar.push_back(v);
        };
        pushFn(fun);
        while ( !workFn.empty() || !workVar.empty() ) {
            if ( !workFn.empty() ) {
                auto f = workFn.back();
                workFn.pop_back();
                if ( f->module == thisModule.get() ) f->visit(vis);
                for ( auto df : f->useFunctions ) pushFn(df);
                for ( auto dv : f->useGlobalVariables ) pushVar(dv);
            } else {
                auto v = workVar.back();
                workVar.pop_back();
                if ( v->module == thisModule.get() ) visitGlobalVariable(bvis, v);
                for ( auto df : v->useFunctions ) pushFn(df);
                for ( auto dv : v->useGlobalVariables ) pushVar(dv);
            }
        }
        vis.propagateFunctionUse(fun);
    }

    void Program::markSymbolUse(bool builtInSym, bool forceAll, bool initThis, Module * macroModule, TextWriter * logs) {
        clearSymbolUse();
        MarkSymbolUse vis(this, builtInSym);
        vis.tw = logs;
        visit(vis);
        vis.markUsedFunctions(library, forceAll, initThis, macroModule);
        vis.markVarsUsed(library, forceAll);
    }

    void Program::removeUnusedSymbols() {
        if ( options.getBoolOption("remove_unused_symbols",true) ) {
            ClearUnusedSymbols cvis(this, thisModule.get());
            visit(cvis);
            MarkSymbolUse vis(this, false);
            vis.RemoveUnusedSymbols(*thisModule);
        }
    }

    void Program::dumpSymbolUse(TextWriter & logs) {
        logs << "USED SYMBOLS ARE:\n";
        for (auto & pm : library.modules) {
            for ( auto & var : pm->globals.each() ) {
                if ( isUsed(var) ) {
                    logs << "let " << var->module->name << "::" << var->name << ": " << var->type->describe() << "\n";
                }
            }
            for ( auto & func : pm->functions.each() ) {
                if ( isUsed(func) ) {
                    logs << func->module->name << "::" << func->describe() << "\n";
                }
            }
        }
        logs << "\n\n";
    }

    struct DependencyCollector {
        void collect ( const Function * fun ) {
            if ( functions.find(fun) != functions.end() ) return;
            functions.insert(fun);
            for ( const auto & depF : fun->useFunctions ) {
                collect(depF);
            }
            for ( const auto & depV : fun->useGlobalVariables ) {
                collect(depV);
            }
        }
        void collect ( const Variable * var ) {
            if ( variables.find(var) != variables.end() ) return;
            variables.insert(var);
            for ( const auto & depF : var->useFunctions ) {
                collect(depF);
            }
            for ( const auto & depV : var->useGlobalVariables ) {
                collect(depV);
            }
        }
        using FunAndName = pair<const Function *,string>;
        vector<FunAndName> getDependenciesByMangledName() {
            vector<FunAndName> vec;
            vec.reserve(functions.size());
            for ( const auto & it : functions ) {
                vec.emplace_back(make_pair(it,it->getMangledName()));
            }
            sort(vec.begin(), vec.end(), [&](const FunAndName & a, const FunAndName & b){
                return a.second < b.second;
            });
            DAS_ASSERT(adjacent_find(vec.begin(), vec.end(), [](const FunAndName & a, const FunAndName & b){ return a.second == b.second; }) == vec.end());
            return vec;
        }
        using VarAndName = pair<const Variable *,string>;
        vector<VarAndName> getVariableDependenciesByMangledName() {
            vector<VarAndName> vec;
            vec.reserve(variables.size());
            for ( const auto & it : variables ) {
                vec.emplace_back(make_pair(it,it->getMangledName()));
            }
            sort(vec.begin(), vec.end(), [&](const VarAndName & a, const VarAndName & b){
                return a.second < b.second;
            });
            DAS_ASSERT(adjacent_find(vec.begin(), vec.end(), [](const VarAndName & a, const VarAndName & b){ return a.second == b.second; }) == vec.end());
            return vec;
        }
        das_hash_set<const Function *> functions;
        das_hash_set<const Variable *> variables;
    };

    DAS_CC_API void collectDependencies ( FunctionPtr fun, const TBlock<void,TArray<Function *>,TArray<Variable *>> & block, Context * context, LineInfoArg * line ) {
        auto program = daScriptEnvironment::getBound()->g_Program;
        if ( !program ) context->throw_error_at(line, "Can't collect dependencies outside of compilation.");
        program->markFunctionDependencies(fun);
        DependencyCollector collector;
        collector.collect(fun);
        auto vecFunc = collector.getDependenciesByMangledName();
        auto vecVars = collector.getVariableDependenciesByMangledName();
        vector<const Function *> tfun;
        tfun.reserve(vecFunc.size());
        for ( auto & fn : vecFunc ) {
            tfun.push_back(fn.first);
        }
        vector<const Variable *> tvar;
        tvar.reserve(vecVars.size());
        for ( auto & fn : vecVars ) {
            tvar.push_back(fn.first);
        }
        // Always call array_mark_locked, even when the dependency list is empty: an
        // un-marked Array carries uninitialised `magic` which fails the lock-magic check
        // on the das-side `for (... in ...)` iterator, crashing the macro with
        // "array magic mismatch on lock" on a function whose body references neither
        // a function dependency nor a global variable (e.g. an empty-body [compute_shader]).
        Array afun, avar;
        array_mark_locked(afun, tfun.size() ? (void*)tfun.data() : nullptr, uint32_t(tfun.size()));
        array_mark_locked(avar, tvar.size() ? (void*)tvar.data() : nullptr, uint32_t(tvar.size()));
        vec4f args[2];
        args[0] = cast<Array &>::from(afun);
        args[1] = cast<Array &>::from(avar);
        context->invoke(block, args, nullptr, line);
    }

    uint64_t getFunctionAotHash ( Function * fun ) {
        if ( fun->aotHash != 0 ) return fun->aotHash; // cached
        DependencyCollector collector;
        collector.collect(fun);
        auto vec = collector.getDependenciesByMangledName();
        vector<uint64_t> uvec;
        uvec.reserve(vec.size() + 1);
        debug_aot_hash("HASH %s %" PRIx64 "\n", fun->getMangledName().c_str(), fun->hash);
        uvec.push_back(fun->hash);
        for ( const auto & fn : vec ) {
            if ( !fn.first->noAot &&!fn.first->builtIn ) {
                DAS_ASSERTF(fn.first->hash,"%s has dependency on %s, which hash hash of 0",
                    fun->getMangledName().c_str(), fn.first->getMangledName().c_str());
                uvec.push_back(fn.first->hash);
                debug_aot_hash("\t%s %" PRIx64 "\n", fn.second.c_str(), fn.first->hash);
            }
        }
        uint64_t res = hash_block64((const uint8_t *)uvec.data(), uint32_t(uvec.size()*sizeof(uint64_t)));
        debug_aot_hash("AOT HASH %" PRIx64 "\n", res);
        fun->aotHash = res;
        return res;
    }

    string getAotHashComment ( const Function * fun ) {
        DependencyCollector collector;
        collector.collect(fun);
        auto vec = collector.getDependenciesByMangledName();
        TextWriter tw;
        tw << fun->getMangledName() << " hash=0x" << HEX << fun->hash;
        for ( const auto & fn : vec ) {
            if ( !fn.first->noAot && !fn.first->builtIn && fn.first != fun ) {
                tw << ", " << fn.second << "=0x" << fn.first->hash;
            }
        }
        tw << DEC;
        return tw.str();
    }

    uint64_t getVariableListAotHash ( const vector<const Variable *> & globs, uint64_t initHash ) {
        DependencyCollector collector;
        for ( const auto & var : globs ) {
            collector.collect(var);
        }
        auto vec = collector.getDependenciesByMangledName();
        vector<uint64_t> uvec;
        uvec.reserve(vec.size() + 1);
        uvec.push_back(initHash);
        for ( const auto & fn : vec ) {
            if ( !fn.first->noAot ) {
                uvec.push_back(fn.first->hash);
            }
        }
        return hash_block64((const uint8_t *)uvec.data(), uint32_t(uvec.size()*sizeof(uint64_t)));
    }
}
