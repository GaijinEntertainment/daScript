#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    typedef das_hash_set<Variable *> ExpressionSources;
    class SourceCollector : public Visitor {
    public:
        SourceCollector() {
            enabled.push_back(true);
        }
    protected:
    // we found source
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( enabled.back() ) sources.insert(expr->variable.get());
        }
    // in op3 only sources can alias
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            enabled.push_back(false);
        }
        virtual void preVisitLeft  ( ExprOp3 * expr, Expression * left ) {
            Visitor::preVisitLeft(expr,left);
            enabled.back() = true;
        }
        virtual ExpressionPtr visit ( ExprOp3 * expr ) override {
            enabled.pop_back();
            return Visitor::visit(expr);
        }
    // pointer deref - all bets are off
        virtual void preVisit ( ExprPtr2Ref * expr ) override {
            Visitor::preVisit(expr);
            alwaysAliases = true;
        }
    // function call - does not alias when does not return ref
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->func->result->ref ) enabled.push_back(false);
        }
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            if ( !expr->func->result->ref ) enabled.push_back(true);
            return Visitor::visit(expr);
        }
    // invoke - all bets are off
        virtual void preVisit ( ExprInvoke * expr ) override {
            Visitor::preVisit(expr);
            alwaysAliases = true;
        }
    protected:
        vector<bool> enabled;
    public:
        ExpressionSources sources;
        bool alwaysAliases = false;
    };

    bool collectSources ( Expression * expr, ExpressionSources & src ) {
        SourceCollector srr;
        expr->visit(srr);
        src = move(srr.sources);
        return srr.alwaysAliases;
    }

    Variable * intersectSources ( ExpressionSources & a, ExpressionSources & b ) {
        for ( auto va : a ) {
            for ( auto vb : b ) {
                if ( va==vb ) {
                    return va;
                }
            }
        }
        return nullptr;
    }


    void collectIndVariables ( Function * at, Function * lookup, das_hash_map<Variable *,Function *>  & indGlobalVariables, das_hash_set<Function *> & accessed ) {
        if ( accessed.find(lookup)!=accessed.end() ) return;
        accessed.insert(lookup);
        for ( auto var : lookup->useGlobalVariables ) {
            if ( indGlobalVariables.find(var)==indGlobalVariables.end() ) {
                indGlobalVariables[var] = lookup;
            }
        }
        for ( auto fun : lookup->useFunctions ) {
            collectIndVariables(at, fun, indGlobalVariables, accessed);
        }
    }

    void deriveAliasing ( const FunctionPtr & func, TextWriter & logs, bool logAliasing ) {
        if ( func->arguments.size()==0 && func->useGlobalVariables.size()==0 && func->useFunctions.size()==0 ) return;
    // collect indirect global variables
        das_hash_map<Variable *,Function *> ind;
        das_hash_set<Function *> depInd;
        collectIndVariables(func.get(),func.get(),ind,depInd);
        if ( !(func->copyOnReturn || func->moveOnReturn) ) return;  // not a cmres function, we don't need cmres aliasing
        if ( func->arguments.size()==0 && ind.size()==0 ) return;   // no arguments, no globals, no cmres aliasing
    // collect type aliasing
        if ( logAliasing ) logs << "function " << func->getMangledName() << " returns by reference\n";
        das_set<Structure *> rdep;
        TypeAliasMap resTypeAliases;
        func->result->collectAliasing(resTypeAliases, rdep, false);
    // do arguments
        func->resultAliases.clear();
        for ( int i=0; i!=func->arguments.size(); ++i ) {
            if ( !(func->arguments[i]->type->isRef() || func->arguments[i]->type->baseType==Type::tPointer) ) {
                continue;
            }
            das_set<Structure *> dep;
            TypeAliasMap typeAliases;
            func->arguments[i]->type->collectAliasing(typeAliases, dep, false);
            for ( auto resT : resTypeAliases ) {
                for ( auto alias : typeAliases ) {
                    if ( alias.second.first->isSameType(*(resT.second.first),RefMatters::no,ConstMatters::no,TemporaryMatters::no,AllowSubstitute::yes,false,false) ) {
                        func->resultAliases.push_back(i);
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
        for ( auto & it : ind ) {
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

    class AliasMarker : public Visitor {
    public:
        AliasMarker ( const ProgramPtr & prog, TextWriter & ls ) : program(prog), logs(ls) {
            checkAliasing = program->options.getBoolOption("no_aliasing", program->policies.no_aliasing);
            logAliasing = program->options.getBoolOption("log_aliasing", false);
        }
    protected:
        ProgramPtr  program;
        das_hash_map<Expression *,Expression *> cmresDest;
        bool checkAliasing = false;
        bool logAliasing = false;
        TextWriter & logs;
    protected:
        virtual void preVisit ( ExprCopy * expr ) override {
            Visitor::preVisit(expr);
            cmresDest[expr->right.get()] = expr->left.get();
        }
        virtual void preVisit ( ExprMove * expr ) override {
            Visitor::preVisit(expr);
            cmresDest[expr->right.get()] = expr->left.get();
        }
        virtual FunctionPtr visit ( Function * fn ) override {
            cmresDest.clear();
            return Visitor::visit(fn);
        }
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->func->resultAliases.size() || expr->func->resultAliasesGlobals.size() ) {
                auto it = cmresDest.find(expr);
                if ( it!=cmresDest.end() ) {
                    auto resOut = it->second;
                    ExpressionSources resSrc;
                    if ( collectSources(resOut, resSrc) ) {
                        if ( checkAliasing ) {
                            program->error("function " + expr->func->describeName() + " result always aliases",
                                "some form of ... *ptr ... = " + expr->func->name + "( ... ) where we don't know where pointer came from", "",
                                    expr->at, CompilationError::argument_aliasing);
                        } else {
                            expr->cmresAlias = true;
                            goto bailout;
                        }
                    }
                    for ( auto ai : expr->func->resultAliases ) {
                        ExpressionSources argSrc;
                        if ( collectSources(expr->arguments[ai].get(), argSrc) ) {
                            if ( checkAliasing ) {
                                program->error("function " + expr->func->describeName() + " result aliases argument " + expr->func->arguments[ai]->name,
                                    "some form of ... = " + expr->func->name + "( ... *ptr ... ) where we don't know where pointer came from", "",
                                        expr->at, CompilationError::argument_aliasing);
                            } else {
                                expr->cmresAlias = true;
                                goto bailout;
                            }
                        } else if ( auto aliasVar = intersectSources ( resSrc, argSrc ) ) {
                            if ( checkAliasing ) {
                                program->error("function " + expr->func->describeName() + " result aliases argument " + expr->func->arguments[ai]->name,
                                    "some form of ... " + aliasVar->name + " ... = " + expr->func->name + "( ... " + aliasVar->name + " ... )", "",
                                        expr->at, CompilationError::argument_aliasing);
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
                                        expr->at, CompilationError::argument_aliasing);
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
                                        expr->at, CompilationError::argument_aliasing);
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

    void Program::deriveAliases(TextWriter & logs) {
        bool logAliasing = options.getBoolOption("log_aliasing", false);
        if ( logAliasing ) logs << "ALIASING:\n";
        thisModule->functions.foreach([&](const FunctionPtr & func) {
            if ( func->builtIn || !func->used ) return;
            deriveAliasing(func, logs, logAliasing);
        });
        AliasMarker marker(this, logs);
        visit(marker);
        if ( logAliasing ) logs << "\n";
    }
}
