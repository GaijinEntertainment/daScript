//
//  main.cpp
//  yzg
//
//  Created by Boris Batkin on 8/23/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "reader.hpp"
#include "ast.hpp"

using namespace std;
using namespace yzg;

#define REPORT_ERRORS 1

#pragma pack(1)
struct Object
{
    float   pos[3];
    float   vel[3];
};
#pragma pack()

__attribute__((noinline)) void updateObject ( Object & obj )
{
    obj.pos[0] += obj.vel[0];
    obj.pos[1] += obj.vel[1];
    obj.pos[2] += obj.vel[2];
}

__attribute__((noinline)) void updateTest ( Object * objects )
{
    for ( int i=0; i<10000; ++i ) {
        updateObject(objects[i]);
    }
}

template <typename FuncT, typename RetT, typename ...Args>
struct SimNode_ExtFuncCall;

template <typename FuncT, typename ...Args>
struct SimNode_ExtFuncCall<FuncT,void,Args...> : public SimNode_Call
{
    virtual __m128 eval ( Context & context ) override {
        evalArgs(context);
        return _mm_setzero_ps();
    }
};


//////////////////
// interop example
// TODO:
//  this can be 100% generated via appropriate variadic templates
//  at some point we need to replace it so that we don't type as much

struct SimNode_InteropUpdate : public SimNode_Call
{
    virtual __m128 eval ( Context & context ) override {
        evalArgs(context);
        Object * pObject = cast<Object *>::to(argValues[0]);
        updateObject(*pObject);
        return _mm_setzero_ps();
    }
};

class BuiltInFunction_IteropUpdate : public BuiltInFunction
{
public:
    BuiltInFunction_IteropUpdate(const TypeDeclPtr & objType) : BuiltInFunction("interopUpdate") {
        auto arg = make_shared<Variable>();
        arg->name = "obj";
        arg->type = objType;
        this->arguments.push_back(arg);
        result = make_shared<TypeDecl>();
    }
    virtual SimNode * makeSimNode ( Context & context ) override {
        return context.makeNode<SimNode_InteropUpdate>();
    }
};

// end of interop example
/////////////////////////

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

void unit_test ( const string & fn )
{
    string str;
#if REPORT_ERRORS
    try {
#endif
        ifstream t(fn);
        if ( !t.is_open() )
            throw "can't open";
        t.seekg(0, ios::end);
        str.reserve(t.tellg());
        t.seekg(0, ios::beg);
        str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
        auto node = read(str);
        auto program = parse(node, nullptr);
        cout << *program << "\n";
        
        Context ctx;
        program->simulate(ctx);
        int numIter = 100;
        
        int fnTest = ctx.findFunction("test");
        double simT = profileBlock(numIter, [&](){
            ctx.call(fnTest, nullptr);
        });
        
        cout << fixed;
        cout << fn << " took:" << simT << "\n";
        
        
#if REPORT_ERRORS
    } catch ( const read_error & error ) {
        reportError ( str, error.at, error.what() );
    } catch ( const parse_error & error ) {
        reportError ( str, error.at ? error.at->at : str.begin(), error.what() );
    } catch ( const semantic_error & error ) {
        reportError ( str, error.at ? error.at->at : str.begin(), error.what() );
    }
#endif
}
    

void unit_test_array_of_structures ( const string & fn )
{
    string str;
#if REPORT_ERRORS
    try {
#endif
        ifstream t(fn);
        if ( !t.is_open() )
            throw "can't open";
        t.seekg(0, ios::end);
        str.reserve(t.tellg());
        t.seekg(0, ios::beg);
        str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
        auto node = read(str);
        auto program = parse(node, [&](const ProgramPtr & prog){
            auto structType = prog->structures["Object"].get();
            auto objType = make_shared<TypeDecl>(Type::tStructure);
            objType->structType = structType;
            objType->at = structType->at;
            prog->addBuiltIn(make_shared<BuiltInFunction_IteropUpdate>(objType));
        });
        cout << *program << "\n";
        
        Context ctx;
        program->simulate(ctx);
        ctx.call(ctx.findFunction("init"), nullptr);
        
        // NOTE: this demonstrates particular shader
        Object * objects = cast<Object *>::to ( ctx.getVariable( ctx.findVariable("objects") ) );
        cout << "objects at " << hex << uint64_t(objects) << dec << endl;
        cout << "before:\n";
        for ( int i=0; i!=5; ++i ) {
            Object * var = objects + i;
            cout << "object[" << i << "].position = " << var->pos[0] << "," << var->pos[1] << "," << var->pos[2] << "\n";
            cout << "object[" << i << "].velocity = " << var->vel[0] << "," << var->vel[1] << "," << var->vel[2] << "\n";
        }
        
        
        int numIter = 100;
        
        int fnTest = ctx.findFunction("test");
        double simT = profileBlock(numIter, [&](){
            ctx.call(fnTest, nullptr);
        });
        
        double cT = profileBlock(numIter, [&](){
            updateTest((Object *)objects);
        });
        
        int fniTest = ctx.findFunction("interopTest");
        double intT = profileBlock(numIter, [&](){
            ctx.call(fniTest, nullptr);
        });

        int updateFn = ctx.findFunction("update");
        double manyT = profileBlock(numIter, [&](){
            for ( int oi=0; oi != 10000; ++oi ) {
                __m128 args[1] = { cast<Object *>::from(objects+oi) };
                ctx.call(updateFn,  args);
            }
        });
        
        int fnfTest = ctx.findFunction("foreachTest");
        double simFT = profileBlock(numIter, [&](){
            ctx.call(fnfTest, nullptr);
        });
        
        int fnfiTest = ctx.findFunction("foreachIteropTest");
        double intFT = profileBlock(numIter, [&](){
            ctx.call(fnfiTest, nullptr);
        });
        
        // NOTE: this demonstrates result of particular shader
        cout << "after:\n";
        for ( int i=0; i!=5; ++i ) {
            Object * var = objects + i;
            cout << "object[" << i << "].position = " << var->pos[0] << "," << var->pos[1] << "," << var->pos[2] << "\n";
            cout << "object[" << i << "].velocity = " << var->vel[0] << "," << var->vel[1] << "," << var->vel[2] << "\n";
        }
        
        cout << fixed;
        cout << "iterations took:" << simT << "\n";
        cout << "foreach iterations took:" << simFT << "\n";
        cout << "c++ version took:" << cT << "\n";
        cout << "interop version took:" << intT << "\n";
        cout << "foreach interop version took:" << intFT << "\n";
        cout << "10000-interop version took:" << manyT << "\n";
        cout << "ratio sim / c: " << simT / cT << "\n";
        cout << "ratio foreach sim / c: " << simFT / cT << "\n";
        cout << "ratio interop / c: " << intT / cT << "\n";
        cout << "ratio foreach interop / c: " << intFT / cT << "\n";
        cout << "ratio 10000-interop / c: " << manyT / cT << "\n";
        cout << "ratio sim / interop: " << simT / intT << "\n";
        
#if REPORT_ERRORS
    } catch ( const read_error & error ) {
        reportError ( str, error.at, error.what() );
    } catch ( const parse_error & error ) {
        reportError ( str, error.at ? error.at->at : str.begin(), error.what() );
    } catch ( const semantic_error & error ) {
        reportError ( str, error.at ? error.at->at : str.begin(), error.what() );
    }
#endif
}

int main(int argc, const char * argv[]) {
    unit_test_array_of_structures("./test/profile_array_of_structures.yzg");
    unit_test_array_of_structures("./test/profile_array_of_structures_vec.yzg");
    unit_test("./test/try_catch.yzg");
    return 0;
}
