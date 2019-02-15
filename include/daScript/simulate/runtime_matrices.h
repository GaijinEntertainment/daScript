#pragma once

namespace das {

    template <typename VecT, int rowC>
    struct Matrix {
        VecT    m[rowC];
    };

    typedef Matrix<float4,4> float4x4;
    typedef Matrix<float3,4> float3x4;
}

// use MAKE_TYPE_FACTORY out of namespaces. Some compilers are not happy otherwise
MAKE_TYPE_FACTORY(float4x4, das::float4x4)
MAKE_TYPE_FACTORY(float3x4, das::float3x4)
