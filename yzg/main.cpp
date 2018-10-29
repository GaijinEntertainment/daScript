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
        auto program = parse(node);
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
        auto program = parse(node);
        cout << *program << "\n";
        
        Context ctx;
        program->simulate(ctx);
        ctx.call(ctx.findFunction("init"), nullptr);
        
        // NOTE: this demonstrates particular shader
        /*
        float * objects = ptr_cast_to<float> ( ctx.getVariable( ctx.findVariable("objects") ) );
        float * var = objects;
        cout << "before:\n";
        for ( int i=0; i!=5; ++i ) {
            cout << "object[" << i << "].position = " << var[0] << "," << var[1] << "," << var[2] << "\n";
            cout << "object[" << i << "].velocity = " << var[3] << "," << var[4] << "," << var[5] << "\n";
            var += 6;
        }
        */
        
        clock_t t0 = clock();
        int numIter = 100;
        for ( int i=0; i < numIter; ++i )
            ctx.call(ctx.findFunction("test"), nullptr);
        clock_t t1 = clock();
        
        // NOTE: this demonstrates result of particular shader
        /*
        var = objects;
        cout << "after:\n";
        for ( int i=0; i!=5; ++i ) {
            cout << "object[" << i << "].position = " << var[0] << "," << var[1] << "," << var[2] << "\n";
            cout << "object[" << i << "].velocity = " << var[3] << "," << var[4] << "," << var[5] << "\n";
            var += 6;
        }
        */
        
        cout << "iterations took:" << double(t1-t0) / (CLOCKS_PER_SEC * numIter) << "\n";
        
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
