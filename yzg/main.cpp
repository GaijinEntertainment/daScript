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

void test_reader ( const string & fn )
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
        cout << *node << "\n";
#if REPORT_ERRORS
    } catch ( const read_error & error ) {
        reportError ( str, error.at, error.what() );
    }
#endif
}

void test_ast ( const string & fn )
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
        auto program = parse(node, [&](const ProgramPtr & prog){});
        cout << *program << "\n";
        
        Context ctx;
        program->simulate(ctx);
        
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

//////////////////
// interop example
// TODO:
//  this can be 100% generated via appropriate variadic templates
//  at some point we need to replace it so that we don't type as much

struct SimNode_InteropUpdate : public SimNode_Call
{
        virtual __m128 eval ( Context & context ) override {
            evalArgs(context);
            Object * pObject = ptr_cast_to<Object>(argValues[0]);
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
        Object * objects = ptr_cast_to<Object> ( ctx.getVariable( ctx.findVariable("objects") ) );
        cout << "objects at " << hex << uint64_t(objects) << endl;
        cout << "before:\n";
        for ( int i=0; i!=5; ++i ) {
            Object * var = objects + i;
            cout << "object[" << i << "].position = " << var->pos[0] << "," << var->pos[1] << "," << var->pos[2] << "\n";
            cout << "object[" << i << "].velocity = " << var->vel[0] << "," << var->vel[1] << "," << var->vel[2] << "\n";
        }
        
        clock_t t0 = clock();
        int numIter = 100;
        for ( int i=0; i < numIter; ++i )
            ctx.call(ctx.findFunction("test"), nullptr);
        clock_t t1 = clock();
        
        clock_t t2 = clock();
        for ( int i=0; i < numIter; ++i )
            updateTest((Object *)objects);
        clock_t t3 = clock();
        
        clock_t t4 = clock();
        for ( int i=0; i < numIter; ++i )
            ctx.call(ctx.findFunction("interopTest"), nullptr);
        clock_t t5 = clock();
        
        clock_t t6 = clock();
        {
            int updateFn = ctx.findFunction("update");
            for ( int i=0; i < numIter; ++i ) {
                for ( int oi=0; oi != 10000; ++oi ) {
                    __m128 args[1] = { ptr_cast_from(objects+oi) };
                    ctx.call(updateFn,  args);
                }
            }
        }
        clock_t t7 = clock();
        
        clock_t t8 = clock();
        for ( int i=0; i < numIter; ++i )
            ctx.call(ctx.findFunction("foreachTest"), nullptr);
        clock_t t9 = clock();
        
        // NOTE: this demonstrates result of particular shader
        cout << "after:\n";
        for ( int i=0; i!=5; ++i ) {
            Object * var = objects + i;
            cout << "object[" << i << "].position = " << var->pos[0] << "," << var->pos[1] << "," << var->pos[2] << "\n";
            cout << "object[" << i << "].velocity = " << var->vel[0] << "," << var->vel[1] << "," << var->vel[2] << "\n";
        }
        
        double simT = double(t1-t0) / (CLOCKS_PER_SEC * numIter);
        double cT = double(t3-t2) / (CLOCKS_PER_SEC * numIter);
        double intT = double(t5-t4) / (CLOCKS_PER_SEC * numIter);
        double manyT = double(t7-t6) / (CLOCKS_PER_SEC * numIter);
        double simFT = double(t9-t8) / (CLOCKS_PER_SEC * numIter);
        
        cout << fixed;
        cout << "iterations took:" << simT << "\n";
        cout << "foreach iterations took:" << simFT << "\n";
        cout << "c++ version took:" << cT << "\n";
        cout << "interop version took:" << intT << "\n";
        cout << "10000-interop version took:" << manyT << "\n";
        cout << "ratio sim / c: " << simT / cT << "\n";
        cout << "ratio foreach sim / c: " << simFT / cT << "\n";
        cout << "ratio interop / c: " << intT / cT << "\n";
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
    // test_reader("./test/test_1.yzg");
    // test_ast("./test/test_2.yzg");
    // test_ast("./test/profile_array_of_structures.yzg");
    unit_test("./test/profile_array_of_structures.yzg");
    return 0;
}
