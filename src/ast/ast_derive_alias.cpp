#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/misc/das_common.h"
#include "ast_derive_alias.h"

namespace das {

    typedef das_hash_map<Variable *,Function *> IndirectSources;
    typedef das_hash_set<Variable *> ExpressionSources;

    void appendIndVariables ( Function * lookup, IndirectSources & indGlobalVariables, das_hash_set<Function *> & accessed ) {
        if ( accessed.find(lookup)!=accessed.end() ) return;
        accessed.insert(lookup);
        for ( auto var : lookup->useGlobalVariables ) {
            if ( indGlobalVariables.find(var)==indGlobalVariables.end() ) {
                indGlobalVariables[var] = lookup;
            }
        }
        for ( auto fun : ordered(lookup->useFunctions, StableOrder()) ) {
            appendIndVariables(fun, indGlobalVariables, accessed);
        }
    }

    static bool mayHaveAddressTaken ( const ProgramPtr & program, const FunctionPtr & func ) {
        return func->addressTaken || ( func->module != program->thisModule.get() && ( !func->privateFunction || func->fromGeneric ) );
    }

    bool isCompatibleFunction ( const FunctionPtr & func, const TypeDeclPtr & inv ) {
        if ( func->arguments.size()!=inv->argTypes.size() ) return false;
        if ( !inv->firstType->isSameType(*func->result, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes) ) {
            return false;
        }
        for ( size_t i=0, is=func->arguments.size(); i!=is; ++i ) {
            if ( !inv->argTypes[i]->isSameType(*func->arguments[i]->type, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, AllowSubstitute::yes, false, false) ) {
                return false;
            }
        }
        return true;
    }

    void collectInvokeIndVariables ( const ProgramPtr & program, const TypeDeclPtr & inv, IndirectSources & sources ) {
        das_hash_set<Function *> accessed;
        program->library.foreach([&](Module * mod){
            mod->functions.foreach([&](const FunctionPtr & gfunc){
                // not built-in, used, address taken, can potentially alias, compatible
                if ( gfunc->isTemplate ) return;
                if ( !gfunc->builtIn && mayHaveAddressTaken(program, gfunc) && !gfunc->aliasCMRES && program->isUsed(gfunc) && isCompatibleFunction(gfunc, inv) ) {
                    appendIndVariables(gfunc, sources, accessed);
                }
            });
            return true;
        },"*");
    }

    bool isCompatibleLambdaFunction ( const FunctionPtr & func, const TypeDeclPtr & inv ) {
        if ( func->arguments.size()!=inv->argTypes.size()+1 ) return false;
        if ( !inv->firstType->isSameType(*func->result, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes) ) {
            return false;
        }
        for ( size_t i=0, is=inv->argTypes.size(); i!=is; ++i ) {    // note, we skip 1st argument
            if ( !inv->argTypes[i]->isSameType(*func->arguments[i+1]->type, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, AllowSubstitute::yes, false, false) ) {
                return false;
            }
        }
        return true;
    }

    void collectInvokeIndLambdaVariables ( const ProgramPtr & program, const TypeDeclPtr & inv, IndirectSources & sources ) {
        das_hash_set<Function *> accessed;
        program->library.foreach([&](Module * mod){
            mod->functions.foreach([&](const FunctionPtr & gfunc){
                // not built-in, used, address taken, can potentially alias, compatible
                if ( gfunc->isTemplate ) return;
                if ( !gfunc->builtIn && mayHaveAddressTaken(program, gfunc) && !gfunc->aliasCMRES && gfunc->lambda && program->isUsed(gfunc) && isCompatibleLambdaFunction(gfunc, inv) ) {
                    appendIndVariables(gfunc, sources, accessed);
                }
            });
            return true;
        },"*");
    }

    typedef das_hash_map<Variable *,Expression *> LoopSources;

