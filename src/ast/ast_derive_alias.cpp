#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

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
        if ( !func->result->isRef() ) return;
        if ( func->arguments.size()==0 && func->useGlobalVariables.size()==0 && func->useFunctions.size()==0 ) return;
    // collect indirect global variables
        das_hash_map<Variable *,Function *> ind;
        das_hash_set<Function *> depInd;
        collectIndVariables(func.get(),func.get(),ind,depInd);
        if ( func->arguments.size()==0 && ind.size()==0 ) return;
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

    void Program::deriveAliases(TextWriter & logs) {
        bool logAliasing = options.getBoolOption("log_aliasing", false);
        if ( logAliasing ) logs << "ALIASING:\n";
        thisModule->functions.foreach([&](const FunctionPtr & func) {
            if ( func->builtIn || !func->used ) return;
            deriveAliasing(func, logs, logAliasing);
        });
        if ( logAliasing ) logs << "\n";
    }
}
