//
//  simulate.cpp
//  yzg
//
//  Created by Boris Batkin on 10/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "simulate.hpp"

namespace yzg
{
    Context::~Context()
    {
        for ( auto pn : nodes ) {
            SimNode * pNode = pn;
            delete pNode;
        }
        nodes.clear();
    }
}
