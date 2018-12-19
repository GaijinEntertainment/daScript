#include "precomp.h"

#include "ast.h"

namespace yzg
{
    // MODULE
    
    intptr_t Module::Karma = 0;
    Module * Module::modules = nullptr;
    
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
        }
    }
    
    Module::~Module() {
        if ( !name.empty() ) {
            Module ** p = &modules;
            for ( auto m = modules; m != nullptr; p = &m->next, m = m->next ) {
                if ( m == this ) {
                    *p = m->next;
                    return;
                }
            }
            assert(0 && "failed to unlink");
        }
    }
    
    bool Module::addHandle ( unique_ptr<TypeAnnotation> && ptr ) {
        auto pp = ptr.get();
        if ( handleTypes.insert(make_pair(ptr->name, move(ptr))).second ) {
            pp->module = this;
            return true;
        } else {
            return false;
        }
    }
    
    bool Module::addVariable ( const VariablePtr & var ) {
        if ( globals.insert(make_pair(var->name, var)).second ) {
            var->module = this;
            return true;
        } else {
            return false;
        }
    }
    
    bool Module::addStructure ( const StructurePtr & st ) {
        if ( structures.insert(make_pair(st->name, st)).second ) {
            st->module = this;
            return true;
        } else {
            return false;
        }
    }
    
    bool Module::addFunction ( const FunctionPtr & fn ) {
        auto mangledName = fn->getMangledName();
        if ( functions.insert(make_pair(mangledName, fn)).second ) {
            functionsByName[fn->name].push_back(fn);
            fn->module = this;
            return true;
        } else {
            // assert(0 && "can't add function");
            return false;
        }
    }
    
    VariablePtr Module::findVariable ( const string & name ) const {
        auto it = globals.find(name);
        return it != globals.end() ? it->second : VariablePtr();
    }
    
    FunctionPtr Module::findFunction ( const string & mangledName ) const {
        auto it = functions.find(mangledName);
        return it != functions.end() ? it->second : FunctionPtr();
    }
    
    StructurePtr Module::findStructure ( const string & name ) const {
        auto it = structures.find(name);
        return it != structures.end() ? it->second : StructurePtr();
    }
    
    TypeAnnotation * Module::findHandle ( const string & name ) const {
        auto it = handleTypes.find(name);
        return it != handleTypes.end() ? it->second.get() : nullptr;
    }
    
    ExprCallFactory * Module::findCall ( const string & name ) const {
        auto it = callThis.find(name);
        return it != callThis.end() ? &it->second : nullptr;
    }
    
    // MODULE LIBRARY
    
    void ModuleLibrary::addBuiltInModule () {
        addModule(Module::require("$"));
    }
    
    void ModuleLibrary::addModule ( Module * module ) {
        assert(module && "module not found?");
        modules.push_back(module);
    }

    void ModuleLibrary::foreach ( function<bool (Module * module)> && func ) const {
        for ( auto pm : modules ) {
            if ( !func(pm) ) break;
        }
    }
    
    vector<TypeAnnotation *> ModuleLibrary::findHandle ( const string & name ) const {
        vector<TypeAnnotation *> ptr;
        string moduleName, annName;
        if ( splitTypeName(name, moduleName, annName) ) {
            foreach([&](Module * pm) -> bool {
                if ( pm->name==moduleName ) {
                    if ( auto pp = pm->findHandle(annName) )
                        ptr.push_back(pp);
                    return false;
                } else {
                    return true;
                }
            });
        } else {
            foreach([&](Module * pm) -> bool {
                if ( auto pp = pm->findHandle(name) )
                    ptr.push_back(pp);
                return true;
            });
        }
        return ptr;
    }
    
    VariablePtr ModuleLibrary::findVariable ( const string & name ) const {
        VariablePtr ptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findVariable(name);
            return !ptr;
        });
        return ptr;
    }
    
    bool splitTypeName ( const string & name, string & moduleName, string & funcName ) {
        auto at = name.find("::");
        if ( at!=string::npos ) {
            moduleName = name.substr(0,at);
            funcName = name.substr(at+2);
            return true;
        } else {
            moduleName = "";
            funcName = name;
            return false;
        }
    }
    
    vector<StructurePtr> ModuleLibrary::findStructure ( const string & name ) const {
        vector<StructurePtr> ptr;
        string moduleName, funcName;
        if ( splitTypeName(name, moduleName, funcName) ) {
            foreach([&](Module * pm) -> bool {
                if ( pm->name==moduleName ) {
                    if ( auto pp = pm->findStructure(funcName) )
                        ptr.push_back(pp);
                    return false;
                } else {
                    return true;
                }
            });
        } else {
            foreach([&](Module * pm) -> bool {
                if ( auto pp = pm->findStructure(name) )
                    ptr.push_back(pp);
                return true;
            });
        }
        return ptr;
    }
    
    TypeDeclPtr ModuleLibrary::makeStructureType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tStructure);
        auto structs = findStructure(name);
        if ( structs.size()==1 ) {
            t->structType = structs.back().get();
        } else {
            assert(0 && "can't make structure type");
        }
        return t;
    }
    
    TypeDeclPtr ModuleLibrary::makeHandleType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tHandle);
        auto handles = findHandle(name);
        if ( handles.size()==1 ) {
            t->annotation = handles.back();
        } else {
            assert(0 && "can't make hanlde type");
        }
        return t;
    }
}

