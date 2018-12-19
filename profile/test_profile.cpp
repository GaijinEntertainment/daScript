#include "precomp.h"

#include "test_profile.h"

#include "ast.h"
#include "ast_interop.h"
#include "ast_handle.h"

using namespace std;
using namespace yzg;

__attribute__((noinline)) void updateObject ( Object & obj ) {
    obj.pos.x += obj.vel.x;
    obj.pos.y += obj.vel.y;
    obj.pos.z += obj.vel.z;
}

void updateTest ( ObjectArray & objects ) {
    for ( auto & obj : objects ) {
        updateObject(obj);
    }
}

void update10000 ( ObjectArray & objects, Context * context ) {
    int updateFn = context->findFunction("update");
    for ( auto & obj : objects ) {
        __m128 args[1] = { cast<Object &>::from(obj) };
        context->eval(updateFn,  args);
    }
}

void update10000ks ( ObjectArray & objects, Context * context ) {
    int ksUpdateFn = context->findFunction("ks_update");
    for ( auto & obj : objects ) {
        __m128 args[2] = { cast<float3 &>::from(obj.pos), cast<float3>::from(obj.vel) };
        context->eval(ksUpdateFn,  args);
    }
}

struct ObjectStructureTypeAnnotation : ManagedStructureAnnotation <Object> {
    ObjectStructureTypeAnnotation() : ManagedStructureAnnotation ("Object") {
        addField("position", offsetof(Object,pos),make_shared<TypeDecl>(Type::tFloat3));
        addField("velocity", offsetof(Object,vel),make_shared<TypeDecl>(Type::tFloat3));
    }
};

template <>
struct typeFactory<Object> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("Object");
    }
};

template <>
struct typeFactory<ObjectArray> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("ObjectArray");
    }
};

class Module_TestProfile : public Module {
public:
    Module_TestProfile() : Module("testProfile") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // register types
        addHandle(make_shared<ObjectStructureTypeAnnotation>());
        addHandle(make_shared<ManagedVectorAnnotation<Object>>("ObjectArray",lib.makeHandleType("Object")));
        // register functions
        addExtern<decltype(updateObject),updateObject>(*this,lib,"interopUpdate");
        addExtern<decltype(updateTest),updateTest>(*this,lib,"interopUpdateTest");
        addExtern<decltype(update10000), update10000>(*this,lib,"update10000");
        addExtern<decltype(update10000ks), update10000ks>(*this,lib,"update10000ks");
    }
};

REGISTER_MODULE(Module_TestProfile);