    // src/ast/ARCHITECTURE_ALIASING.md#in-place-build-aliases
    class SourceCollector : public Visitor {
    public:
        SourceCollector ( const ProgramPtr & prog, const LoopSources & loops, bool reads, bool anyG )
            : program(prog), loopSources(loops), allReads(reads), anyGlobals(anyG) {}
    protected:
        virtual bool canVisitStructureFieldInit ( Structure * ) override { return false; }
        virtual bool canVisitArgumentInit ( Function * , const VariablePtr &, Expression * ) override { return false; }
        virtual bool canVisitIfSubexpr ( ExprIfThenElse * ) override {
            return !skipDepth;
        }
        virtual bool canVisitExpr ( ExprTypeInfo *, Expression * )  override  { return false; }
        virtual bool canVisitMakeStructureBlock ( ExprMakeStruct *, Expression * )  override  { return false; }
        virtual bool canVisitMakeStructureBody ( ExprMakeStruct * )  override {
            return !skipDepth;
        }
        virtual bool canVisitQuoteSubexpression ( ExprQuote * )  override  { return false; }
        virtual bool canVisitWithAliasSubexpression ( ExprAssume * )  override  { return false; }
        virtual bool canVisitMakeBlockBody ( ExprMakeBlock * )  override  { return everyRead(); }
        bool everyRead () const { return allReads && !secondNameDepth; }
        void skipReads () { if ( !everyRead() ) skipDepth ++; }
        void resumeReads () { if ( !everyRead() ) skipDepth --; }
        bool passesSources ( const TypeDeclPtr & res ) const {
            return res->ref || res->isGoodIteratorType();
        }
        bool invokePassesSources ( const TypeDeclPtr & argT ) const {
            return !argT->isGoodBlockType() && passesSources(argT->firstType);
        }
        void addSources ( const IndirectSources & vars ) {
            for ( auto & v : vars ) {
                sources.insert(v.first);
            }
        }
        void resolveSecondName ( Variable * var, Expression * names ) {
            if ( resolved.find(var)!=resolved.end() ) return;
            resolved.insert(var);
            secondNameDepth ++;
            names->visit(*this);
            secondNameDepth --;
        }
    // we found source
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( skipDepth ) return;
            auto var = expr->variable;
            sources.insert(var);
            if ( !var->type->ref ) return;
            auto loop = loopSources.find(var);
            if ( loop!=loopSources.end() ) {
                resolveSecondName(var, loop->second);
            } else if ( expr->local && var->init ) {
                resolveSecondName(var, var->init);
            }
        }
    // []
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
        }
        virtual void preVisitAtIndex ( ExprAt * expr, Expression * index ) override {
            Visitor::preVisitAtIndex(expr,index);
            skipReads();
        }
        virtual ExpressionPtr visit ( ExprAt * expr ) override {
            resumeReads();
            return Visitor::visit(expr);
        }
    // ?[]
        virtual void preVisit ( ExprSafeAt * expr ) override {
            Visitor::preVisit(expr);
        }
        virtual void preVisitSafeAtIndex ( ExprSafeAt * expr, Expression * index ) override {
            Visitor::preVisitSafeAtIndex(expr,index);
            skipReads();
        }
        virtual ExpressionPtr visit ( ExprSafeAt * expr ) override {
            resumeReads();
            return Visitor::visit(expr);
        }
    // ?:
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            skipReads();
        }
        virtual void preVisitLeft  ( ExprOp3 * expr, Expression * left ) override {
            Visitor::preVisitLeft(expr,left);
            resumeReads();
        }
        virtual ExpressionPtr visit ( ExprOp3 * expr ) override {
            return Visitor::visit(expr);
        }
    // pointer deref - all bets are off
        virtual void preVisit ( ExprPtr2Ref * expr ) override {
            Visitor::preVisit(expr);
            if ( !skipDepth && !expr->assumeNoAlias ) alwaysAliases = true;
        }
    // call
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( !passesSources(expr->func->result) ) skipReads();
            if ( everyRead() && anyGlobals ) {
                IndirectSources globSrc;
                das_hash_set<Function *> accessed;
                appendIndVariables(expr->func, globSrc, accessed);
                addSources(globSrc);
            }
        }
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            if ( !passesSources(expr->func->result) ) resumeReads();
            return Visitor::visit(expr);
        }
    // invoke
        virtual void preVisit ( ExprInvoke * expr ) override {
            Visitor::preVisit(expr);
            auto argT = expr->arguments[0]->type;
            if ( !invokePassesSources(argT) ) skipReads();
            if ( !skipDepth && anyGlobals ) {
                IndirectSources globSrc;
                if ( argT->isGoodFunctionType() ) {
                    collectInvokeIndVariables(program, argT, globSrc);
                } else if ( argT->isGoodLambdaType() ) {
                    collectInvokeIndLambdaVariables(program, argT, globSrc);
                }
                addSources(globSrc);
            }
        }
        virtual ExpressionPtr visit ( ExprInvoke * expr ) override {
            auto argT = expr->arguments[0]->type;
            if ( !invokePassesSources(argT) ) resumeReads();
            return Visitor::visit(expr);
        }
    protected:
        int skipDepth = 0;
        int secondNameDepth = 0;
        das_hash_set<Variable *> resolved;
        ProgramPtr program;
        const LoopSources & loopSources;
        set<Expression *> invokeBlocks;
    public:
        ExpressionSources sources;
        bool alwaysAliases = false;
        bool allReads = false;
        bool anyGlobals = false;
    };

    bool collectSources ( const ProgramPtr & prog, const LoopSources & loops, Expression * expr, ExpressionSources & src, bool allReads, bool anyG ) {
        SourceCollector srr(prog, loops, allReads, anyG);
        expr->visit(srr);
        src = das::move(srr.sources);
        return srr.alwaysAliases;
    }

    bool appendSources ( const ProgramPtr & prog, const LoopSources & loops, Expression * expr, ExpressionSources & src, bool allReads, bool anyG ) {
        SourceCollector srr(prog, loops, allReads, anyG);
        expr->visit(srr);
        for ( auto s : srr.sources ) {
            src.insert(s);
        }
        return srr.alwaysAliases;
    }

    Variable * intersectSources ( ExpressionSources & a, ExpressionSources & b ) {
        if ( a.size() < b.size() ) {
            for ( auto va : a ) {
                if ( b.find(va)!=b.end() ) return va;
            }
            return nullptr;
        } else {
            for ( auto vb : b ) {
                if ( a.find(vb)!=a.end() ) return vb;
            }
            return nullptr;
        }
    }

    void deriveAliasing ( const FunctionPtr & func, TextWriter & logs, bool logAliasing ) {
        if ( func->arguments.size()==0 && func->useGlobalVariables.size()==0 && func->useFunctions.size()==0 ) return;
    // collect indirect global variables
        IndirectSources ind;
        das_hash_set<Function *> depInd;
        appendIndVariables(func,ind,depInd);
        if ( !(func->copyOnReturn || func->moveOnReturn) ) return;  // not a cmres function, we don't need cmres aliasing
        if ( func->arguments.size()==0 && ind.size()==0 ) return;   // no arguments, no globals, no cmres aliasing
    // collect type aliasing
        if ( logAliasing ) logs << "function " << func->getMangledName() << " returns by reference\n";
        das_set<Structure *> rdep;
        TypeAliasMap resTypeAliases;
        func->result->collectAliasing(resTypeAliases, rdep, false);
    // do arguments
        func->resultAliases.clear();
        for ( size_t i=0, is=func->arguments.size(); i!=is; ++i ) {
            if ( !(func->arguments[i]->type->isRef() || func->arguments[i]->type->baseType==Type::tPointer) ) {
                continue;
            }
            das_set<Structure *> dep;
            TypeAliasMap typeAliases;
            func->arguments[i]->type->collectAliasing(typeAliases, dep, false);
            for ( auto resT : resTypeAliases ) {
                for ( auto alias : typeAliases ) {
                    if ( alias.second.first->isSameType(*(resT.second.first),RefMatters::no,ConstMatters::no,TemporaryMatters::no,AllowSubstitute::yes,false,false) ) {
                        func->resultAliases.push_back(int(i));
                        if ( logAliasing ) logs << "\targument " << i << " aliasing result with type "
                            << func->arguments[i]->type->describe() << "\n";
                        goto nada;
                    }
                }
            }
            nada:;
        }
    // do globals
        func->resultAliasesGlobals.clear();
        for ( auto & it : ordered(ind, StableOrder()) ) {
            if ( !(it.first->type->isRef() || it.first->type->baseType==Type::tPointer) || it.first->type->temporary) {
                continue;
            }
            das_set<Structure *> dep;
            TypeAliasMap typeAliases;
            it.first->type->collectAliasing(typeAliases, dep, false);
            for ( auto resT : resTypeAliases ) {
                for ( auto alias : typeAliases ) {
                    if ( alias.second.first->isSameType(*(resT.second.first),RefMatters::no,ConstMatters::no,TemporaryMatters::no,AllowSubstitute::yes,false,false) ) {
                        Function::AliasInfo info = {it.first, it.second, resT.second.second || alias.second.second};
                        func->resultAliasesGlobals.emplace_back(info);
                        if ( logAliasing ) logs << "\tglobal variable " << it.first->getMangledName() << " aliasing result with type "
                            << it.first->type->describe() << (info.viaPointer ? " through pointer aliasing" : "") <<  "\n";
                        goto nadaHere;
                    }
                }
            }
            nadaHere:;
        }
    }

    class DestinationReads : public Visitor {
    public:
        DestinationReads ( Expression * dest ) : destType(dest->type), local(plainLocal(dest)) {}
        bool readBy ( Expression * value ) {
            value->visit(*this);
            return found;
        }
    protected:
        virtual bool canVisitStructureFieldInit ( Structure * ) override { return false; }
        virtual bool canVisitArgumentInit ( Function * , const VariablePtr &, Expression * ) override { return false; }
        virtual bool canVisitExpr ( ExprTypeInfo *, Expression * ) override { return false; }
        virtual bool canVisitQuoteSubexpression ( ExprQuote * ) override { return false; }
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            auto vt = expr->variable->type;
            bool arg = expr->argument || expr->block;
            if ( expr->variable==local ) found = true;
            else if ( vt->baseType==Type::tPointer ) read(vt->firstType, true);
            else if ( vt->ref ) read(vt, !arg);
            else if ( expr->variable->global || (arg && vt->isRefType()) ) read(vt, false);
        }
        virtual void preVisit ( ExprPtr2Ref * expr ) override {
            Visitor::preVisit(expr);
            read(expr->subexpr->type ? expr->subexpr->type->firstType : nullptr, true);
        }
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            readElement(expr->subexpr, expr->type);
        }
        virtual void preVisit ( ExprSafeAt * expr ) override {
            Visitor::preVisit(expr);
            readElement(expr->subexpr, expr->type);
        }
        virtual void preVisit ( ExprCall * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprNew * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprOp1 * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprOp2 * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprOp3 * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprCopy * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprMove * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprClone * expr ) override { Visitor::preVisit(expr); readCall(expr); }
        virtual void preVisit ( ExprInvoke * expr ) override { Visitor::preVisit(expr); read(nullptr, false); }
        virtual void preVisit ( ExprMakeBlock * expr ) override { Visitor::preVisit(expr); read(nullptr, false); }
        virtual void preVisit ( ExprMakeStruct * expr ) override {
            Visitor::preVisit(expr);
            bool init = expr->constructor || expr->useInitializer;
            bool userCtor = expr->constructor && !expr->constructor->generated;
            if ( expr->block || (init && (userCtor || initReads(expr))) ) read(nullptr, false);
        }
    private:
        void readElement ( Expression * container, TypeDecl * element ) {
            if ( !container->type->isFixedArray() && !container->type->isVectorType() ) read(element, false);
        }
        void readCall ( ExprCallFunc * call ) {
            if ( call->func && !pureValueCall(call->func) ) read(nullptr, false);
        }
        void read ( TypeDecl * type, bool through ) {
            if ( local && !through ) return;
            if ( mayHold(type) ) found = true;
        }
        bool mayHold ( TypeDecl * type ) {
            if ( !type ) return true;
            if ( destAliases.empty() ) {
                das_set<Structure *> dep;
                destType->collectAliasing(destAliases, dep, false);
            }
            TypeAliasMap typeAliases;
            das_set<Structure *> dep;
            type->collectAliasing(typeAliases, dep, false);
            auto same = [](TypeDecl * a, TypeDecl * b) {
                return a->isSameType(*b,RefMatters::no,ConstMatters::no,TemporaryMatters::no,AllowSubstitute::yes,false,false);
            };
            for ( auto & d : destAliases ) if ( same(d.second.first, type) ) return true;
            for ( auto & t : typeAliases ) if ( same(t.second.first, destType) ) return true;
            return false;
        }
        bool initReads ( ExprMakeStruct * mks ) {
            if ( mks->makeType->baseType==Type::tHandle ) return false;
            if ( mks->makeType->baseType!=Type::tStructure || !mks->makeType->structType ) return false;
            for ( auto & fld : mks->makeType->structType->fields ) {
                if ( fld.init ) fld.init->visit(*this);
            }
            return found;
        }
        static bool pureValueCall ( Function * fn ) {
            if ( !fn ) return false;
            const uint32_t reads = uint32_t(SideEffects::userScenario) | uint32_t(SideEffects::accessExternal)
                | uint32_t(SideEffects::modifyArgument) | uint32_t(SideEffects::accessGlobal) | uint32_t(SideEffects::invoke);
            if ( fn->builtIn ? fn->sideEffectFlags!=0 : (!fn->knownSideEffects || (fn->sideEffectFlags & reads)) ) return false;
            for ( auto & arg : fn->arguments ) {
                if ( arg->type->ref || arg->type->isRefType() || arg->type->baseType==Type::tPointer ) return false;
            }
            return true;
        }
        static Variable * plainLocal ( Expression * dest ) {
            for ( ;; ) {
                if ( dest->rtti_isR2V() ) dest = static_cast<ExprRef2Value *>(dest)->subexpr;
                else if ( dest->rtti_isField() ) dest = static_cast<ExprField *>(dest)->value;
                else if ( dest->rtti_isSwizzle() ) dest = static_cast<ExprSwizzle *>(dest)->value;
                else if ( dest->rtti_isAt() ) {
                    auto at = static_cast<ExprAt *>(dest);
                    if ( !(at->subexpr->type->isFixedArray() || at->subexpr->type->isVectorType()) || !at->index->rtti_isConstant() ) return nullptr;
                    dest = at->subexpr;
                } else break;
            }
            if ( !dest->rtti_isVar() ) return nullptr;
            auto ev = static_cast<ExprVar *>(dest);
            auto var = ev->variable;
            if ( var->global || var->type->ref || var->type->baseType==Type::tPointer || ev->argument || ev->block ) return nullptr;
            return var;
        }
        TypeDecl *      destType;
        TypeAliasMap    destAliases;
        Variable *      local;
        bool            found = false;
    };

    bool mayReadDestination ( Expression * value, Expression * dest ) {
        return DestinationReads(dest).readBy(value);
    }

    // src/ast/ARCHITECTURE_ALIASING.md#in-place-build-aliases
    class AliasMarker : public Visitor {
    public:
        AliasMarker ( const ProgramPtr & prog, bool permanent, bool everything, bool report, TextWriter & ls ) : program(prog), logs(ls) {
            checkAliasing = programReport = report;
            logAliasing = program->options.getBoolOption("log_aliasing", false);
            isPermanent = permanent;
            isEverything = everything;
        }
    protected:
        ProgramPtr  program;
        das_hash_map<Expression *,Expression *> cmresDest;
        LoopSources loopSources;
        bool checkAliasing = false;
        bool programReport = false;
        bool logAliasing = false;
        TextWriter & logs;
        bool isPermanent = false;
        bool isEverything = false;
    protected:
        virtual bool canVisitStructureFieldInit ( Structure * ) override { return false; }
        virtual bool canVisitArgumentInit ( Function * , const VariablePtr &, Expression * ) override { return false; }
        virtual bool canVisitQuoteSubexpression ( ExprQuote * ) override { return false; }
        virtual bool canVisitGlobalVariable ( Variable * var ) override {
            if ( var->aliasesResolved ) return false;
            if ( !program->isUsed(var) && !isEverything ) return false;
            var->aliasesResolved = isPermanent;
            return true;
        }
        virtual bool canVisitStructure ( Structure * st ) override {
            return !st->isTemplate;
        }
        virtual bool canVisitFunction ( Function * fun ) override {
            if ( fun->stub ) return false;
            if ( fun->isTemplate ) return false;
            if ( fun->aliasesResolved ) return false;
            if ( !program->isUsed(fun) && !isEverything ) return false;
            fun->aliasesResolved = isPermanent;
            return true;
        }
    protected:
        virtual void preVisit ( ExprCopy * expr ) override {
            Visitor::preVisit(expr);
            cmresDest[expr->right] = expr->left;
        }
        virtual void preVisit ( ExprMove * expr ) override {
            Visitor::preVisit(expr);
            cmresDest[expr->right] = expr->left;
        }
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            for ( size_t i=0, is=das::min(expr->iteratorVariables.size(), expr->sources.size()); i!=is; ++i ) {
                loopSources[expr->iteratorVariables[i]] = expr->sources[i];
            }
        }
        virtual void preVisit ( Function * fn ) override {
            Visitor::preVisit(fn);
            auto origin = fn->getOriginPtr();
            checkAliasing = programReport && !(origin && origin->module && origin->module->aliasingAllowed);
        }
        virtual FunctionPtr visit ( Function * fn ) override {
            cmresDest.clear();
            loopSources.clear();
            return Visitor::visit(fn);
        }
    // make local
        void preVisitMakeLocal ( ExprMakeLocal * expr ) {
            auto it = cmresDest.find(expr);
            if ( it==cmresDest.end() ) return;
            if ( !mayReadDestination(expr, it->second) ) return;
            if ( checkAliasing ) {
                program->error("[[" + string(expr->__rtti) + " ]] aliases",
                    "the value reads memory which can be its destination: a global, a reference, a pointer or a call", "",
                        expr->at, CompilationError::invalid_expression);
            } else {
                expr->alwaysAlias = true;
            }
        }
        virtual void preVisit ( ExprMakeArray * expr ) override {
            Visitor::preVisit(expr);
            preVisitMakeLocal(expr);
        }
        virtual void preVisit ( ExprMakeStruct * expr ) override {
            Visitor::preVisit(expr);
            preVisitMakeLocal(expr);
        }
        virtual void preVisit ( ExprMakeTuple * expr ) override {
            Visitor::preVisit(expr);
            preVisitMakeLocal(expr);
        }
        virtual void preVisit ( ExprMakeVariant * expr ) override {
            Visitor::preVisit(expr);
            preVisitMakeLocal(expr);
        }
    // invoke
        virtual void preVisit ( ExprInvoke * expr ) override {
            Visitor::preVisit(expr);
            auto it = cmresDest.find(expr);
            if ( it!=cmresDest.end() ) {
                auto argT = expr->arguments[0]->type;
                if ( argT->isGoodBlockType() ) {
                    if ( mayReadDestination(expr, it->second) ) {
                        expr->cmresAlias = true;
                    }
                    return;
                }
                if ( !(argT->firstType->isRefType() && !argT->firstType->ref) ) {
                    return; // not a CMRES
                }
                // everything can alias on the right
                auto resOut = it->second;
                ExpressionSources resSrc;
                if ( collectSources(program, loopSources, resOut, resSrc, false, true) ) {
                    if ( checkAliasing ) {
                        program->error("invoke result always aliases",
                            "some form of ... *ptr ... = invoke( ... ) where we don't know where pointer came from", "",
                                expr->at, CompilationError::invalid_argument);
                    } else {
                        expr->cmresAlias = true;
                        return;
                    }
                }
                // check if there are any globals
                bool anyGlobals = false;
                for ( auto v : resSrc ) {
                    if ( v->global ) {
                        anyGlobals = true;
                        break;
                    }
                }
                // invoke arguments can only alias invoke of function or lambda
                for ( size_t ai=0, ais=expr->arguments.size(); ai!=ais; ++ai ) {
                    if ( !(expr->arguments[ai]->type->isRef() || expr->arguments[ai]->type->baseType==Type::tPointer) ) {
                        continue;
                    }
                    ExpressionSources argSrc;
                    if ( collectSources(program, loopSources, expr->arguments[ai], argSrc, false, anyGlobals) ) {
                        if ( checkAliasing ) {
                            program->error("invoke result aliases argument " + to_string(ai),
                                "some form of ... = invoke( ... *ptr ... ) where we don't know where pointer came from", "",
                                    expr->at, CompilationError::invalid_argument);
                        } else {
                            expr->cmresAlias = true;
                            return;
                        }
                    } else if ( auto aliasVar = intersectSources ( resSrc, argSrc ) ) {
                        if ( checkAliasing ) {
                            program->error("invoke result aliases argument " + to_string(ai),
                                "some form of ... " + aliasVar->name + " ... = invoke( ... " + aliasVar->name + " ... )", "",
                                    expr->at, CompilationError::invalid_argument);
                        } else {
                            expr->cmresAlias = true;
                            return;
                        }
                    }
                }
                // we never get invoke(@@....,....) because its converted into a call
                // we can analyze functions by signature, and collect all global variables referenced by matching functions
                if ( anyGlobals && program->thisModule->isModule ) {
                    if ( checkAliasing ) {
                        program->error("invoke result potentially aliases a global through a function of a module that requires this one",
                            "some form of ... global ... = invoke( ... ) in a module", "",
                                expr->at, CompilationError::invalid_argument_global);
                    } else {
                        expr->cmresAlias = true;
                    }
                    return;
                }
                if ( anyGlobals) {
                    IndirectSources globSrc;
                    if ( argT->isGoodFunctionType() ) {
                        collectInvokeIndVariables(program, expr->arguments[0]->type, globSrc);
                    } else {
                        collectInvokeIndLambdaVariables(program, expr->arguments[0]->type, globSrc);
                    }
                    for ( auto gIt : globSrc ) {
                        if ( resSrc.find(gIt.first)!=resSrc.end() ) {
                            if ( checkAliasing ) {
                                if ( argT->isGoodFunctionType() ) {
                                    program->error("invoke result potentially aliases global " + gIt.first->name + " through function " + gIt.second->getMangledName(),
                                        "some form of ... " + gIt.first->name + " ... = invoke( ... " + gIt.first->name + " ... )", "",
                                            expr->at, CompilationError::invalid_argument_global);
                                } else {
                                    program->error("invoke result potentially aliases global " + gIt.first->name + " through lambda at " + gIt.second->at.describe(),
                                        "some form of ... " + gIt.first->name + " ... = invoke( ... " + gIt.first->name + " ... )", "",
                                            expr->at, CompilationError::invalid_argument_global);
                                }
                            } else {
                                expr->cmresAlias = true;
                                goto bailout;
                            }
                        }
                    }
                }
                bailout:;
            }
        }
    // call
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->func ) {
                program->error("call '" + expr->name + "' was not resolved during type inference", "", "",
                    expr->at, CompilationError::internal_expression);
                return;
            }
            if ( expr->func->aliasCMRES ) {
                expr->cmresAlias = true;
                goto bailout;
            }
            if ( !expr->func->neverAliasCMRES && (expr->func->resultAliases.size() || expr->func->resultAliasesGlobals.size()) ) {
                auto it = cmresDest.find(expr);
                if ( it!=cmresDest.end() ) {
                    auto resOut = it->second;
                    ExpressionSources resSrc;
                    if ( collectSources(program, loopSources, resOut, resSrc, false, true) ) {
                        if ( checkAliasing ) {
                            program->error("function " + expr->func->describeName() + " result always aliases",
                                "some form of ... *ptr ... = " + expr->func->name + "( ... ) where we don't know where pointer came from", "",
                                    expr->at, CompilationError::invalid_argument);
                        } else {
                            expr->cmresAlias = true;
                            goto bailout;
                        }
                    }
                    // check if there are any globals
                    bool anyGlobals = false;
                    for ( auto v : resSrc ) {
                        if ( v->global ) {
                            anyGlobals = true;
                            break;
                        }
                    }
                    // now go thorough all arguments, which are potential aliases
                    for ( auto ai : expr->func->resultAliases ) {
                        ExpressionSources argSrc;
                        if ( collectSources(program, loopSources, expr->arguments[ai], argSrc, false, anyGlobals) ) {
                            if ( checkAliasing ) {
                                program->error("function " + expr->func->describeName() + " result aliases argument " + expr->func->arguments[ai]->name,
                                    "some form of ... = " + expr->func->name + "( ... *ptr ... ) where we don't know where pointer came from", "",
                                        expr->at, CompilationError::invalid_argument);
                            } else {
                                expr->cmresAlias = true;
                                goto bailout;
                            }
                        } else if ( auto aliasVar = intersectSources ( resSrc, argSrc ) ) {
                            if ( checkAliasing ) {
                                program->error("function " + expr->func->describeName() + " result aliases argument " + expr->func->arguments[ai]->name,
                                    "some form of ... " + aliasVar->name + " ... = " + expr->func->name + "( ... " + aliasVar->name + " ... )", "",
                                        expr->at, CompilationError::invalid_argument);
                            } else {
                                expr->cmresAlias = true;
                                goto bailout;
                            }
                        }
                    }
                    for ( auto & vit : expr->func->resultAliasesGlobals) {
                        ExpressionSources argSrc;
                        argSrc.insert(vit.var);
                        if ( vit.viaPointer ) {
                            if ( checkAliasing ) {
                                program->error("function " + expr->func->describeName() + " result aliases global variable " + vit.var->getMangledName() + " through function " + vit.func->getMangledName(),
                                    "some form of ... = " + expr->func->name + "(...) where we don't know where the pointer in " + vit.var->name + " came from", "",
                                        expr->at, CompilationError::invalid_argument_global);
                            } else {
                                expr->cmresAlias = true;
                                goto bailout;
                            }
                        } else if ( auto aliasVar = intersectSources ( resSrc, argSrc ) ) {
                            if ( checkAliasing ) {
                                string whereMessage;
                                if ( vit.func == expr->func ) {
                                    whereMessage = expr->func->name + "() uses " + vit.var->name;
                                } else {
                                    whereMessage = expr->func->name + "() indirectly calls " + vit.func->name + "() which uses " + vit.var->name;
                                }
                                program->error("function " + expr->func->describeName() + " result aliases global variable " + vit.var->getMangledName() + " through function " + vit.func->getMangledName(),
                                    "some form of ... " + aliasVar->name + " ... = " + expr->func->name + "(...) where " + whereMessage, "",
                                        expr->at, CompilationError::invalid_argument_global);
                            } else {
                                expr->cmresAlias = true;
                                goto bailout;
                            }
                        }
                    }
                }
            }
        bailout:;
            if ( logAliasing && expr->cmresAlias ) {
                logs << expr->at.describe() << ": " << expr->func->describeName() << " aliases with CMRES, stack optimization disabled\n";
            }
        }
    };

    static void deriveFunctionAliasing ( Program * program, TextWriter & logs, bool logAliasing ) {
        if ( logAliasing ) logs << "module " << program->thisModule->name << ":\n";
        program->thisModule->functions.foreach([&](const FunctionPtr & func){
            if ( func->builtIn || func->isTemplate ) return;
            deriveAliasing(func, logs, logAliasing);
        });
    }

    void Program::deriveAliases(TextWriter & logs, bool permanent, bool everything) {
        bool logAliasing = options.getBoolOption("log_aliasing", false);
        if ( logAliasing ) logs << "ALIASING:\n";
        deriveFunctionAliasing(this, logs, logAliasing);
        bool report = !aliasesChecked && options.getBoolOption("no_aliasing", policies.no_aliasing);
        AliasMarker marker(this, permanent, everything, report, logs);
        visit(marker);
        if ( logAliasing ) logs << "\n";
    }

    void Program::checkAliases(TextWriter & logs) {
        aliasesChecked = true;
        auto savedFunctions = functionStates;
        auto savedVariables = variableStates;
        if ( thisModule->isModule || policies.export_all ) {
            markModuleSymbolUse();
        } else {
            markExecutableSymbolUse();
        }
        deriveFunctionAliasing(this, logs, false);
        AliasMarker marker(this, false, false, true, logs);
        visit(marker);
        functionStates = das::move(savedFunctions);
        variableStates = das::move(savedVariables);
    }
}
