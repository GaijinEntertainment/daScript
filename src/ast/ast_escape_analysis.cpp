#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_escape_analysis.h"

namespace das {

    // Escape analysis and the optimization that consumes it are two separate passes, so the analysis
    // result (the Variable escape flags) stands on its own and the AST mutation is separable:
    //
    //   ANALYSIS (escapeAnalysis) - three pieces with a clear data boundary between them:
    //     1. EscapeGraph        - PURE DATA: a flow graph over pointer-valued variables + per-function
    //                             return summaries. No logic.
    //     2. EscapeGraphBuilder - one whole-program walk that POPULATES an EscapeGraph (sinks, flow
    //                             edges, return summaries, work lists). No propagation.
    //     3. EscapeSolver       - CONSUMES the graph: two monotone fixpoints (escape + fresh-owned),
    //                             then writes the result onto Variable::does_not_escape / escape_* /
    //                             escape_no_stack. No AST walking.
    //
    //   OPTIMIZATION (scopeFreeOptimization, ScopeFreeVisitor) - consumes Variable::does_not_escape and
    //     emits a deterministic free at the end of the declaring block (scope exit), instead of leaving
    //     the object to the heap GC. Its "did the AST change" result drives a re-infer.
    //
    // The analysis is conservative: a use is "safe" only when recognized (field-base read, escape-
    // neutral argument, copy alias, forwarded ownership). Any unrecognized use leaks the pointer, so
    // under-approximating the safe uses keeps it sound.

    static bool isFreshAlloc ( Expression * init ) {
        // `new T()` is an ExprNew; `new T(field=val)` lowers to ExprAscend over ExprMakeStruct.
        // both hand the local a freshly-allocated, uniquely-owned heap object.
        return init && ( init->rtti_isNewExpr() || init->rtti_isAscend() );
    }

    // type shape we can scope-free: a freshly-allocated, non-smart pointer to a daslang struct.
    static bool isEscapeFreePtr ( const TypeDeclPtr & typ ) {
        if ( !typ ) return false;
        if ( typ->ref || typ->constant ) return false;
        if ( typ->baseType != Type::tPointer ) return false;
        if ( typ->smartPtr ) return false;
        if ( !typ->firstType || typ->firstType->baseType != Type::tStructure ) return false;
        return typ->canDelete();
    }

    // shared gates for a scope-free candidate local: a freeable pointer-to-struct in a function
    // whose scope-exit semantics are well-defined (not generated/generator/lambda; unsafe excluded
    // as it can hide aliasing the field-base analysis cannot see)
    static bool isFreeCandidateShape ( Function * func, ExprLet * expr, const VariablePtr & var ) {
        return func
            && !func->generated && !func->generator && !func->lambda
            && !func->hasUnsafe
            && !expr->inScope && !var->inScope
            && !var->consumed && !var->single_return_via_move && !var->early_out
            && isEscapeFreePtr(var->type);
    }

    // a local pointer is a candidate for scope-free iff it is freshly allocated and type-eligible
    static bool isEscapeFreeCandidate ( Function * func, ExprLet * expr, const VariablePtr & var ) {
        return isFreeCandidateShape(func, expr, var) && isFreshAlloc(var->init);
    }

    // strip pure pointer<->ref / ref->value representation nodes, returning the underlying expression
    static Expression * peelRefNodes ( Expression * e ) {
        while ( e ) {
            if ( e->rtti_isPtr2Ref() ) e = ((ExprPtr2Ref *)e)->subexpr;
            else if ( e->rtti_isRef2Ptr() ) e = ((ExprRef2Ptr *)e)->subexpr;
            else if ( e->rtti_isR2V() ) e = ((ExprRef2Value *)e)->subexpr;
            else break;
        }
        return e;
    }

    // the exact ExprVar a value dereferences to (after peeling representation nodes), or null
    static ExprVar * derefBaseVar ( Expression * e ) {
        e = peelRefNodes(e);
        return ( e && e->rtti_isVar() ) ? (ExprVar *) e : nullptr;
    }

    // the called function, if the expression is a direct call (after peeling), or null
    static Function * derefCallee ( Expression * e ) {
        e = peelRefNodes(e);
        return ( e && e->rtti_isCallFunc() ) ? ((ExprCallFunc *)e)->func : nullptr;
    }

