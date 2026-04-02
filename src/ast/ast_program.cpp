#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    // program

    vector<ReaderMacroPtr> Program::getReaderMacro ( const string & name ) const {
        vector<ReaderMacroPtr> macros;
        string moduleName, markupName;
        splitTypeName(name, moduleName, markupName);
        auto tmod = thisModule.get();
        auto modMacro = [&](Module * mod) -> bool {
            if ( thisModule->isVisibleDirectly(mod) && mod!=tmod ) {
                auto it = mod->readMacros.find(markupName);
                if ( it != mod->readMacros.end() ) {
                    macros.push_back(it->second);
                }
            }
            return true;
        };
        library.foreach(modMacro, moduleName);
        return macros;
    }

    int Program::getContextStackSize() const {
        return options.getIntOption("stack", policies.stack);
    }

    vector<EnumerationPtr> Program::findEnum ( const string & name ) const {
        return library.findEnum(name,thisModule.get());
    }

    vector<TypeDeclPtr> Program::findAlias ( const string & name ) const {
        return library.findAlias(name,thisModule.get());
    }

    vector<AnnotationPtr> Program::findAnnotation ( const string & name ) const {
        return library.findAnnotation(name,thisModule.get());
    }

    vector<TypeInfoMacroPtr> Program::findTypeInfoMacro ( const string & name ) const {
        return library.findTypeInfoMacro(name,thisModule.get());
    }

    vector<StructurePtr> Program::findStructure ( const string & name ) const {
        return library.findStructure(name,thisModule.get());
    }

    void Program::error ( const string & str, const string & extra, const string & fixme, const LineInfo & at, CompilationError cerr ) {
        errors.emplace_back(str,extra,fixme,at,cerr);
        failToCompile = true;
    }

    void Program::linkError ( const string & str, const string & extra ) {
        aotErrors.emplace_back(str,extra,"",LineInfo(), CompilationError::missing_aot);
        if ( policies.fail_on_no_aot ) {
            failToCompile = true;
            errors.emplace_back("AOT link failed on " + str,extra,"",LineInfo(), CompilationError::missing_aot);
        }
    }

    Module * Program::addModule ( const string & name ) {
        if ( auto lm = library.findModule(name) ) {
            return lm;
        }
        if ( auto pm = Module::require(name) ) {
            library.addModule(pm);
            return pm;
        }
        return nullptr;
    }

    bool Program::addAlias ( const TypeDeclPtr & at ) {
        return thisModule->addAlias(at, true);
    }

    bool Program::addVariable ( const VariablePtr & var ) {
        return thisModule->addVariable(var, true);
    }

    bool Program::addStructure ( const StructurePtr & st ) {
        return thisModule->addStructure(st, true);
    }

    bool Program::addEnumeration ( const EnumerationPtr & st ) {
        return thisModule->addEnumeration(st, true);
    }

    FunctionPtr Program::findFunction(const string & mangledName) const {
        return thisModule->findFunction(mangledName);
    }

    bool Program::addFunction ( const FunctionPtr & fn ) {
        return thisModule->addFunction(fn, true);
    }

    bool Program::addGeneric ( const FunctionPtr & fn ) {
        return thisModule->addGeneric(fn, true);
    }

    bool Program::addStructureHandle ( const StructurePtr & st, const TypeAnnotationPtr & ann, const AnnotationArgumentList & arg ) {
        if ( ann->rtti_isStructureTypeAnnotation() ) {
            auto annotation = static_pointer_cast<StructureTypeAnnotation>(ann->clone());
            annotation->name = st->name;
            string err;
            if ( annotation->create(st,arg,err) ) {
                return thisModule->addAnnotation(annotation, true);
            } else {
                error("can't create structure handle "+ann->name,err,"",st->at,CompilationError::invalid_annotation);
                return false;
            }
        } else {
            error("not a structure annotation "+ann->name,"","",st->at,CompilationError::invalid_annotation);
            return false;
        }
    }

    Program::Program() {
        thisModule = make_unique<ModuleDas>();
        library.addBuiltInModule();
        library.addModule(thisModule.get());
    }

    TypeDecl * Program::makeTypeDeclaration(const LineInfo &at, const string &name) {
        das::vector<das::StructurePtr> structs;
        das::vector<das::AnnotationPtr> handles;
        das::vector<das::EnumerationPtr> enums;
        das::vector<das::TypeDeclPtr> aliases;
        library.findWithCallback(name, thisModule.get(), [&](Module * pm, const string &name, Module * inWhichModule) {
            library.findStructure(structs, pm, name, inWhichModule);
            library.findAnnotation(handles, pm, name, inWhichModule);
            library.findEnum(enums, pm, name, inWhichModule);
            library.findAlias(aliases, pm, name, inWhichModule);
        });

        if ( ((structs.size()!=0)+(handles.size()!=0)+(enums.size()!=0)+(aliases.size()!=0)) > 1 ) {
            string candidates = describeCandidates(structs);
            candidates += describeCandidates(handles, false);
            candidates += describeCandidates(enums, false);
            candidates += describeCandidates(aliases, false);
            error("undefined make type declaration type "+name,candidates,"",
                at,CompilationError::type_not_found);
            return nullptr;
        } else if ( structs.size() ) {
            if ( structs.size()==1 ) {
                auto pTD = new TypeDecl(structs.back());
                pTD->at = at;
                return pTD;
            } else {
                string candidates = describeCandidates(structs);
                error("too many options for "+name,candidates,"",
                    at,CompilationError::structure_not_found);
                return nullptr;
            }
        } else if ( handles.size() ) {
            if ( handles.size()==1 ) {
                if ( handles.back()->rtti_isHandledTypeAnnotation() ) {
                    auto pTD = new TypeDecl(Type::tHandle);
                    pTD->annotation = static_cast<TypeAnnotation *>(handles.back().get());
                    pTD->at = at;
                    return pTD;
                } else {
                    error("not a handled type annotation "+name,"","",
                        at,CompilationError::handle_not_found);
                    return nullptr;
                }
            } else {
                string candidates = describeCandidates(handles);
                error("too many options for "+name, candidates, "",
                    at,CompilationError::handle_not_found);
                return nullptr;
            }
        } else if ( enums.size() ) {
            if ( enums.size()==1 ) {
                auto pTD = new TypeDecl(enums.back());
                pTD->enumType = enums.back().get();
                pTD->at = at;
                return pTD;
            } else {
                string candidates = describeCandidates(enums);
                error("too many options for "+name,candidates,"",
                    at,CompilationError::enumeration_not_found);
                return nullptr;
            }
        } else if ( aliases.size() ) {
            if ( aliases.size()==1 ) {
                auto pTD = new TypeDecl(*aliases.back());
                pTD->at = at;
                return pTD;
            } else {
                string candidates = describeCandidates(aliases);
                error("too many options for "+name,candidates,"",
                    at,CompilationError::type_alias_not_found);
                return nullptr;
            }
        } else {
            auto tt = new TypeDecl(Type::alias);
            tt->alias = name;
            tt->at = at;
            return tt;
        }
    }

    ExprLooksLikeCall * Program::makeCall ( const LineInfo & at, const string & name ) {
        vector<ExprCallFactory *> ptr;
        string moduleName, funcName;
        splitTypeName(name, moduleName, funcName);
        if ( moduleName != "_" && moduleName != "__" ) {    // those are never found. in reality we may want to support this one day with "*" and "thisModuleName" accordingly
            library.foreach([&](Module * pm) -> bool {
                if ( auto pp = pm->findCall(funcName) ) {
                    ptr.push_back(pp);
                }
                return true;
            }, moduleName);
        }
        if ( ptr.size()==1 ) {
            return (*ptr.back())(at);
        } else if ( ptr.size()==0 ) {
            return new ExprCall(at,name);
        } else {
            error("too many options for " + name,"","", at, CompilationError::function_not_found);
            return new ExprCall(at,name);
        }
    }

    ExprLooksLikeCall * Program::makeCall ( const LineInfo & at, const LineInfo & atEnd, const string & name ) {
        auto res = makeCall(at, name);
        if ( res ) {
            res->atEnclosure = at;
            res->atEnclosure.last_column = atEnd.last_column;
            res->atEnclosure.last_line = atEnd.last_line;
        }
        return res;
    }

    ExpressionPtr Program::makeConst ( const LineInfo & at, const TypeDeclPtr & type, vec4f value ) {
        if ( type->dim.size() || type->ref ) return nullptr;
        switch ( type->baseType ) {
            case Type::tBool:           return make_smart<ExprConstBool>(at, cast<bool>::to(value));
            case Type::tInt8:           return make_smart<ExprConstInt8>(at, cast<int8_t>::to(value));
            case Type::tInt16:          return make_smart<ExprConstInt16>(at, cast<int16_t>::to(value));
            case Type::tInt64:          return make_smart<ExprConstInt64>(at, cast<int64_t>::to(value));
            case Type::tInt:            return make_smart<ExprConstInt>(at, cast<int32_t>::to(value));
            case Type::tInt2:           return make_smart<ExprConstInt2>(at, cast<int2>::to(value));
            case Type::tInt3:           return make_smart<ExprConstInt3>(at, cast<int3>::to(value));
            case Type::tInt4:           return make_smart<ExprConstInt4>(at, cast<int4>::to(value));
            case Type::tUInt8:          return make_smart<ExprConstUInt8>(at, cast<uint8_t>::to(value));
            case Type::tUInt16:         return make_smart<ExprConstUInt16>(at, cast<uint16_t>::to(value));
            case Type::tUInt64:         return make_smart<ExprConstUInt64>(at, cast<uint64_t>::to(value));
            case Type::tUInt:           return make_smart<ExprConstUInt>(at, cast<uint32_t>::to(value));
            case Type::tBitfield:       return make_smart<ExprConstBitfield>(at, cast<uint32_t>::to(value));
            case Type::tBitfield8:      {
                auto res = make_smart<ExprConstBitfield>(at, cast<uint8_t>::to(value));
                res->baseType = Type::tBitfield8;
                return res;
            }
            case Type::tBitfield16:     {
                auto res = make_smart<ExprConstBitfield>(at, cast<uint16_t>::to(value));
                res->baseType = Type::tBitfield16;
                return res;
            }
            case Type::tBitfield64:     {
                auto res = make_smart<ExprConstBitfield>(at, cast<uint64_t>::to(value));
                res->baseType = Type::tBitfield64;
                return res;
            }
            case Type::tUInt2:          return make_smart<ExprConstUInt2>(at, cast<uint2>::to(value));
            case Type::tUInt3:          return make_smart<ExprConstUInt3>(at, cast<uint3>::to(value));
            case Type::tUInt4:          return make_smart<ExprConstUInt4>(at, cast<uint4>::to(value));
            case Type::tFloat:          return make_smart<ExprConstFloat>(at, cast<float>::to(value));
            case Type::tFloat2:         return make_smart<ExprConstFloat2>(at, cast<float2>::to(value));
            case Type::tFloat3:         return make_smart<ExprConstFloat3>(at, cast<float3>::to(value));
            case Type::tFloat4:         return make_smart<ExprConstFloat4>(at, cast<float4>::to(value));
            case Type::tDouble:         return make_smart<ExprConstDouble>(at, cast<double>::to(value));
            case Type::tRange:          return make_smart<ExprConstRange>(at, cast<range>::to(value));
            case Type::tURange:         return make_smart<ExprConstURange>(at, cast<urange>::to(value));
            case Type::tRange64:        return make_smart<ExprConstRange64>(at, cast<range64>::to(value));
            case Type::tURange64:       return make_smart<ExprConstURange64>(at, cast<urange64>::to(value));
            default:                    DAS_ASSERTF(0, "we should not even be here"); return nullptr;
        }
    }

    StructurePtr Program::visitStructure(Visitor & vis, Structure * pst) {
        vis.preVisit(pst);
        pst->aliases.foreach([&](auto & alsv){
            vis.preVisitStructureAlias(pst, alsv->alias, alsv.get());
            vis.preVisit(alsv.get());
            auto alssv = alsv->visit(vis);
            if ( alssv ) alssv = vis.visit(alssv.get());
            if ( alssv ) alssv = vis.visitStructureAlias(pst, alssv->alias, alssv.get());
            if ( alssv!=alsv ) {
                pst->aliases.replace(alsv->alias, alssv);
                alsv = alssv;
            }
        });
        for ( auto & fi : pst->fields ) {
            vis.preVisitStructureField(pst, fi, &fi==&pst->fields.back());
            if ( fi.type ) {
                vis.preVisit(fi.type.get());
                fi.type = fi.type->visit(vis);
                fi.type = vis.visit(fi.type.get());
            }
            if ( fi.init && vis.canVisitStructureFieldInit(pst) ) {
                fi.init = fi.init->visit(vis);
            }
            vis.visitStructureField(pst, fi, &fi==&pst->fields.back());
        }
        return vis.visit(pst);
    }

    EnumerationPtr Program::visitEnumeration(Visitor & vis, Enumeration * penum) {
        vis.preVisit(penum);
        size_t count = 0;
        size_t total = penum->list.size();
        for ( auto & itf : penum->list ) {
            vis.preVisitEnumerationValue(penum, itf.name, itf.value.get(), count==total);
            if ( itf.value ) itf.value = itf.value->visit(vis);
            itf.value = vis.visitEnumerationValue(penum, itf.name, itf.value.get(), count==total);
            count ++;
        }
        return vis.visit(penum);
    }

    void Program::visitModules(Visitor & vis, bool visitGenerics) {
        vis.preVisitProgram(this);
        library.foreach([&](Module * pm) -> bool {
            visitModule(vis, pm, visitGenerics);
            return true;
        }, "*");
        vis.visitProgram(this);
    }

    void Program::visitModulesInOrder(Visitor & vis, bool visitGenerics) {
        vis.preVisitProgram(this);
        library.foreach_in_order([&](Module * pm) -> bool {
            visitModule(vis, pm, visitGenerics);
            return true;
        }, thisModule.get());
        vis.visitProgram(this);
    }

    void Program::visit(Visitor & vis, bool visitGenerics, bool sortStructures ) {
        vis.preVisitProgram(this);
        visitModule(vis, thisModule.get(), visitGenerics, sortStructures);
        vis.visitProgram(this);
    }

    static void collectStructDeps ( const TypeDeclPtr & type, Structure * owner, das_hash_set<Structure *> & deps ) {
        if ( !type ) return;
        if ( type->baseType == Type::tStructure && type->structType && type->structType != owner ) {
            if ( type->isPointer() ) return;   // pointers don't need full definition
            if ( !deps.insert(type->structType).second ) return;   // already visited
            // recurse into the struct's own fields
            for ( auto & field : type->structType->fields ) {
                collectStructDeps(field.type, owner, deps);
            }
        }
        // recurse into firstType / secondType for containers
        if ( type->firstType ) collectStructDeps(type->firstType, owner, deps);
        if ( type->secondType ) collectStructDeps(type->secondType, owner, deps);
        // recurse into argTypes (e.g. tuple, variant element types)
        for ( auto & argType : type->argTypes ) {
            collectStructDeps(argType, owner, deps);
        }
    }

    static void topoSortStructures ( vector<StructurePtr> & structs ) {
        if ( structs.size() <= 1 ) return;
        // build adjacency: struct -> set of structs it depends on (by value)
        das_hash_map<Structure *, das_hash_set<Structure *>> deps;
        das_hash_set<Structure *> allSet;
        for ( auto & sp : structs ) {
            allSet.insert(sp.get());
        }
        for ( auto & sp : structs ) {
            auto & d = deps[sp.get()];
            for ( auto & field : sp->fields ) {
                collectStructDeps(field.type, sp.get(), d);
            }
            // only keep deps that are in our set
            das_hash_set<Structure *> filtered;
            for ( auto dep : d ) {
                if ( allSet.count(dep) ) filtered.insert(dep);
            }
            d = das::move(filtered);
        }
        // Kahn's algorithm using vector as queue
        das_hash_map<Structure *, int> inDegree;
        for ( auto & [s, dd] : deps ) {
            inDegree[s] = (int)dd.size();
        }
        vector<Structure *> sorted;
        sorted.reserve(structs.size());
        // seed with zero-dependency structs
        for ( auto & sp : structs ) {
            if ( inDegree[sp.get()] == 0 ) sorted.push_back(sp.get());
        }
        // process in FIFO order
        for ( size_t qi = 0; qi < sorted.size(); qi++ ) {
            auto s = sorted[qi];
            for ( auto & [other, dd] : deps ) {
                if ( dd.erase(s) ) {
                    inDegree[other]--;
                    if ( inDegree[other] == 0 ) sorted.push_back(other);
                }
            }
        }
        if ( sorted.size() != structs.size() ) return; // cycle - keep original order
        // reorder structs to match sorted order
        das_hash_map<Structure *, StructurePtr> byPtr;
        for ( auto & sp : structs ) byPtr[sp.get()] = sp;
        for ( size_t i = 0; i < sorted.size(); i++ ) {
            structs[i] = byPtr[sorted[i]];
        }
    }

    void Program::visitModule(Visitor & vis, Module * thatModule, bool visitGenerics, bool sortStructures) {
        vis.preVisitModule(thatModule);
        // enumerations
        thatModule->enumerations.foreach([&](auto & penum){
            if ( vis.canVisitEnumeration(penum.get()) ) {
                auto penumn = visitEnumeration(vis, penum.get());
                if ( penumn != penum ) {
                    thatModule->enumerations.replace(penum->name, penumn);
                    penum = penumn;
                }
            }
        });
        // structures
        if ( sortStructures ) {
            // collect, topologically sort, then visit
            vector<StructurePtr> allStructs;
            thatModule->structures.foreach([&](auto & spst){
                if ( vis.canVisitStructure(spst.get()) ) {
                    allStructs.push_back(spst);
                }
            });
            topoSortStructures(allStructs);
            for ( auto & spst : allStructs ) {
                Structure * pst = spst.get();
                StructurePtr pstn = visitStructure(vis, pst);
                if ( pstn.get() != pst ) {
                    thatModule->structures.replace(pst->name, pstn);
                }
            }
        } else {
            thatModule->structures.foreach([&](auto & spst){
                Structure * pst = spst.get();
                if ( vis.canVisitStructure(pst) ) {
                    StructurePtr pstn = visitStructure(vis, pst);
                    if ( pstn.get() != pst ) {
                        thatModule->structures.replace(pst->name, pstn);
                        spst = pstn;
                    }
                }
            });
        }
        // aliases
        thatModule->aliasTypes.foreach([&](auto & alsv){
            vis.preVisitAlias(alsv.get(), alsv->alias);
            vis.preVisit(alsv.get());
            auto alssv = alsv->visit(vis);
            if ( alssv ) alssv = vis.visit(alssv.get());
            if ( alssv ) alssv = vis.visitAlias(alssv.get(), alssv->alias);
            if ( alssv!=alsv ) {
                thatModule->aliasTypes.replace(alssv->alias, alssv);
                alsv = alssv;
            }
        });
        // real things
        vis.preVisitProgramBody(this,thatModule);
        // globals
        vis.preVisitGlobalLetBody(this);
        thatModule->globals.foreach([&](auto & var){
            if ( vis.canVisitGlobalVariable(var.get()) ) {
                vis.preVisitGlobalLet(var);
                if ( var->type ) {
                    vis.preVisit(var->type.get());
                    var->type = var->type->visit(vis);
                    var->type = vis.visit(var->type.get());
                }
                if ( var->init ) {
                    vis.preVisitGlobalLetInit(var, var->init.get());
                    var->init = var->init->visit(vis);
                    var->init = vis.visitGlobalLetInit(var, var->init.get());
                }
                auto varn = vis.visitGlobalLet(var);
                if ( varn!=var ) {
                    thatModule->globals.replace(var->name, varn);
                    var = varn;
                }
            }
        });
        vis.visitGlobalLetBody(this);
        // generics
        if ( visitGenerics ) {
            thatModule->generics.foreach([&](auto & fn){
                if ( !fn->builtIn ) {
                    auto nfn = fn->visit(vis);
                    if ( fn != nfn ) {
                        thatModule->generics.replace(fn->getMangledName(), nfn);
                        fn = nfn;
                        DAS_ASSERTF(false,"todo: take care of genericsByName?");
                    }
                }
            });
        }
        // functions
        thatModule->functions.foreach([&](auto & fn){
            if ( !fn->builtIn ) {
                if ( vis.canVisitFunction(fn.get()) ) {
                    auto nfn = fn->visit(vis);
                    if ( fn != nfn ) {
                        thatModule->functions.replace(fn->getMangledName(), nfn);
                        fn = nfn;
                        DAS_ASSERTF(false,"todo: take care of functionsByName?");
                    }
                }
            }
        });
        vis.visitModule(thatModule);
    }

    bool Program::getOptimize() const {
        if ( policies.no_optimizations ) return false;
        auto arg = options.find("optimize",Type::tBool);
        if ( arg ) return arg->bValue;
        arg = options.find("no_optimization",Type::tBool);
        if ( arg ) return !arg->bValue;
        return true;
    }

    bool Program::getDebugger() const {
        return policies.debugger || options.getBoolOption("debugger",false);
    }

    bool Program::getProfiler() const {
        return policies.profiler || options.getBoolOption("profiler",false);
    }

    void Program::optimize(TextWriter & logs, ModuleGroup & libGroup) {
        bool logOpt = options.getBoolOption("log_optimization",false);
        bool logPass = options.getBoolOption("log_optimization_passes",false);
        bool log = logOpt || logPass;
        bool any, last;
        int optimizationRound = 1;
        if (log) {
            logs << *this << "\n";
        }
        do {
            if ( log ) logs << "OPTIMIZE " << optimizationRound << ":\n"; if ( logPass ) logs << *this;
            any = false;
            last = optimizationRefFolding(optimizationRound);    if ( failed() ) break;  any |= last;
            if ( log ) logs << "REF FOLDING: " << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            last = optimizationUnused(logs, optimizationRound);    if ( failed() ) break;  any |= last;
            if ( log ) logs << "REMOVE UNUSED:" << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            last = optimizationConstFolding(optimizationRound);  if ( failed() ) break;  any |= last;
            if ( log ) logs << "CONST FOLDING:" << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            last = optimizationCondFolding(optimizationRound);  if ( failed() ) break;  any |= last;
            if ( log ) logs << "COND FOLDING:" << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            last = optimizationBlockFolding(optimizationRound);  if ( failed() ) break;  any |= last;
            if ( log ) logs << "BLOCK FOLDING:" << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            // this is here again for a reason
            last = optimizationUnused(logs, optimizationRound);    if ( failed() ) break;  any |= last;
            if ( log ) logs << "REMOVE UNUSED:" << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            // now, user macros
            last = false;
            auto modMacro = [&](Module * mod) -> bool {    // we run all macros for each module
                if ( thisModule->isVisibleDirectly(mod) && mod!=thisModule.get() ) {
                    for ( const auto & pm : mod->optimizationMacros ) {
                        last |= pm->apply(this, thisModule.get());
                        if ( failed() ) {                       // if macro failed, we report it, and we are done
                            error("optimization macro " + mod->name + "::" + pm->name + " failed", "","",LineInfo());
                            return false;
                        }
                    }
                }
                return true;
            };
            Module::foreach(modMacro);
            if ( failed() ) break;
            any |= last;
            libGroup.foreach(modMacro,"*");
            if ( failed() ) break;
            any |= last;
            if ( log ) logs << "MACROS:" << (last ? "optimized" : "nothing") << "\n"; if ( logPass ) logs << *this;
            optimizationRound++;
        } while ( any );
    }

}
