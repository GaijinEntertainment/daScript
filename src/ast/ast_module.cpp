#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    bool splitTypeName ( const string & name, string & moduleName, string & funcName ) {
        auto at = name.find("::");
        if ( at!=string::npos ) {
            moduleName = name.substr(0,at);
            funcName = name.substr(at+2);
            return true;
        } else {
            moduleName = "*";
            funcName = name;
            return false;
        }
    }

    // ANNOTATION

    const AnnotationArgument * AnnotationArgumentList::find ( const string & name, Type type ) const {
        auto it = find_if(arguments.begin(), arguments.end(), [&](const AnnotationArgument & arg){
            return (arg.name==name) && (type==Type::tVoid || type==arg.type);
        });
        return it==arguments.end() ? nullptr : &*it;
    }

    bool AnnotationArgumentList::getOption(const string & name, bool def) const {
        auto arg = find(name, Type::tBool);
        return arg ? arg->bValue : def;
    }

    // MODULE

    intptr_t Module::Karma = 0;
    Module * Module::modules = nullptr;

    TypeAnnotation * Module::resolveAnnotation ( TypeInfo * info ) {
        intptr_t ann = (intptr_t) (info->annotation_or_name);
        if ( ann & 1 ) {
            string name = (char *) ( ann & ~1 );
            string moduleName, annName;
            splitTypeName(name, moduleName, annName);
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

    void Module::Shutdown() {
        auto m = modules;
        while ( m ) {
            auto pM = m;
            m = m->next;
            delete pM;
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

    Module::Module ( const string & n ) : name(n) {
        if ( !name.empty() ) {
            next = modules;
            modules = this;
            builtIn = true;
        }
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

    bool Module::addVariable ( const VariablePtr & var, bool canFail ) {
        if ( globals.insert(make_pair(var->name, var)).second ) {
            var->module = this;
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
        auto mangledName = fn->getMangledName();
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

    StructurePtr Module::findStructure ( const string & na ) const {
        auto it = structures.find(na);
        return it != structures.end() ? it->second : StructurePtr();
    }

    AnnotationPtr Module::findAnnotation ( const string & na ) const {
        auto it = handleTypes.find(na);
        return it != handleTypes.end() ? it->second : nullptr;
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
        auto access = make_shared<FileAccess>();
        auto fileInfo = make_unique<FileInfo>((char *) str, uint32_t(str_len));
        access->setFileInfo(modName, move(fileInfo));
        ModuleGroup dummyLibGroup;
        auto program = parseDaScript(modName, access, issues, dummyLibGroup, true);
        ownFileInfo = access->letGoOfFileInfo(modName);
        DAS_ASSERTF(ownFileInfo,"something went wrong and FileInfo for builtin module can not be obtained");
        if ( program ) {
            if (program->failed()) {
                for (auto & err : program->errors) {
                    issues << reportError(err.at, err.what, err.cerr);
                }
                DAS_FATAL_LOG("%s\n", issues.str().c_str());
                DAS_FATAL_LOG("builtin module did not compile %s\n", modName.c_str());
                DAS_FATAL_ERROR;
                return false;
            }
            // ok, now let's rip content
            for (auto & gen : program->thisModule->generics) {
                addGeneric(gen.second);
            }
            for (auto & glob : program->thisModule->globals) {
                addVariable(glob.second);
            }
            for (auto & fun : program->thisModule->functions) {
                addFunction(fun.second);
            }
            return true;
        } else {
            DAS_FATAL_LOG("builtin module did not parse %s\n", modName.c_str());
            DAS_FATAL_ERROR;
            return false;
        }
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

    void ModuleLibrary::foreach ( function<bool (Module * module)> && func, const string & moduleName ) const {
        bool any = moduleName=="*";
        for ( auto pm : modules ) {
            if ( !any && pm->name!=moduleName ) continue;
            if ( !func(pm) ) break;
        }
    }

    Module * ModuleLibrary::findModule ( const string & mn ) const {
        auto it = find_if(modules.begin(), modules.end(), [&](Module * mod){
            return mod->name == mn;
        });
        return it!=modules.end() ? *it : nullptr;
    }

    vector<TypeDeclPtr> ModuleLibrary::findAlias ( const string & name ) const {
        vector<TypeDeclPtr> ptr;
        string moduleName, aliasName;
        splitTypeName(name, moduleName, aliasName);
        foreach([&](Module * pm) -> bool {
            if ( auto pp = pm->findAlias(aliasName) )
                ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<AnnotationPtr> ModuleLibrary::findAnnotation ( const string & name ) const {
        vector<AnnotationPtr> ptr;
        string moduleName, annName;
        splitTypeName(name, moduleName, annName);
        foreach([&](Module * pm) -> bool {
            if ( auto pp = pm->findAnnotation(annName) )
                ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<StructurePtr> ModuleLibrary::findStructure ( const string & name ) const {
        vector<StructurePtr> ptr;
        string moduleName, funcName;
        splitTypeName(name, moduleName, funcName);
        foreach([&](Module * pm) -> bool {
            if ( auto pp = pm->findStructure(funcName) )
                ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    vector<EnumerationPtr> ModuleLibrary::findEnum ( const string & name ) const {
        vector<EnumerationPtr> ptr;
        string moduleName, enumName;
        splitTypeName(name, moduleName, enumName);
        foreach([&](Module * pm) -> bool {
            if ( auto pp = pm->findEnum(enumName) )
                ptr.push_back(pp);
            return true;
        }, moduleName);
        return ptr;
    }

    TypeDeclPtr ModuleLibrary::makeStructureType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tStructure);
        auto structs = findStructure(name);
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
        auto t = make_shared<TypeDecl>(Type::tHandle);
        auto handles = findAnnotation(name);
        if ( handles.size()==1 ) {
            if ( handles.back()->rtti_isHandledTypeAnnotation() ) {
                t->annotation = static_pointer_cast<TypeAnnotation>(handles.back());
            } else {
                DAS_FATAL_LOG("makeHandleType(%s) failed, not a handle type\n", name.c_str());
                DAS_FATAL_ERROR;
                return nullptr;
            }
        } else {
            DAS_FATAL_LOG("makeHandleType(%s) failed, missing annotation\n", name.c_str());
            DAS_FATAL_ERROR;
            return nullptr;
        }
        return t;
    }

    TypeDeclPtr ModuleLibrary::makeEnumType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tEnumeration);
        auto enums = findEnum(name);
        if ( enums.size()==1 ) {
            t->enumType = enums.back();
        } else {
            DAS_FATAL_LOG("makeEnumType(%s) failed\n", name.c_str());
            DAS_FATAL_ERROR;
            return nullptr;
        }
        return t;
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
}