    // positional index of `arg` in the call's argument list (~0 if not found)
    static size_t callArgIndex ( ExprLooksLikeCall * call, Expression * arg ) {
        for ( size_t i=0; i!=call->arguments.size(); ++i ) {
            if ( call->arguments[i]==arg ) return i;
        }
        return ~size_t(0);
    }

    // a by-value (non-ref) pointer to a daslang struct - the value whose escape we can track
    static bool isPointerToStruct ( const TypeDeclPtr & typ ) {
        return typ && !typ->ref && typ->baseType==Type::tPointer && !typ->smartPtr
            && typ->firstType && typ->firstType->baseType==Type::tStructure;
    }

    static bool isParamEscapeCandidate ( const VariablePtr & var ) {
        return isPointerToStruct(var->type);
    }

    // a function whose body we can soundly analyze for parameter escape: visible body, no hidden
    // aliasing via unsafe, not a generated / generator / lambda shape the field-base analysis can't model
    static bool isParamAnalyzableFunc ( Function * func ) {
        return func && !func->builtIn && !func->stub && !func->isTemplate
            && !func->generated && !func->generator && !func->lambda && !func->hasUnsafe;
    }

    // can a pointer passed at positional `argIndex` of this call escape through the callee? returns
    // true when it CANNOT (escape-neutral for that one argument). built-ins are judged by their
    // declared side effects + a return that can't carry the pointer out; script functions by the
    // interprocedural per-parameter result computed in ParamEscapeAnalysis (which already folds in the
    // return / global-store / store-into-another-arg / transitive-call channels).
    static bool isArgEscapeNeutral ( Function * fn, size_t argIndex ) {
        if ( !fn || fn->unsafeOperation ) return false;
        if ( fn->builtIn ) {
            if ( fn->sideEffectFlags != 0 ) return false;
            auto res = fn->result;
            return res && !res->ref && (res->isVoid() || res->isWorkhorseType());
        }
        if ( argIndex >= fn->arguments.size() ) return false;
        return fn->arguments[argIndex]->does_not_escape;
    }

    static bool finalListFreesVar ( ExprBlock * block, Variable * var ) {
        class VarRefFinder : public Visitor {
        public:
            VarRefFinder ( Variable * v ) : var(v) {}
            bool found = false;
        protected:
            virtual ExpressionPtr visit ( ExprVar * e ) override {
                if ( e->variable == var ) found = true;
                return Visitor::visit(e);
            }
            Variable * var;
        };

        VarRefFinder finder(var);
        for ( auto & fe : block->finalList ) {
            fe->visit(finder);
            if ( finder.found ) return true;
        }
        return false;
    }

    enum class EscapeKind {
        DoesNotEscape,
        Return,
        Argument,
        Global
    };

    static void recordEscape ( Variable * var, EscapeKind kind ) {
        switch ( kind ) {
            case EscapeKind::DoesNotEscape: var->does_not_escape = true;  break;
            case EscapeKind::Return:        var->escapes_return = true;   break;
            case EscapeKind::Argument:      var->escapes_argument = true; break;
            case EscapeKind::Global:        var->escapes_global = true;   break;
        }
    }

    static const char * escapeKindName ( EscapeKind kind ) {
        switch ( kind ) {
            case EscapeKind::DoesNotEscape: return "does not escape";
            case EscapeKind::Return:        return "escapes via return";
            case EscapeKind::Argument:      return "escapes via argument";
            case EscapeKind::Global:        return "escapes via store";
        }
        return "?";
    }

    static void logEscape ( TextWriter * logs, Function * func, Variable * var, EscapeKind kind ) {
        if ( !logs ) return;
        if ( !var->at.empty() && var->at.fileInfo ) {
            *logs << var->at.fileInfo->name << ":" << var->at.line << ":" << var->at.column << " ";
        }
        *logs << "escape analysis: '" << var->name << "' " << escapeKindName(kind)
              << " in '" << func->module->name << "::" << func->name << "'\n";
    }

