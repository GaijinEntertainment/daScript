#include "precomp.h"

#include "ast.h"

namespace yzg
{
    // MODULE
    
    intptr_t Module::Karma = 0;
    Module * Module::modules = nullptr;
    
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
        return handleTypes.insert(make_pair(ptr->name, move(ptr))).second;
    }
    
    bool Module::addVariable ( const VariablePtr & var ) {
        return globals.insert(make_pair(var->name, var)).second;
    }
    
    bool Module::addStructure ( const StructurePtr & st ) {
        return structures.insert(make_pair(st->name, st)).second;
    }
    
    bool Module::addFunction ( const FunctionPtr & fn ) {
        auto mangledName = fn->getMangledName();
        if ( functions.insert(make_pair(mangledName, fn)).second ) {
            functionsByName[fn->name].push_back(fn);
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
    
    TypeAnnotation * ModuleLibrary::findHandle ( const string & name ) const {
        TypeAnnotation * ptr = nullptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findHandle(name);
            return !ptr;
        });
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
    
    FunctionPtr ModuleLibrary::findFunction ( const string & mangledName ) const {
        FunctionPtr ptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findFunction(mangledName);
            return !ptr;
        });
        return ptr;
    }
    
    StructurePtr ModuleLibrary::findStructure ( const string & name ) const {
        StructurePtr ptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findStructure(name);
            return !ptr;
        });
        return ptr;
    }
    
    TypeDeclPtr ModuleLibrary::makeStructureType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tStructure);
        t->structType = findStructure(name).get();
        if ( !t->structType ) {
            assert(0 && "can't make structure type");
        }
        return t;
    }
    
    TypeDeclPtr ModuleLibrary::makeHandleType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tHandle);
        t->annotation = findHandle(name);
        if ( !t->annotation ) {
            assert(0 && "can't make hanlde type");
        }
        return t;
    }
}

