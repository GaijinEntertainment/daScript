//
//  main.cpp
//  yzg
//
//  Created by Boris Batkin on 8/23/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include <iostream>

#include "reader.hpp"
#include "ast.hpp"

using namespace std;
using namespace yzg;

void test_reader ( const string & fn )
{
    ifstream fs(fn);
    if ( !fs.is_open() )
        throw "can't open";
    auto node = read(fs);
    cout << *node << "\n";
}

void test_ast ( const string & fn )
{
    ifstream fs(fn);
    if ( !fs.is_open() )
        throw "can't open";
    auto node = read(fs);
    auto program = parse(node);
    cout << *program << "\n";
}

int main(int argc, const char * argv[]) {
    //test_reader("./test/test_1.yzg");
    test_ast("./test/test_2.yzg");
    return 0;
}
