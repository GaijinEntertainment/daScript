#pragma once

#include "daScript/simulate/runtime_matrices.h"

namespace das {
    void float4x4_identity ( float4x4 & mat );
    void float3x4_identity ( float3x4 & mat );
    float4x4 float4x4_translation(float3 xyz);
    float3x4 float3x4_mul(const float3x4 &a, const float3x4 &b);
    float4x4 float4x4_mul(const float4x4 &a, const float4x4 &b);
    float4x4 float4x4_transpose ( const float4x4 & src );
    float3x4 float3x4_inverse ( const float3x4 & src );

    inline float3 float3x4_mul_vec3p(const float3x4 &a, float3 b) {
        mat44f va;
        v_mat44_make_from_43cu(va, &a.m[0].x);
        return v_mat44_mul_vec3p(va, v_ldu(&b.x));
    }

    inline float4 float4x4_mul_vec4(const float4x4 &a, float4 b) {
        mat44f va;
        memcpy(&va,&a,sizeof(float4x4));
        return v_mat44_mul_vec4(va, b);
    }

    inline float3 rotate(const float3x4 &a, float3 b) {
        mat44f va;
        v_mat44_make_from_43cu(va, &a.m[0].x);
        return v_mat44_mul_vec3v(va, v_ldu(&b.x));
    }

    __forceinline bool float4x4_equ ( const float4x4 & a, const float4x4 & b ) {
        return memcmp(&a,&b,sizeof(float4x4))==0;
    }
    __forceinline bool float4x4_nequ ( const float4x4 & a, const float4x4 & b ) {
        return memcmp(&a,&b,sizeof(float4x4))!=0;
    }
}

