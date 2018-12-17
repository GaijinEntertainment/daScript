#include "precomp.h"

#include "ast.h"
#include "ast_interop.h"
#include "ast_handle.h"

#include <dirent.h>

using namespace std;
using namespace yzg;

struct Object {
    float3   pos;
    float3   vel;
};

typedef vector<Object> ObjectArray;

__attribute__((noinline)) void updateObject ( Object * obj ) {
    obj->pos.x += obj->vel.x;
    obj->pos.y += obj->vel.y;
    obj->pos.z += obj->vel.z;
}

void updateTest ( ObjectArray * objects ) {
    for ( auto & obj : *objects ) {
        updateObject(&obj);
    }
}

void update10000 ( ObjectArray * objects, Context * context ) {
    int updateFn = context->findFunction("update");
    for ( auto & obj : *objects ) {
        __m128 args[1] = { cast<Object *>::from(&obj) };
        context->eval(updateFn,  args);
    }
}

void update10000ks ( ObjectArray * objects, Context * context ) {
    int ksUpdateFn = context->findFunction("ks_update");
    for ( auto & obj : *objects ) {
        __m128 args[2] = { cast<float3 *>::from(&obj.pos), cast<float3>::from(obj.vel) };
        context->eval(ksUpdateFn,  args);
    }
}

struct ObjectStructureTypeAnnotation : StructureTypeAnnotation<Object> {
    ObjectStructureTypeAnnotation() : StructureTypeAnnotation("Object") {
        addField("position", offsetof(Object,pos),make_shared<TypeDecl>(Type::tFloat3));
        addField("velocity", offsetof(Object,vel),make_shared<TypeDecl>(Type::tFloat3));
    }
};

template <>
struct typeFactory<Object *> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("Object");
    }
};

template <>
struct typeFactory<ObjectArray *> {
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
        addHandle(make_unique<ObjectStructureTypeAnnotation>());
        addHandle(make_unique<VectorTypeAnnotation<Object>>("ObjectArray",lib.makeHandleType("Object")));
        // register functions
        addExtern<decltype(updateObject),updateObject>(*this,lib,"interopUpdate");
        addExtern<decltype(updateTest),updateTest>(*this,lib,"interopUpdateTest");
        addExtern<decltype(update10000), update10000>(*this,lib,"update10000");
        addExtern<decltype(update10000ks), update10000ks>(*this,lib,"update10000ks");
    }
};

// TEST

bool unit_test ( const string & fn ) {
    // cout << fn << " ";
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "not found\n";
        return false;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str()) ) {
        if ( program->failed() ) {
            cout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
            return false;
        } else {
            // cout << *program << "\n";
            Context ctx(&str);
            program->simulate(ctx);
            // vector of 10000 objects
            vector<Object> objects;
            objects.resize(10000);
            // run the test
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                ctx.restart();
                __m128 args[1] = { cast<vector<Object> *>::from(&objects) };
                bool result = cast<bool>::to(ctx.eval(fnTest, args));
                if ( auto ex = ctx.getException() ) {
                    cout << "exception: " << ex << "\n";
                    return false;
                }
                if ( !result ) {
                    cout << "failed\n";
                    return false;
                }
                // cout << "ok\n";
                return true;
            } else {
                cout << "function 'test' not found\n";
                return false;
            }
        }
    } else {
        return false;
    }
}


int main(int argc, const char * argv[]) {
    // register modules
    auto module_TestProfile = make_unique<Module_TestProfile>();
    // run tests
    unit_test("../../profile/profile_array_of_structures_vec.das");
    unit_test("../../profile/profile_try_catch.das");
    return 0;
}
