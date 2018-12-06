#include "precomp.h"

#include "ast.h"
#include "interop.h"

using namespace std;
using namespace yzg;

#define REPORT_ERRORS 1

#pragma pack(1)
struct Object
{
    float3   pos;
    float3   vel;
};
#pragma pack()

__attribute__((noinline)) void updateObject ( Object * obj )
{
    obj->pos.x += obj->vel.x;
    obj->pos.y += obj->vel.y;
    obj->pos.z += obj->vel.z;
}

__attribute__((noinline)) void updateTest ( Object * objects )
{
    for ( int i=0; i<10000; ++i ) {
        updateObject(objects + i);
    }
}

Context * g_Context = nullptr;
void stack_depth_4(const char * str)
{
    int fnIndex = g_Context->findFunction("stack_depth_5");
    const char * message = "from c++";
    __m128 args[1] = { cast<char *>::from((char *)message) };
    g_Context->eval(fnIndex, args);
}

// this is how we declare type
template <>
struct typeFactory<Object *> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeStructureType("Object");
    }
};

#include <mach/mach.h>
#include <mach/mach_time.h>

template <typename TT>
inline double profileBlock ( int numIter, TT && block ) {
    uint64_t minT = -1;
    for ( int i = 0; i != numIter; ++i ) {
        uint64_t t0 = mach_absolute_time();
        block();
        uint64_t t1 = mach_absolute_time();
        minT = min(t1-t0, minT);
    }
    mach_timebase_info_data_t s_timebase_info;
    mach_timebase_info(&s_timebase_info);
    return minT * s_timebase_info.numer / (1000000000.0 * s_timebase_info.denom);
}

void unit_test ( const string & fn, int numIter = 1 )
{
    string str;
    ifstream t(fn);
    if ( !t.is_open() )
        throw "can't open";
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), [](const ProgramPtr & prog){
        addExtern<decltype(stack_depth_4),stack_depth_4>(*prog->thisModule,prog->library,"stack_depth_4");
    }) ) {
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what );
            }
        } else {
            cout << *program << "\n";
            Context ctx(&str);
            g_Context = &ctx;
            program->simulate(ctx);
            int fnTest = ctx.findFunction("test");
            if ( fnTest != -1 ) {
                double simT = profileBlock(numIter, [&](){
                    ctx.restart();
                    ctx.eval(fnTest, nullptr);
                    if ( auto ex = ctx.getException() )
                        cout << "exception: " << ex << "\n";
                });
                cout << fn << " took:" << fixed << simT << "\n";
            } else {
                cout << "function 'test' not found\n";
            }
        }
    }
}

void unit_test_array_of_structures ( const string & fn )
{
    string str;

    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "can't open\n";
        return;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    auto program = parseDaScript(str.c_str(), [&](const ProgramPtr & prog){
        addExtern<decltype(updateObject),updateObject>(*prog->thisModule,prog->library,"interopUpdate");
    });
    if ( !program ) {
        cout << "can't parse\n";
        return;
    }
    if ( program->failed() ) {
        for ( auto & err : program->errors ) {
            cout << reportError(&str, err.at.line, err.at.column, err.what );
        }
    } else {
        cout << *program << "\n";
        
        Context ctx(&str);
        program->simulate(ctx);
        ctx.eval(ctx.findFunction("init"), nullptr);
        
        // NOTE: this demonstrates particular shader
        Object * objects = cast<Object *>::to ( ctx.getVariable( ctx.findVariable("objects") ) );
        cout << "objects at " << hex << uint64_t(objects) << dec << endl;
        cout << "before:\n";
        for ( int i=0; i!=5; ++i ) {
            Object * var = objects + i;
            cout << "object[" << i << "].position = " << var->pos.x << "," << var->pos.y << "," << var->pos.z << "\n";
            cout << "object[" << i << "].velocity = " << var->vel.x << "," << var->vel.y << "," << var->vel.z << "\n";
        }
        
        int numIter = 100;
        
        int fnTest = ctx.findFunction("test");
        double simT = profileBlock(numIter, [&](){
            ctx.restart();
            ctx.eval(fnTest, nullptr);
        });
        
        double cT = profileBlock(numIter, [&](){
            updateTest((Object *)objects);
        });
        
        int fniTest = ctx.findFunction("interopTest");
        double intT = profileBlock(numIter, [&](){
            ctx.restart();
            ctx.eval(fniTest, nullptr);
        });

        int updateFn = ctx.findFunction("update");
        double manyT = profileBlock(numIter, [&](){
            ctx.restart();
            for ( int oi=0; oi != 10000; ++oi ) {
                __m128 args[1] = { cast<Object *>::from(objects+oi) };
                ctx.eval(updateFn,  args);
            }
        });
        
        int ksUpdateFn = ctx.findFunction("ks_update");
        double manyKsT = profileBlock(numIter, [&](){
            ctx.restart();
            for ( int oi=0; oi != 10000; ++oi ) {
                __m128 args[2] = { cast<float3 *>::from(&objects[oi].pos), cast<float3>::from(objects[oi].vel) };
                ctx.eval(ksUpdateFn,  args);
            }
        });
        
        // NOTE: this demonstrates result of particular shader
        cout << "after:\n";
        for ( int i=0; i!=5; ++i ) {
            Object * var = objects + i;
            cout << "object[" << i << "].position = " << var->pos.x << "," << var->pos.y << "," << var->pos.z << "\n";
            cout << "object[" << i << "].velocity = " << var->vel.x << "," << var->vel.y << "," << var->vel.z << "\n";
        }
        
        cout << fixed;
        cout << "iterations took:" << simT << "\n";
        cout << "c++ version took:" << cT << "\n";
        cout << "interop version took:" << intT << "\n";
        cout << "10000-interop version took:" << manyT << "\n";
        cout << "10000-interop ks version took:" << manyKsT << "\n";
        cout << "ratio sim / c: " << simT / cT << "\n";
        cout << "ratio interop / c: " << intT / cT << "\n";
        cout << "ratio 10000-interop / c: " << manyT / cT << "\n";
        cout << "ratio 10000-interop ks / c: " << manyKsT / cT << "\n";
        cout << "ratio sim / interop: " << simT / intT << "\n";
    }
}


int main(int argc, const char * argv[]) {
//    unit_test("../../test/hello_world.das");
    unit_test("../../test/unit_test.das");
#if 0
    unit_test_array_of_structures("../../test/profile_array_of_structures_vec.das");
    unit_test("../../test/profile_try_catch.das", 100);
#endif
    return 0;
}