    // ===== Escape graph =====
    // The product of one whole-program walk: a flow graph over pointer-valued variables (parameters
    // and locals alike) plus per-function return summaries. PURE DATA - no analysis logic. Built by
    // EscapeGraphBuilder, consumed by EscapeSolver.
    //
    // Model: Choi et al., "Escape Analysis for Java" (OOPSLA'99) connection graph + the three-state
    // NoEscape/ArgEscape/GlobalEscape lattice; backward flow-insensitive propagation as in Gay &
    // Steensgaard, "Fast Escape Analysis and Stack Allocation" (CC'00).
    struct EscapeGraph {
        // SEEDS - direct facts read straight off the syntax:
        das_hash_map<Variable *, EscapeKind>           sinks;        // a var used in a leaking position, + how
        das_hash_map<Function *, bool>                 freshOwned;   // optimistic: f returns a uniquely-owned fresh object
        // EDGES - relations the solver propagates to a fixpoint:
        das_hash_map<Variable *, vector<Variable *>>   revDep;       // escapes(src) |= escapes(dst); revDep[dst] = {src...}
        das_hash_map<Function *, vector<Function *>>   retForward;   // freshOwned(f) &= freshOwned(g) for `return g(...)`
        // WORK LISTS - what to materialize once the fixpoint settles:
        vector<pair<Variable *, Function *>>           paramFn;      // analyzable params       -> Variable::does_not_escape
        vector<pair<Variable *, Function *>>           freshLocalFn; // fresh-alloc local owners -> EscapeKind

        struct CallResultLet { Variable * var; Function * callee; Function * fn; };
        vector<CallResultLet>                          callResultLets; // `var c = f(...)` candidates for ownership transfer
        das_set<Variable *>                            aliasedLocal; // copied/captured local   -> not stack-relocatable
    };

    // ===== Escape graph builder =====
    // ONE pass over analyzed function bodies. Its only job is to POPULATE an EscapeGraph: record
    // direct escape sinks, alias/argument flow edges, per-function return summaries, and the
    // variables to materialize. It performs NO propagation - that is the solver's job.
    //   input:  the set of functions we are allowed to analyze
    //   output: a populated EscapeGraph (returned by the static build() entry point)
    class EscapeGraphBuilder : public Visitor {
    public:
        static EscapeGraph build ( Program * prog, const das_set<Function *> & analyzed ) {
            EscapeGraphBuilder builder(analyzed);
            prog->visit(builder);
            return move(builder.graph);
        }
    protected:
        EscapeGraphBuilder ( const das_set<Function *> & analyzed_ ) : analyzed(analyzed_) {}

        // ---- recording helpers (the only writers of `graph`) ----
        void sink ( Variable * v, EscapeKind kind ) {                  // a direct leak (seed)
            if ( graph.sinks.find(v)==graph.sinks.end() ) graph.sinks[v] = kind;
        }
        void flowEdge ( Variable * src, Variable * dst ) {             // escapes(src) |= escapes(dst)
            graph.revDep[dst].push_back(src);
        }

        // ---- classification helpers ----
        EscapeKind depthKind () const {                                // how an unrecognized use leaks, by context
            if ( returnDepth > 0 ) return EscapeKind::Return;
            if ( argDepth > 0 ) return EscapeKind::Argument;
            return EscapeKind::Global;
        }

        // ---- per-function setup ----
        virtual bool canVisitFunction ( Function * fun ) override {
            return analyzed.find(fun)!=analyzed.end();
        }
        virtual void preVisit ( Function * fun ) override {
            func = fun; returnDepth = 0; argDepth = 0; makeBlockDepth = 0;
            safeBase.clear();
            for ( auto & arg : fun->arguments )
                if ( isParamEscapeCandidate(arg) ) graph.paramFn.push_back({arg, fun});
            graph.freshOwned[fun] = isPointerToStruct(fun->result);    // optimistic; a non-fresh return revokes it
        }
        virtual FunctionPtr visit ( Function * fun ) override { func = nullptr; return fun; }

        // ---- variables to materialize ----
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool ) override {
            if ( isEscapeFreeCandidate(func, expr, var) ) {
                graph.freshLocalFn.push_back({var, func});                            // `var x = new T`
            } else if ( isFreeCandidateShape(func, expr, var) ) {
                if ( auto callee = derefCallee(var->init) )                           // `var c = f(...)`
                    graph.callResultLets.push_back({var, callee, func});
            }
            return var;
        }

