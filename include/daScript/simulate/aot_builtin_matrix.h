#pragma once

#include "daScript/simulate/runtime_matrices.h"

namespace das {
    void float4x4_identity ( float4x4 & mat );
    void float3x4_identity ( float3x4 & mat );
    float4x4 float_4x4_translation(float3 xyz);
    float3x4 float3x4_mul(const float3x4 &a, const float3x4 &b);

    inline float3 float3x4_mul_vec3p(const float3x4 &a, float3 b) {
        mat44f va;
        v_mat44_make_from_43cu(va, &a.m[0].x);
        return v_mat44_mul_vec3p(va, v_ldu(&b.x));
    }
    inline float3 rotate(const float3x4 &a, float3 b) {
        mat44f va;
        v_mat44_make_from_43cu(va, &a.m[0].x);
        return v_mat44_mul_vec3v(va, v_ldu(&b.x));
    }
}

