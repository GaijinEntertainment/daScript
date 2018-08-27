//
//  main.cpp
//  yzg
//
//  Created by Boris Batkin on 8/23/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include <iostream>

#include "reader.hpp"

using namespace std;
using namespace yzg;

void test ( const string & fn )
{
    ifstream fs(fn);
    if ( !fs.is_open() )
        throw "can't open";
    auto node = read(fs);
    cout << *node << "\n";
}

int main(int argc, const char * argv[]) {
    test("./test/test_1.yzg");
    return 0;
}
