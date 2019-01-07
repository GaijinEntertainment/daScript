#pragma once

namespace das
{
    using namespace std;
    
    template <typename TT>
    struct vec2 {
        TT   x, y;
        __forceinline friend ostream& operator<< (ostream& stream, const vec2<TT> & vec) {
            stream << vec.x << "," << vec.y;
            return stream;
        }
        __forceinline bool operator == ( const vec2<TT> & vec ) const {
            return x==vec.x && y==vec.y;
        }
    };
    
    template <typename TT>
    struct vec3 {
        TT   x, y, z;
        __forceinline friend ostream& operator<< (ostream& stream, const vec3<TT> & vec) {
            stream << vec.x << "," << vec.y << "," << vec.z;
            return stream;
        }
        __forceinline bool operator == ( const vec3<TT> & vec ) const {
            return x==vec.x && y==vec.y && z==vec.z;
        }
    };
    
    template <typename TT>
    struct vec4 {
        TT  x, y, z, w;
        __forceinline friend ostream& operator<< (ostream& stream, const vec4<TT> & vec) {
            stream << vec.x << "," << vec.y << "," << vec.z << "," << vec.w;
            return stream;
        }
        __forceinline bool operator == ( const vec4<TT> & vec ) const {
            return x==vec.x && y==vec.y && z==vec.z && w==vec.w;
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