        // ---- SAFE (non-escaping) uses: pre-marked BEFORE the inner ExprVar is visited ----
        // a plain copy `var x = y` makes x an alias of y: y escapes only if x does (flow edge), and a
        // copied-from LOCAL can no longer be stack-relocated (the alias would dangle on relocation)
        virtual void preVisitLetInit ( ExprLet *, const VariablePtr & var, Expression * init ) override {
            if ( !isPointerToStruct(var->type) || var->init_via_move || var->init_via_clone || isFreshAlloc(init) ) return;
            if ( auto src = derefBaseVar(init) ) {
                if ( (src->local || src->argument) && isPointerToStruct(src->variable->type) ) {
                    safeBase.insert(src);
                    flowEdge(src->variable, var);
                    if ( src->local ) graph.aliasedLocal.insert(src->variable);
                }
            }
        }
        virtual void preVisit ( ExprField * expr ) override {          // a field read never leaks the base
            if ( auto v = derefBaseVar(expr->value) ) safeBase.insert(v);
        }
        virtual void preVisit ( ExprSafeField * expr ) override {      // `p?.x` is a distinct hook from ExprField
            if ( auto v = derefBaseVar(expr->value) ) safeBase.insert(v);
        }
        virtual void preVisit ( ExprOp2 * expr ) override {            // a null-guard `p == null` doesn't leak p
            if ( isArgEscapeNeutral(expr->func, 0) ) { if ( auto v = derefBaseVar(expr->left) ) safeBase.insert(v); }
            if ( isArgEscapeNeutral(expr->func, 1) ) { if ( auto v = derefBaseVar(expr->right) ) safeBase.insert(v); }
        }
        virtual void preVisitCallArg ( ExprCall * call, Expression * arg, bool ) override {
            argDepth++;
            auto src = derefBaseVar(arg);
            if ( !src ) return;
            auto fn = call->func;
            auto idx = callArgIndex(call, arg);
            if ( fn && !fn->builtIn && !fn->unsafeOperation && analyzed.find(fn)!=analyzed.end()
                 && idx < fn->arguments.size() && isParamEscapeCandidate(fn->arguments[idx]) ) {
                safeBase.insert(src);                                  // analyzed callee: arg escapes iff its param does
                flowEdge(src->variable, fn->arguments[idx]);
            } else if ( isArgEscapeNeutral(call->func, idx) ) {
                safeBase.insert(src);                                  // builtin / opaque callee that can't retain it
            }
            // otherwise the argument leaks: handled as a direct sink in visit(ExprVar)
        }
        virtual ExpressionPtr visitCallArg ( ExprCall *, Expression * arg, bool ) override {
            argDepth--; return arg;
        }
        // a string builder ("{x}") formats each element into a FRESH string by a read-only walk of its
        // value; it never retains the pointer, so a base used only here does not escape - like a field
        // read. (A custom string-cast lowers to a separate call element, handled by preVisitCallArg.)
        virtual void preVisitStringBuilderElement ( ExprStringBuilder *, Expression * expr, bool ) override {
            if ( auto v = derefBaseVar(expr) ) safeBase.insert(v);
        }

        // ---- scope tracking that affects classification ----
        virtual void preVisit ( ExprReturn * expr ) override {
            returnDepth++;
            if ( !func || !isPointerToStruct(func->result) ) return;
            auto e = peelRefNodes(expr->subexpr);
            if ( e && isFreshAlloc(e) ) {
                // returns a freshly-allocated object - keeps fresh-owned candidacy (no-op)
            } else if ( e && e->rtti_isCallFunc() && ((ExprCallFunc *)e)->func ) {
                graph.retForward[func].push_back(((ExprCallFunc *)e)->func);
            } else {
                graph.freshOwned[func] = false;   // parameter / global / field / null / ... - not fresh-owned
            }
        }
        virtual ExpressionPtr visit ( ExprReturn * expr ) override { returnDepth--; return expr; }
        // a block captures by reference and cannot be stored/returned: its captures do not escape, but
        // (like a copy alias) the reference forbids stack relocation of the captured pointee
        virtual void preVisit ( ExprMakeBlock * ) override { makeBlockDepth++; }
        virtual ExpressionPtr visit ( ExprMakeBlock * expr ) override { makeBlockDepth--; return expr; }

        // ---- the leak classifier: any tracked use not pre-marked safe is a direct sink ----
        virtual ExpressionPtr visit ( ExprVar * expr ) override {
            const auto trackedVar = expr->variable && (expr->local || expr->argument)
                && isPointerToStruct(expr->variable->type);
            if ( trackedVar ) {
                if ( makeBlockDepth > 0 && expr->local ) graph.aliasedLocal.insert(expr->variable);
                if ( safeBase.find(expr)==safeBase.end() ) sink(expr->variable, depthKind());
            }
            return expr;
        }

