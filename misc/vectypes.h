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
    
    template <typename TT>
    struct vec2
    {
        TT   x, y;
        inline friend ostream& operator<< (ostream& stream, const vec2<TT> & vec) {
            stream << vec.x << "," << vec.y;
            return stream;
        }
    };
    
    template <typename TT>
    struct vec3
    {
        TT   x, y, z;
        inline friend ostream& operator<< (ostream& stream, const vec3<TT> & vec) {
            stream << vec.x << "," << vec.y << "," << vec.z;
            return stream;
        }
    };
    
    template <typename TT>
    struct vec4
    {
        TT  x, y, z, w;
        inline friend ostream& operator<< (ostream& stream, const vec4<TT> & vec) {
            stream << vec.x << "," << vec.y << "," << vec.z << "," << vec.w;
            return stream;
        }
    };
    
    typedef vec2<float> float2;
    typedef vec3<float> float3;
    typedef vec4<float> float4;
    
    typedef vec2<int32_t> int2;
    typedef vec3<int32_t> int3;
    typedef vec4<int32_t> int4;
    
    typedef vec2<uint32_t> uint2;
    typedef vec3<uint32_t> uint3;
    typedef vec4<uint32_t> uint4;
}

#endif /* vectypes_h */
