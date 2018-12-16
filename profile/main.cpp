#include "precomp.h"

#include "ast.h"
#include "ast_interop.h"

#include <dirent.h>

using namespace std;
using namespace yzg;

struct Object {
    float3   pos;
    float3   vel;
};

__attribute__((noinline)) void updateObject ( Object * obj ) {
    obj->pos.x += obj->vel.x;
    obj->pos.y += obj->vel.y;
    obj->pos.z += obj->vel.z;
}

void updateTest ( Object objects[10000] ) {
    for ( int i=0; i!=10000; ++i ) {
        updateObject(objects + i);
    }
}

void update10000 ( Object objects[10000], Context * context ) {
    int updateFn = context->findFunction("update");
    for ( int oi=0; oi != 10000; ++oi ) {
        __m128 args[1] = { cast<Object *>::from(objects+oi) };
        context->eval(updateFn,  args);
    }
}

void update10000ks ( Object objects[10000], Context * context ) {
    int ksUpdateFn = context->findFunction("ks_update");
    for ( int oi=0; oi != 10000; ++oi ) {
        __m128 args[2] = {
            cast<float3 *>::from(&objects[oi].pos), cast<float3>::from(objects[oi].vel) };
        context->eval(ksUpdateFn,  args);
    }
}

struct ObjectStructureTypeAnnotation : StructureTypeAnnotation<Object> {
    ObjectStructureTypeAnnotation() : StructureTypeAnnotation("Object") {
        addField("position", offsetof(Object,pos),make_shared<TypeDecl>(Type::tFloat3));
        addField("velocity", offsetof(Object,vel),make_shared<TypeDecl>(Type::tFloat3));
    }
};

// this is how we declare type
template <>
struct typeFactory<Object *> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("Object");
    }
};

class Module_TestProfile : public Module {
public:
    Module_TestProfile() : Module{"testProfile" }{
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // register types
        addHandle(make_unique<ObjectStructureTypeAnnotation>());
        // register functions
        addExtern<decltype(updateObject),updateObject>(*this,lib,"interopUpdate");
        addExternEx<decltype(updateTest),updateTest,void,Object[10000]>(*this,lib,"interopUpdateTest");
        addExternEx<decltype(update10000), update10000, void, Object[10000], Context *>(*this,lib,"update10000");
        addExternEx<decltype(update10000ks), update10000ks, void, Object[10000], Context *>(*this,lib,"update10000ks");
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
    if ( auto program = parseDaScript(str.c_str(), [](const ProgramPtr & prog){}) ) {
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
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                ctx.restart();
                bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
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