        EscapeGraph graph;                            // the product being built
        const das_set<Function *> & analyzed;         // input: functions we may analyze
        Function * func = nullptr;                    // walk state below
        int returnDepth = 0, argDepth = 0, makeBlockDepth = 0;
        das_set<ExprVar *> safeBase;                  // ExprVar uses already accounted as non-leaking
    };

    // ===== Escape solver =====
    // Consumes an EscapeGraph and produces the final per-variable result. Two monotone fixpoints over
    // the graph's edges, then write-back onto the AST:
    //   1. escape:      propagate the escape bit backward along flow edges (worklist).
    //   2. fresh-owned: revoke a function's fresh-owned summary if any forwarded callee is not.
    //   3. materialize: Variable::does_not_escape (params + freeable locals), EscapeKind, and
    //      escape_no_stack (freeable but copied/captured -> freed at scope exit, not relocated).
    class EscapeSolver {
    public:
        EscapeSolver ( EscapeGraph g, TextWriter * logs_ ) : graph(move(g)), logs(logs_) {
            for ( auto & s : graph.sinks ) esc[s.first] = s.second;   // seed the escape fixpoint
        }
        bool solveAndApply () {
            solveEscape();
            solveFreshOwned();
            // `var c = f(...)` where f returns a uniquely-owned fresh object and c does not itself
            // escape the caller: ownership transfers, so the caller frees c at scope exit.
            for ( auto & cr : graph.callResultLets ) {
                if ( graph.freshOwned[cr.callee] && !escapes(cr.var) )
                    graph.freshLocalFn.push_back({cr.var, cr.fn});
            }
            return materialize();
        }
    protected:
        // a variable's escape verdict IS its EscapeKind: DoesNotEscape (the map default) means it does
        // not escape; any other kind means it does, and records how. No separate boolean needed.
        EscapeKind escapeOf ( Variable * v ) { return esc[v]; }
        bool escapes ( Variable * v ) { return escapeOf(v) != EscapeKind::DoesNotEscape; }

        // backward reachability: a var escapes if any value it flowed into escapes
        void solveEscape () {
            vector<Variable *> work;
            for ( auto & kv : esc ) if ( kv.second != EscapeKind::DoesNotEscape ) work.push_back(kv.first);
            while ( !work.empty() ) {
                auto v = work.back(); work.pop_back();
                auto it = graph.revDep.find(v);
                if ( it==graph.revDep.end() ) continue;
                auto vkind = esc[v];   // capture BEFORE touching esc[src], whose insert may rehash esc
                for ( auto src : it->second ) {
                    auto & k = esc[src];
                    if ( k==EscapeKind::DoesNotEscape ) { k = vkind; work.push_back(src); }
                }
            }
        }
        // a function is fresh-owned iff every return is fresh or forwards another fresh-owned function
        void solveFreshOwned () {
            bool changed = true;
            while ( changed ) {
                changed = false;
                for ( auto & kv : graph.retForward ) {
                    if ( !graph.freshOwned[kv.first] ) continue;
                    for ( auto g : kv.second )
                        if ( !graph.freshOwned[g] ) { graph.freshOwned[kv.first] = false; changed = true; break; }
                }
            }
        }
        // write the settled result onto the AST
        bool materialize () {
            bool any = false;
            for ( auto & pf : graph.paramFn ) {
                bool e = escapes(pf.first);
                pf.first->does_not_escape = !e;
                if ( e && logs ) logParam(pf.second, pf.first);
                any = true;
            }
            for ( auto & lf : graph.freshLocalFn ) {
                auto kind = escapeOf(lf.first);
                recordEscape(lf.first, kind);
                // freeable but copied/captured: free at scope exit, but do NOT stack-relocate (the
                // surviving alias/reference would dangle once the pointee moves into the frame)
                if ( kind==EscapeKind::DoesNotEscape && graph.aliasedLocal.find(lf.first)!=graph.aliasedLocal.end() ) {
                    lf.first->escape_no_stack = true;
                }
                logEscape(logs, lf.second, lf.first, kind);
                any = true;
            }
            return any;
        }
        void logParam ( Function * fn, Variable * var ) {
            if ( !var->at.empty() && var->at.fileInfo )
                *logs << var->at.fileInfo->name << ":" << var->at.line << ":" << var->at.column << " ";
            *logs << "escape analysis: parameter '" << var->name << "' escapes in '"
                  << fn->module->name << "::" << fn->name << "'\n";
        }

        EscapeGraph                          graph;   // input graph (also holds the promoted free list)
        das_hash_map<Variable *, EscapeKind> esc;     // per-variable escape fixpoint state (default = DoesNotEscape)
        TextWriter *                         logs = nullptr;
    };

    // ===== Scope-free optimization (consumes Variable::does_not_escape) =====

    // set the stack-allocation flag on the fresh-alloc init expression; returns true if it changed it.
    // only the shapes the codegen tiers can stack-allocate: a plain `new T()` (no initializer call,
    // no dim) over a plain struct, and `new T(f=v)` which lowers to an ascend over a make-struct.
    // capped by size. classes / constructors / handles are excluded: their construction has semantics
    // (ctor invocation, rtti) that the plain in-frame build does not reproduce (breaks under JIT).
    static bool setAllocateOnStack ( Expression * init ) {
        // upper bound on a pointee we are willing to move onto the (fixed-size) stack frame
        static constexpr uint32_t MAX_STACK_ALLOC_SIZE = 256;
        if ( init && init->rtti_isNewExpr() ) {
            auto en = (ExprNew *) init;
            auto st = en->typeexpr ? en->typeexpr->structType : nullptr;
            bool persistent = st && st->persistent;
            bool isClass = st && st->isClass;
            // use the non-asserting 64-bit size: a malformed/oversized type (e.g. in invalid_types.das)
            // yields a huge value that simply fails the cap, instead of tripping the size<=0x7fffffff assert
            bool fits = en->typeexpr && en->typeexpr->getBaseSizeOf64() <= uint64_t(MAX_STACK_ALLOC_SIZE);
            if ( !en->initializer && en->typeexpr && en->typeexpr->baseType!=Type::tFixedArray && !persistent && !isClass && fits && !en->allocate_on_stack ) {
                en->allocate_on_stack = true; return true;
            }
        } else if ( init && init->rtti_isAscend() ) {
            auto ea = (ExprAscend *) init;
            bool plainStruct = false;
            if ( ea->subexpr && ea->subexpr->rtti_isMakeStruct() ) {
                auto mks = (ExprMakeStruct *) ea->subexpr;
                auto mkT = mks->makeType;
                while ( mkT && mkT->baseType==Type::tFixedArray && mkT->firstType ) mkT = mkT->firstType;
                auto st = mkT ? mkT->structType : nullptr;
                // note: isNewClass means "make-struct produced by a `new`" (set for plain structs too),
                // NOT "is a class" - the class signal is the constructor / forceClass / structType->isClass
                plainStruct = !mks->constructor && !mks->isNewHandle
                    && !mks->forceClass && !( st && st->isClass );
            }
            auto subT = ea->subexpr ? ea->subexpr->type : nullptr;
            while ( subT && subT->baseType==Type::tFixedArray && subT->firstType ) subT = subT->firstType;
            bool persistent = subT && subT->structType && subT->structType->persistent;
            bool fits = ea->subexpr && ea->subexpr->type && ea->subexpr->type->getSizeOf64() <= uint64_t(MAX_STACK_ALLOC_SIZE);
            if ( plainStruct && !persistent && fits && !ea->allocate_on_stack ) {
                ea->allocate_on_stack = true; return true;
            }
        }
        return false;
    }

    static bool initIsAllocatedOnStack ( Expression * init ) {
        if ( init && init->rtti_isNewExpr() ) return ((ExprNew *)init)->allocate_on_stack;
        if ( init && init->rtti_isAscend() ) return ((ExprAscend *)init)->allocate_on_stack;
        return false;
    }

    class ScopeFreeVisitor : public Visitor {
    public:
        bool anyWork = false;
        ScopeFreeVisitor ( TextWriter * logs_, bool forceStack_ ) : logs(logs_), forceStack(forceStack_) {}
    protected:
        virtual bool canVisitFunction ( Function * fun ) override {
            return !fun->stub && !fun->isTemplate;
        }
        virtual void preVisit ( Function * fun ) override {
            Visitor::preVisit(fun);
            func = fun;
            pending.clear();
        }
        virtual FunctionPtr visit ( Function * fun ) override {
            func = nullptr;
            return Visitor::visit(fun);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            blocks.push_back(block);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            for ( auto & p : pending ) {
                if ( p.second != block ) continue;
                if ( finalListFreesVar(block, p.first) ) continue;   // already emitted on a prior pass
                emitScopeFree(block, p.first);
            }
            blocks.pop_back();
            return Visitor::visit(block);
        }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            if ( var->does_not_escape && !blocks.empty() ) {
                // put the pointee in the stack frame instead of the heap; idempotent so the
                // notInferred re-infer loop terminates once the flag is already set
                if ( forceStack && !var->escape_no_stack && setAllocateOnStack(var->init) ) {
                    anyWork = true;
                    func->notInferred();
                }
                // emit the scope-free only when it frees something real: a heap shell (not stack-
                // allocated) or owned heap members (arrays/tables) inside the pointee. a stack-
                // allocated POD frees nothing, so skip it - else every scope pays an interop call.
                bool stacked = forceStack && initIsAllocatedOnStack(var->init);
                bool ownsHeap = !( var->type->firstType && var->type->firstType->isNoHeapType() );
                if ( !stacked || ownsHeap ) {
                    pending.push_back({var, blocks.back()});
                }
            }
            return Visitor::visitLet(expr,var,last);
        }
        void emitScopeFree ( ExprBlock * block, Variable * var ) {
            if ( var->type->firstType->getSizeOf64() > 0x7fffffffull ) return;  // skip pointees over the 2^31 (32-bit) size limit - oversized/invalid types that error out anyway
            anyWork = true;
            func->notInferred();
            // GC-style raw free (no finalizer), matching what the heap GC would do for this garbage.
            auto call = new ExprCall(var->at, "_::builtin_scope_free");
            call->arguments.push_back(new ExprVar(var->at, var->name));
            call->arguments.push_back(new ExprConstUInt(var->at, var->type->firstType->getSizeOf()));
            call->alwaysSafe = true;
            block->finalList.insert(block->finalList.begin(), call);
            if ( logs ) {
                if ( !var->at.empty() && var->at.fileInfo ) {
                    *logs << var->at.fileInfo->name << ":" << var->at.line << ":" << var->at.column << " ";
                }
                *logs << "scope-free applied to '" << var->name << "' in '" << func->module->name << "::" << func->name << "'\n";
            }
        }
        Function * func = nullptr;
        TextWriter * logs = nullptr;
        bool forceStack = false;
        vector<ExprBlock *> blocks;
        vector<pair<Variable *, ExprBlock *>> pending;
    };

    // ANALYSIS: build the escape graph from one whole-program walk, solve it, and write the result
    // onto the AST (Variable::does_not_escape / escape_* / escape_no_stack). No AST mutation - the
    // flags ARE the analysis result, consumed by scopeFreeOptimization (and reusable by anything else).
    bool escapeAnalysis(Program * program, TextWriter & logs) {
        auto & options = program->options;
        auto & policies = program->policies;
        auto forceStack = options.getBoolOption("force_allocate_on_stack", policies.force_allocate_on_stack);
        if ( !options.getBoolOption("force_escape_free", policies.force_escape_free) && !forceStack ) return false;
        auto logEscape = options.getBoolOption("log_escape_analysis", policies.log_escape_analysis);
        das_set<Function *> analyzed;
        program->thisModule->functions.foreach([&](auto & fn){
            if ( isParamAnalyzableFunc(fn) ) analyzed.insert(fn);
        });
        EscapeGraph graph = EscapeGraphBuilder::build(program, analyzed);
        return EscapeSolver(move(graph), logEscape ? &logs : nullptr).solveAndApply();
    }

    // OPTIMIZATION: consume the analysis result, emitting scope-exit frees / stack relocation. Returns
    // whether the AST changed, so the caller re-infers the inserted scope_free calls.
    bool scopeFreeOptimization(Program * program, TextWriter & logs) {
        auto & options = program->options;
        auto & policies = program->policies;
        auto forceStack = options.getBoolOption("force_allocate_on_stack", policies.force_allocate_on_stack);
        if ( !options.getBoolOption("force_escape_free", policies.force_escape_free) && !forceStack ) return false;
        auto logEscape = options.getBoolOption("log_escape_analysis", policies.log_escape_analysis);
        ScopeFreeVisitor sfv(logEscape ? &logs : nullptr, forceStack);
        program->visit(sfv);
        return sfv.anyWork;
    }

}
