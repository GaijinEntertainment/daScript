#pragma once

#include "daScript/simulate/runtime_matrices.h"

namespace das {
    void float4x4_identity ( float4x4 & mat );
    void float3x4_identity ( float3x4 & mat );
    void float3x3_identity ( float3x3 & mat );
    float4x4 float4x4_identity_m ( void );
    float3x4 float3x4_identity_m ( void );
    float3x3 float3x3_identity_m ( void );
    float4x4 float4x4_translation(float3 xyz);
    float3x3 float3x3_mul(const float3x3 &a, const float3x3 &b);
    float3x4 float3x4_mul(const float3x4 &a, const float3x4 &b);
    float4x4 float4x4_mul(const float4x4 &a, const float4x4 &b);
    float4x4 float4x4_transpose ( const float4x4 & src );
    float4x4 float4x4_inverse( const float4x4 & src);
    float3x4 float3x4_inverse ( const float3x4 & src );
    float3x3 float3x3_inverse( const float3x3 & src);
    float4x4 float4x4_from_float34 ( const float3x4 & mat );
    float3x3 float3x3_from_float44 ( const float4x4 & mat );
    float3x3 float3x3_from_float34 ( const float3x4 & mat );
    float3x4 float3x4_from_float44 ( const float4x4 & mat );

    float4x4 float4x4_orthonormal_inverse( const float4x4 & src);
    float4x4 float4x4_persp_forward(float wk, float hk, float zn, float zf);
    float4x4 float4x4_persp_reverse(float wk, float hk, float zn, float zf);
    float4x4 float4x4_look_at(float4 eye, float4 at, float4 up);
    float4x4 float4x4_compose(float4 pos, float4 rot, float4 scale);
    void float4x4_decompose(const float4x4 & mat, float3 & pos, float4 & rot, float4 & scale);

    float4 un_quat_from_unit_arc(float3 v0, float3 v1);
    float4 un_quat_from_unit_vec_ang(float3 v, float ang);
    float4 un_quat(const float4x4 & m);

    float4 quat_mul(float4 q1, float4 q2);
    float3 quat_mul_vec(float4 q, float3 v);
    float4 quat_conjugate(float4 q);

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

    inline float3 float3x3_mul_vec3(const float3x3 &a, float3 b) {
        mat33f va;  va.col0 = a.m[0]; va.col1 = a.m[1]; va.col2 = a.m[2];
        return v_mat33_mul_vec3(va, b);
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

    __forceinline bool float3x4_equ ( const float3x4 & a, const float3x4 & b ) {
        return memcmp(&a,&b,sizeof(float3x4))==0;
    }
    __forceinline bool float3x4_nequ ( const float3x4 & a, const float3x4 & b ) {
        return memcmp(&a,&b,sizeof(float3x4))!=0;
    }
    __forceinline bool float3x3_equ ( const float3x3 & a, const float3x3 & b ) {
        return memcmp(&a,&b,sizeof(float3x3))==0;
    }
    __forceinline bool float3x3_nequ ( const float3x3 & a, const float3x3 & b ) {
        return memcmp(&a,&b,sizeof(float3x3))!=0;
    }
}

