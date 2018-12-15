#include "precomp.h"

#include "ast.h"
#include "ast_interop.h"

#include <dirent.h>

using namespace std;
using namespace yzg;

#define REPORT_ERRORS 1

#pragma pack(1)
struct Object {
    float3   pos;
    float3   vel;
};
#pragma pack()

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
        __m128 args[2] = { cast<float3 *>::from(&objects[oi].pos), cast<float3>::from(objects[oi].vel) };
        context->eval(ksUpdateFn,  args);
    }
}

// this is how we declare type
template <>
struct typeFactory<Object *> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeStructureType("Object");
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
    if ( auto program = parseDaScript(str.c_str(), [](const ProgramPtr & prog){
        addExtern<decltype(updateObject),updateObject>(*prog->thisModule,prog->library,"interopUpdate");
        addExternEx<decltype(updateTest),updateTest,void,Object[10000]>(*prog->thisModule,prog->library,"interopUpdateTest");
        addExternEx<decltype(update10000), update10000, void, Object[10000], Context *>(*prog->thisModule,prog->library,"update10000");
        addExternEx<decltype(update10000ks), update10000ks, void, Object[10000], Context *>(*prog->thisModule,prog->library,"update10000ks");
    }) ) {
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
    unit_test("../../profile/profile_array_of_structures_vec.das");
    unit_test("../../profile/profile_try_catch.das");
    return 0;
}
