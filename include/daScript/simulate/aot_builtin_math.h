#pragma once

#include <math.h>

#include <dag_noise/dag_uint_noise.h>

namespace das {
        __forceinline unsigned int uint_noise2D_int2(das::int2 pos, unsigned int seed) {
      return uint_noise2D(pos.x, pos.y, seed);
    }
    __forceinline unsigned int uint_noise3D_int3(das::int3 pos, unsigned int seed) {
      return uint_noise3D(pos.x, pos.y, pos.z, seed);
    }

    __forceinline float length2(float2 a){vec4f v = v_ldu_half(&a.x); v = v_mul(v,v); return v_extract_x(v_sqrt_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float length3(float3 a){return v_extract_x(v_length3_x(v_ldu(&a.x)));}
    __forceinline float length4(float4 a){return v_extract_x(v_length4_x(v_ldu(&a.x)));}
    __forceinline float lengthSq2(float2 a){vec4f v = v_ldu_half(&a.x); v = v_mul(v,v); return v_extract_x(v_add_x(v, v_rot_1(v)));}
    __forceinline float lengthSq3(float3 a){return v_extract_x(v_length3_sq_x(v_ldu(&a.x)));}
    __forceinline float lengthSq4(float4 a){return v_extract_x(v_length4_sq_x(v_ldu(&a.x)));}

    __forceinline float invlength2(float2 a){vec4f v = v_ldu_half(&a.x); v = v_mul(v,v); return v_extract_x(v_rsqrt_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float invlength3(float3 a){return v_extract_x(v_rsqrt_x(v_length3_sq_x(v_ldu(&a.x))));}
    __forceinline float invlength4(float4 a){return v_extract_x(v_rsqrt_x(v_length4_sq_x(v_ldu(&a.x))));}

    __forceinline float invlengthSq2(float2 a){vec4f v = v_ldu_half(&a.x); v = v_mul(v,v); return v_extract_x(v_rcp_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float invlengthSq3(float3 a){return v_extract_x(v_rcp_x(v_length3_sq_x(v_ldu(&a.x))));}
    __forceinline float invlengthSq4(float4 a){return v_extract_x(v_rcp_x(v_length4_sq_x(v_ldu(&a.x))));}

    __forceinline float distance3(float3 a, float3 b){return v_extract_x(v_length3_x(v_sub(v_ldu(&a.x), v_ldu(&b.x))));}
    __forceinline float invdistance3(float3 a, float3 b){return v_extract_x(v_rcp_x(v_length3_x(v_sub(v_ldu(&a.x), v_ldu(&b.x)))));}
    __forceinline float distanceSq3(float3 a, float3 b){return v_extract_x(v_length3_sq_x(v_sub(v_ldu(&a.x), v_ldu(&b.x))));}
    __forceinline float invdistanceSq3(float3 a, float3 b){return v_extract_x(v_rcp_x(v_length3_sq_x(v_sub(v_ldu(&a.x), v_ldu(&b.x)))));}
    __forceinline float dot2(float2 a, float2 b){vec4f v = v_mul(v_ldu_half(&a.x), v_ldu_half(&b.x)); return v_extract_x(v_add_x(v, v_rot_1(v)));}
    __forceinline float dot3(float3 a, float3 b){return v_extract_x(v_dot3_x(v_ldu(&a.x), v_ldu(&b.x)));}
    __forceinline float dot4(float4 a, float4 b){return v_extract_x(v_dot4_x(v_ldu(&a.x), v_ldu(&b.x)));}

    __forceinline float3 cross3(float3 a, float3 b){vec4f v = v_cross3(v_ldu(&a.x), v_ldu(&b.x));return *(float3*)&v;}
    __forceinline double dabs  (double a){return fabs(a);}
    __forceinline double dsqrt (double a){return sqrt(a);}
    __forceinline double dexp  (double a){return exp(a);}
    __forceinline double drcp  (double a){return 1.0 / a;}
    __forceinline double dlog  (double a){return log(a);}
    __forceinline double dpow  (double a, double b){return pow(a,b);}
    __forceinline double dexp2 (double a){return exp2(a);}
    __forceinline double dlog2 (double a){return log2(a);}
    __forceinline double dsin  (double a){return sin(a);}
    __forceinline double dcos  (double a){return cos(a);}
    __forceinline double dasin (double a){return asin(a);}
    __forceinline double dacos (double a){return acos(a);}
    __forceinline double dtan  (double a){return tan(a);}
    __forceinline double datan (double a){return atan(a);}
    __forceinline double datan2(double a,double b){return atan2(a,b);}

    // __forceinline float2 normalize2(float2 a){vec4f v = v_norm2(v_ldu_half(&a.x)); return *(float2*)&v;}
    __forceinline float3 normalize3(float3 a){vec4f v = v_norm3(v_ldu(&a.x)); return *(float3*)&v;}
    __forceinline float4 normalize4(float4 a){vec4f v = v_norm4(v_ldu(&a.x)); return *(float4*)&v;}

    __forceinline void sincosF ( float a, float & sv, float & cv ) {
        vec4f s,c;
        v_sincos4(v_splats(a), s, c);
        sv = v_extract_x(s);
        cv = v_extract_x(c);
    }

    __forceinline void sincosD ( double a, double & sv, double & cv ) {
        sv = sin(a);
        cv = cos(a);
    }

    // def reflect(v,n:float3)
    //  return v - float(2.) * dot(v, n) * n
    __forceinline float3 reflect ( float3 vv, float3 nn ) {
        vec4f v = v_ldu(&vv.x);
        vec4f n = v_ldu(&nn.x);
        vec4f t = v_mul(v_dot3(v,n),n);
        vec4f res = v_sub(v, v_add(t,t));
        return *((float3 *)&res);
    }

    // def refract(v,n:float3;nint:float;outRefracted:float3&)
    // let dt = dot(v,n)
    // let discr = 1. - nint*nint*(1.-dt*dt)
    // if discr > 0.
    //     outRefracted = nint*(v-n*dt)-n*sqrt(discr)
    //     return true
    // return false
    __forceinline bool refract ( float3 vv, float3 nn, float nint, float3 & outRefracted ) {
        vec4f v = v_ldu(&vv.x);
        vec4f n = v_ldu(&nn.x);
        vec4f dtv = v_dot3(v,n);
        float dt = v_extract_x(dtv);
        float discr = 1.0f - nint*nint*(1.0f - dt*dt);
        if ( discr > 0.0f ) {
            vec4f nintv = v_splats(nint);
            vec4f sqrt_discr = v_perm_xxxx(v_sqrt_x(v_splats(discr)));
            vec4f outR = v_sub(v_mul(nintv,v_sub(v,v_mul(n,dtv))), v_mul(n,sqrt_discr));
            outRefracted = *((float3 *)&outR);
            return true;
        } else {
            return false;
        }
    }
}

