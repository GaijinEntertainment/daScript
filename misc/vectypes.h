//
//  vectypes.h
//  yzg
//
//  Created by Boris Batkin on 11/1/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef vectypes_h
#define vectypes_h

#include <ostream>

namespace yzg
{
    using namespace std;
    
    struct float2
    {
        float   x, y;
        
        inline friend ostream& operator<< (ostream& stream, const float2 & vec) {
            stream << vec.x << "," << vec.y;
            return stream;
        }
    };
    
    struct float3
    {
        float   x, y, z;
        
        inline friend ostream& operator<< (ostream& stream, const float3 & vec) {
            stream << vec.x << "," << vec.y << "," << vec.z;
            return stream;
        }
    };
    
    struct float4
    {
        float   x, y, z, w;
        
        inline friend ostream& operator<< (ostream& stream, const float4 & vec) {
            stream << vec.x << "," << vec.y << "," << vec.z << "," << vec.w;
            return stream;
        }
    };
}

#endif /* vectypes_h */
