#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    unsigned ModuleKarma = 0;

    bool splitTypeName ( const string & name, string & moduleName, string & funcName ) {
        auto at = name.find("::");
        if ( at!=string::npos ) {
            moduleName = name.substr(0,at);
            funcName = name.substr(at+2);
            if ( moduleName=="builtin`" ) moduleName="$";
            return true;
        } else {
            moduleName = "*";
            funcName = name;
            return false;
        }
    }

    // ANNOTATION

    const AnnotationArgument * AnnotationArgumentList::find ( const string & name, Type type ) const {
        auto it = find_if(begin(), end(), [&](const AnnotationArgument & arg){
            return (arg.name==name) && (type==Type::tVoid || type==arg.type);
        });
        return it==end() ? nullptr : &*it;
    }

    bool AnnotationArgumentList::getBoolOption(const string & name, bool def) const {
        auto arg = find(name, Type::tBool);
        return arg ? arg->bValue : def;
    }

    int32_t AnnotationArgumentList::getIntOption(const string & name, int32_t def) const {
        auto arg = find(name, Type::tInt);
        return arg ? arg->iValue : def;
    }

    // MODULE

    Module * Module::modules = nullptr;

    void Module::addDependency ( Module * mod, bool pub ) {
        requireModule[mod] |= pub;
        for ( auto it : mod->requireModule ) {
            if ( it.second ) {
                addDependency(it.first, false);
            }
        }
    }

    TypeAnnotation * Module::resolveAnnotation ( const TypeInfo * info ) {
        intptr_t ann = (intptr_t) (info->annotation_or_name);
        if ( ann & 1 ) {
            // convert module name from w-char to regular char
            wchar_t * wsname = (wchar_t *) ( ann & ~1 );
            char cvtbuf[256], *cvt;
            for (cvt = cvtbuf; *wsname; wsname++, cvt++) {
                DAS_ASSERT(cvt - cvtbuf < 255);
                *cvt = (char)*wsname;
            }
            *cvt = 0;
            // end convert
            string moduleName, annName;
            splitTypeName(cvtbuf, moduleName, annName);
            info->annotation_or_name = nullptr;
            for ( auto pm = Module::modules; pm!=nullptr; pm=pm->next ) {
                if ( pm->name == moduleName ) {
                    if ( auto annT = pm->findAnnotation(annName) ) {
                        info->annotation_or_name = (TypeAnnotation *) annT.get();
                    }
                    break;
                }
            }
        }
        return info->annotation_or_name;
    }

    void resetFusionEngine();

    void Module::Initialize() {
        bool all = false;
        while ( !all ) {
            all = true;
            for ( auto m = modules; m ; m = m->next ) {
                all &= m->initDependencies();
            }
        }
    }

    void Module::Shutdown() {
        ReuseGuard<TypeDecl> rguard;
        shutdownDebugAgent();
        auto m = modules;
        while ( m ) {
            auto pM = m;
            m = m->next;
            delete pM;
        }
        resetFusionEngine();
        ReuseAllocator<TypeDecl>::canHold = false;
    }

    void Module::foreach ( const callable<bool (Module * module)> & func ) {
        for (auto m = modules; m != nullptr; m = m->next) {
            if (!func(m)) break;
        }
    }

    Module * Module::require ( const string & name ) {
        for ( auto m = modules; m != nullptr; m = m->next ) {
            if ( m->name == name ) {
                return m;
            }
        }
        return nullptr;
    }

    Module * Module::requireEx ( const string & name, bool allowPromoted ) {
        for ( auto m = modules; m != nullptr; m = m->next ) {
            if ( allowPromoted || !m->promoted ) {
                if ( m->name == name ) {
                    return m;
                }
            }
        }
        return nullptr;
    }

    Type Module::findOption ( const string & name ) {
        Type optT = Type::none;
        for ( auto m = modules; m != nullptr; m = m->next ) {
            auto tt = m->getOptionType(name);
            if ( tt != Type::none ) {
                DAS_ASSERTF(optT==Type::none, "duplicate module option %s", name.c_str());
                optT = tt;
            }
        }
        return optT;
    }

    Module::Module ( const string & n ) : name(n) {
        if ( !name.empty() ) {
            next = modules;
            modules = this;
            builtIn = true;
        }
        if ( n != "$" ) {
            requireModule[require("$")] = false;
        } else {
            requireModule[this] = false;
        }
    }

    void Module::promoteToBuiltin(const FileAccessPtr & access) {
        DAS_ASSERTF(!builtIn, "failed to promote. already builtin");
        next = modules;
        modules = this;
        builtIn = true;
        promoted = true;
        promotedAccess = access;
    }

    Module::~Module() {
        if ( builtIn ) {
            Module ** p = &modules;
            for ( auto m = modules; m != nullptr; p = &m->next, m = m->next ) {
                if ( m == this ) {
                    *p = m->next;
                    return;
                }
            }
            DAS_ASSERTF(0, "failed to unlink. was builtIn field assigned after the fact?");
        }
    }

    bool Module::addAlias ( const TypeDeclPtr & at, bool canFail ) {
        if ( at->alias.empty() ) return false;
        if ( aliasTypes.insert(make_pair(at->alias, move(at))).second ) {
            at->module = this;
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate alias %s to module %s\n",at->alias.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::addAnnotation ( const AnnotationPtr & ptr, bool canFail ) {
        if ( handleTypes.insert(make_pair(ptr->name, move(ptr))).second ) {
            ptr->seal(this);
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate annotation %s to module %s\n", ptr->name.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::addTypeInfoMacro ( const TypeInfoMacroPtr & ptr, bool canFail ) {
        if ( typeInfoMacros.insert(make_pair(ptr->name, move(ptr))).second ) {
            ptr->seal(this);
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate typeinfo macro %s to module %s\n", ptr->name.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::addReaderMacro ( const ReaderMacroPtr & ptr, bool canFail ) {
        if ( readMacros.insert(make_pair(ptr->name, move(ptr))).second ) {
            ptr->seal(this);
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate reader macro %s to module %s\n", ptr->name.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }


    bool Module::addVariable ( const VariablePtr & var, bool canFail ) {
        if ( globals.insert(make_pair(var->name, var)).second ) {
            var->module = this;
            globalsInOrder.push_back(var);
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate variable %s to module %s\n", var->name.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::addEnumeration ( const EnumerationPtr & en, bool canFail ) {
        if ( enumerations.insert(make_pair(en->name, en)).second ) {
            en->module = this;
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate enumeration %s to module %s\n", en->name.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::removeStructure ( const StructurePtr & st ) {
        auto it = structures.find(st->name);
        if ( it!=structures.end() && it->second==st ) {
            structures.erase(it);
            auto itO = find ( structuresInOrder.begin(), structuresInOrder.end(), st );
            structuresInOrder.erase(itO);
            return true;
        } else {
            return false;
        }
    }

    bool Module::addStructure ( const StructurePtr & st, bool canFail ) {
        if ( structures.insert(make_pair(st->name, st)).second ) {
            st->module = this;
            structuresInOrder.push_back(st);
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate structure %s to module %s\n", st->name.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::addFunction ( const FunctionPtr & fn, bool canFail ) {
        fn->module = this;
        auto mangledName = fn->getMangledName();
        fn->module = nullptr;
        if ( fn->builtIn && fn->sideEffectFlags==uint32_t(SideEffects::none) && fn->result->isVoid() ) {
            DAS_FATAL_LOG("can't add function %s to module %s; it has no side effects and no return type\n", mangledName.c_str(), name.c_str() );
            DAS_FATAL_ERROR;
        }
        if ( fn->builtIn && fn->sideEffectFlags==uint32_t(SideEffects::modifyArgument)  ) {
            bool anyRW = false;
            for ( const auto & arg : fn->arguments ) {
                if ( arg->type->isRef() && !arg->type->isConst() ) {
                    anyRW = true;
                } else if ( arg->type->isPointer() && arg->type->firstType && !arg->type->firstType->isConst() ) {
                    anyRW = true;
                }
            }
            if ( !anyRW ) {
                DAS_FATAL_LOG("can't add function %s to module %s; modify argument requires non-const ref argument\n", mangledName.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
        }
        if ( functions.insert(make_pair(mangledName, fn)).second ) {
            functionsByName[fn->name].push_back(fn);
            fn->module = this;
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate function %s to module %s\n", mangledName.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    bool Module::addGeneric ( const FunctionPtr & fn, bool canFail ) {
        auto mangledName = fn->getMangledName();
        if ( generics.insert(make_pair(mangledName, fn)).second ) {
            genericsByName[fn->name].push_back(fn);
            fn->module = this;
            return true;
        } else {
            if ( !canFail ) {
                DAS_FATAL_LOG("can't add duplicate generic function %s to module %s\n", mangledName.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            return false;
        }
    }

    TypeDeclPtr Module::findAlias ( const string & na ) const {
        auto it = aliasTypes.find(na);
        return it != aliasTypes.end() ? it->second : TypeDeclPtr();
    }

    VariablePtr Module::findVariable ( const string & na ) const {
        auto it = globals.find(na);
        return it != globals.end() ? it->second : VariablePtr();
    }

    FunctionPtr Module::findFunction ( const string & mangledName ) const {
        auto it = functions.find(mangledName);
        return it != functions.end() ? it->second : FunctionPtr();
    }

    FunctionPtr Module::findUniqueFunction ( const string & mangledName ) const {
        auto it = functionsByName.find(mangledName);
        if ( it==functionsByName.end() ) return nullptr;
        if ( it->second.size()!=1 ) return nullptr;
        return it->second[0];
    }

    StructurePtr Module::findStructure ( const string & na ) const {
        auto it = structures.find(na);
        return it != structures.end() ? it->second : StructurePtr();
    }

    AnnotationPtr Module::findAnnotation ( const string & na ) const {
        auto it = handleTypes.find(na);
        return it != handleTypes.end() ? it->second : nullptr;
    }

    TypeInfoMacroPtr Module::findTypeInfoMacro ( const string & na ) const {
        auto it = typeInfoMacros.find(na);
        return it != typeInfoMacros.end() ? it->second : nullptr;
    }

    EnumerationPtr Module::findEnum ( const string & na ) const {
        auto it = enumerations.find(na);
        return it != enumerations.end() ? it->second : nullptr;
    }

    ExprCallFactory * Module::findCall ( const string & na ) const {
        auto it = callThis.find(na);
        return it != callThis.end() ? &it->second : nullptr;
    }

    bool Module::compileBuiltinModule ( const string & modName, unsigned char * str, unsigned int str_len ) {
        TextWriter issues;
        str[str_len-1] = 0;//replace last symbol with null terminating. fixme: This is sloppy, and assumes there is something to replace!
        auto access = make_smart<FileAccess>();
        auto fileInfo = make_unique<FileInfo>((char *) str, uint32_t(str_len));
        access->setFileInfo(modName, move(fileInfo));
        ModuleGroup dummyLibGroup;
        auto program = parseDaScript(modName, access, issues, dummyLibGroup, true);
        ownFileInfo = access->letGoOfFileInfo(modName);
        DAS_ASSERTF(ownFileInfo,"something went wrong and FileInfo for builtin module can not be obtained");
        if ( program ) {
            if (program->failed()) {
                for (auto & err : program->errors) {
                    issues << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
                }
                DAS_FATAL_LOG("%s\n", issues.str().c_str());
                DAS_FATAL_LOG("builtin module did not compile %s\n", modName.c_str());
                DAS_FATAL_ERROR;
                return false;
            }
            // ok, now let's rip content
            for (auto & at : program->thisModule->aliasTypes) {
                addAlias(at.second);
            }
            for (auto & en : program->thisModule->enumerations) {
                addEnumeration(en.second);
            }
            for (auto & st : program->thisModule->structures) {
                addStructure(st.second);
            }
            for (auto & gen : program->thisModule->generics) {
                addGeneric(gen.second);
            }
            for (auto & gvar : program->thisModule->globalsInOrder) {
                addVariable(gvar);
            }
            for (auto & fun : program->thisModule->functions) {
                addFunction(fun.second);
            }
            for (auto & rqm : program->thisModule->requireModule) {
                if ( rqm.first != this ) {
                    requireModule[rqm.first] |= rqm.second;
                }
            }
            // macros
            auto ptm = program->thisModule.get();
            if ( ptm->macroContext ) {
                swap ( macroContext, ptm->macroContext );
                for ( auto & fna : ptm->handleTypes ) {
                    addAnnotation(fna.second);
                }
            }
            variantMacros.insert(variantMacros.end(), ptm->variantMacros.begin(), ptm->variantMacros.end());
            macros.insert(macros.end(), ptm->macros.begin(), ptm->macros.end());
            inferMacros.insert(inferMacros.end(), ptm->inferMacros.begin(), ptm->inferMacros.end());
            optimizationMacros.insert(optimizationMacros.end(), ptm->optimizationMacros.begin(), ptm->optimizationMacros.end());
            lintMacros.insert(lintMacros.end(), ptm->lintMacros.begin(), ptm->lintMacros.end());
            for ( auto & rm : ptm->readMacros ) {
                addReaderMacro(rm.second);
            }
            return true;
        } else {
            DAS_FATAL_LOG("builtin module did not parse %s\n", modName.c_str());
            DAS_FATAL_ERROR;
            return false;
        }
    }

    bool isValidBuiltinName ( const string & name, bool canPunkt ) {
        bool hasPunkt = false;
        bool hasAlNum = false;
        for ( auto ch : name ) {
            if ( isalpha(ch) ) {
                hasAlNum = true;
                if ( isupper(ch) ) {
                    return false;
                }
            } else if ( isdigit(ch) || ch=='_' || ch=='`' ) {
                hasAlNum = true;
            } else if ( ispunct(ch) ) {
                if ( canPunkt ) {
                    hasPunkt = true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
        return hasPunkt ^ hasAlNum; // has punkt, or alnum, but never both
    }

    void Module::verifyBuiltinNames(uint32_t flags) {
        bool failed = false;
        if ( flags & VerifyBuiltinFlags::verifyAliasTypes ) {
            for ( auto & it : aliasTypes ) {
                if ( !isValidBuiltinName(it.first) ) {
                    DAS_FATAL_LOG("%s - alias type has incorrect name. expecting snake_case\n", it.first.c_str());
                    failed = true;
                }
            }
        }
        if ( flags & VerifyBuiltinFlags::verifyHandleTypes ) {
            for ( auto & it : handleTypes ) {
                if ( !isValidBuiltinName(it.first) ) {
                    DAS_FATAL_LOG("%s - annotation has incorrect name. expecting snake_case\n", it.first.c_str());
                    failed = true;
                }
            }
        }
        if ( flags & VerifyBuiltinFlags::verifyGlobals ) {
            for ( auto & var : globalsInOrder ) {
                if ( !isValidBuiltinName(var->name) ) {
                    DAS_FATAL_LOG("%s - global variable has incorrect name. expecting snake_case\n", var->name.c_str());
                    failed = true;
                }
            }
        }
        if ( flags & VerifyBuiltinFlags::verifyFunctions ) {
            for ( auto & it : functions ) {
                auto fun = it.second.get();
                if ( !isValidBuiltinName(fun->name, true) ) {
                    DAS_FATAL_LOG("%s - function has incorrect name. expecting snake_case\n", fun->name.c_str());
                    failed = true;
                }
            }
        }
        if ( flags & VerifyBuiltinFlags::verifyGenerics ) {
            for ( auto & it : generics ) {
                auto fun = it.second.get();
                if ( !isValidBuiltinName(fun->name) ) {
                    DAS_FATAL_LOG("%s - generic function has incorrect name. expecting snake_case\n", fun->name.c_str());
                    failed = true;
                }
            }
        }
        if ( flags & VerifyBuiltinFlags::verifyStructures ) {
            for ( auto & it : structures ) {
                auto st = it.second;
                if ( !isValidBuiltinName(st->name) ) {
                    DAS_FATAL_LOG("%s - structure has incorrect name. expecting snake_case\n", st->name.c_str());
                    failed = true;
                }
                if ( flags & VerifyBuiltinFlags::verifyStructureFields ) {
                    for ( auto & fd : st->fields ) {
                        if ( !isValidBuiltinName(fd.name) ) {
                            DAS_FATAL_LOG("%s.%s - structure field has incorrect name. expecting snake_case\n", st->name.c_str(), fd.name.c_str());
                            failed = true;
                        }
                    }
                }
            }
        }
        if ( flags & VerifyBuiltinFlags::verifyEnums ) {
            for ( auto & it : enumerations ) {
                auto en = it.second;
                if ( !isValidBuiltinName(en->name) ) {
                    DAS_FATAL_LOG("%s - enumeration has incorrect name. expecting snake_case\n", en->name.c_str());
                    failed = true;
                }
                if ( flags & VerifyBuiltinFlags::verifyEnumFields ) {
                    for ( auto & fd : en->list ) {
                        if ( !isValidBuiltinName(fd.name) ) {
                            DAS_FATAL_LOG("%s.%s - enumeration field has incorrect name. expecting snake_case\n", en->name.c_str(), fd.name.c_str());
                            failed = true;
                        }
                    }
                }
            }
        }
        DAS_ASSERTF(!failed, "verifyBuiltinNames failed");
    }

    void Module::verifyAotReady() {
        bool failed = false;
        for ( auto & it : functions ) {
            auto fun = it.second.get();
            if ( fun->builtIn ) {
                auto bif = (BuiltInFunction *) fun;
                if ( !bif->policyBased && bif->cppName.empty() ) {
                    DAS_FATAL_LOG("builtin function %s is missing cppName\n", fun->describe().c_str());
                    failed = true;
                }
            }
        }
        DAS_ASSERTF(!failed, "verifyAotReady failed");
    }

    // MODULE LIBRARY

    void ModuleLibrary::addBuiltInModule () {
        addModule(Module::require("$"));
    }

    void ModuleLibrary::addModule ( Module * module ) {
        DAS_ASSERTF(module, "module not found? or you have forgotten to NEED_MODULE(Module_BuiltIn) be called first");
        if ( module ) {
            if ( find(modules.begin(),modules.end(),module)==modules.end() ) {
                modules.push_back(module);
                module->addPrerequisits(*this);
            }
        }
    }

    void ModuleLibrary::foreach ( const callable<bool (Module * module)> & func, const string & moduleName ) const {
        bool any = moduleName=="*";
        for ( auto pm : modules ) {
            if ( !any && pm->name!=moduleName ) continue;
            if ( !func(pm) ) break;
        }
    }

    void ModuleLibrary::foreach_in_order ( const callable<bool (Module * module)> & func, Module * thisM ) const {
        DAS_ASSERT(modules.size());
        // {builtin} {THIS_MODULE} {require1} {require2} ...
        for ( auto m = modules.begin(); m!=modules.end(); ++m ) {
            if ( *m==thisM ) continue;
            if ( !func(*m) ) return;
        }
        func(thisM);
    }

    Module * ModuleLibrary::findModule ( const string & mn ) const {
        auto it = find_if(modules.begin(), modules.end(), [&](Module * mod){
            return mod->name == mn;
        });
        return it!=modules.end() ? *it : nullptr;
    }

    vector<TypeDeclPtr> ModuleLibrary::findAlias ( const string & name, Module * inWhichModule ) const {
        vector<TypeDeclPtr> ptr;
        string moduleName, aliasName;
        splitTypeName(name, moduleName, aliasName);
        foreach([&](Module * pm) -> bool {
            if ( !inWhichModule || inWhichModule->isVisibleDirectly(pm) )
                if ( auto pp = pm->findAlias(aliasName) )
                    ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<AnnotationPtr> ModuleLibrary::findAnnotation ( const string & name, Module * inWhichModule ) const {
        vector<AnnotationPtr> ptr;
        string moduleName, annName;
        splitTypeName(name, moduleName, annName);
        foreach([&](Module * pm) -> bool {
            if ( !inWhichModule || inWhichModule->isVisibleDirectly(pm) )
                if ( auto pp = pm->findAnnotation(annName) )
                    ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<TypeInfoMacroPtr> ModuleLibrary::findTypeInfoMacro ( const string & name, Module * inWhichModule ) const {
        vector<TypeInfoMacroPtr> ptr;
        string moduleName, annName;
        splitTypeName(name, moduleName, annName);
        foreach([&](Module * pm) -> bool {
            if ( !inWhichModule || inWhichModule->isVisibleDirectly(pm) )
                if ( auto pp = pm->findTypeInfoMacro(annName) )
                    ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<StructurePtr> ModuleLibrary::findStructure ( const string & name, Module * inWhichModule ) const {
        vector<StructurePtr> ptr;
        string moduleName, funcName;
        splitTypeName(name, moduleName, funcName);
        foreach([&](Module * pm) -> bool {
            if ( !inWhichModule || inWhichModule->isVisibleDirectly(pm) )
                if ( auto pp = pm->findStructure(funcName) )
                    ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<EnumerationPtr> ModuleLibrary::findEnum ( const string & name, Module * inWhichModule ) const {
        vector<EnumerationPtr> ptr;
        string moduleName, enumName;
        splitTypeName(name, moduleName, enumName);
        foreach([&](Module * pm) -> bool {
            if ( !inWhichModule || inWhichModule->isVisibleDirectly(pm) )
                if ( auto pp = pm->findEnum(enumName) )
                    ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    TypeDeclPtr ModuleLibrary::makeStructureType ( const string & name ) const {
        auto t = make_smart<TypeDecl>(Type::tStructure);
        auto structs = findStructure(name,nullptr);
        if ( structs.size()==1 ) {
            t->structType = structs.back().get();
        } else {
            DAS_FATAL_LOG("makeStructureType(%s) failed\n", name.c_str());
            DAS_FATAL_ERROR;
            return nullptr;
        }
        return t;
    }

    TypeDeclPtr ModuleLibrary::makeHandleType ( const string & name ) const {
        auto t = make_smart<TypeDecl>(Type::tHandle);
        auto handles = findAnnotation(name,nullptr);
        if ( handles.size()==1 ) {
            if ( handles.back()->rtti_isHandledTypeAnnotation() ) {
                t->annotation = static_cast<TypeAnnotation*>(handles.back().get());
            } else {
                DAS_FATAL_LOG("makeHandleType(%s) failed, not a handle type\n", name.c_str());
                DAS_FATAL_ERROR;
                return nullptr;
            }
        } else if ( handles.size()==0 ) {
            DAS_FATAL_LOG("makeHandleType(%s) failed, missing annotation\n", name.c_str());
            DAS_FATAL_ERROR;
            return nullptr;
        } else {
            DAS_FATAL_LOG("makeHandleType(%s) failed, duplicate annotation\n", name.c_str());
            DAS_FATAL_ERROR;
            return nullptr;
        }
        return t;
    }

    TypeDeclPtr ModuleLibrary::makeEnumType ( const string & name ) const {
        auto enums = findEnum(name,nullptr);
        if ( enums.size()==1 ) {
            return enums.back()->makeEnumType();
        } else {
            DAS_FATAL_LOG("makeEnumType(%s) failed\n", name.c_str());
            DAS_FATAL_ERROR;
            return nullptr;
        }
    }

    // Module group

    ModuleGroup::~ModuleGroup() {
        for ( auto & mod : modules ) {
            if ( !mod->builtIn ) {
                delete mod;
            }
        }
    }

    ModuleGroupUserData * ModuleGroup::getUserData ( const string & dataName ) const {
        auto it = userData.find(dataName);
        return it != userData.end() ? it->second.get() : nullptr;
    }

    bool ModuleGroup::setUserData ( ModuleGroupUserData * data ) {
        auto it = userData.find(data->name);
        if ( it != userData.end() ) {
            return false;
        }
        userData[data->name] = ModuleGroupUserDataPtr(data);
        return true;
    }

    bool Module::isVisibleDirectly ( Module * objModule ) const {
        if ( objModule==this ) return true;
        return requireModule.find(objModule) != requireModule.end();
    }
}

