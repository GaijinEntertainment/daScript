#pragma once

#include <dag_noise/dag_uint_noise.h>

namespace das {
    __forceinline unsigned int uint_noise2D_int2(das::int2 pos, unsigned int seed)
    {
        return uint_noise2D(pos.x, pos.y, seed);
    }
    __forceinline unsigned int uint_noise3D_int3(das::int3 pos, unsigned int seed)
    {
        return uint_noise3D(pos.x, pos.y, pos.z, seed);
    }

    __forceinline float length2(vec4f a){vec4f v = a; v = v_mul(v,v); return v_extract_x(v_sqrt_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float length3(vec4f a){return v_extract_x(v_length3_x(a));}
    __forceinline float length4(vec4f a){return v_extract_x(v_length4_x(a));}
    __forceinline float lengthSq2(vec4f a){vec4f v = a; v = v_mul(v,v); return v_extract_x(v_add_x(v, v_rot_1(v)));}
    __forceinline float lengthSq3(vec4f a){return v_extract_x(v_length3_sq_x(a));}
    __forceinline float lengthSq4(vec4f a){return v_extract_x(v_length4_sq_x(a));}

#if defined(_MSC_VER) && !defined(__clang__) && (_MSC_VER==1923)
    // workaround for internal compiler error with MSVC 2019
    __forceinline float invlength2(vec4f a) { return 1.0f / length2(a); }
    __forceinline float invlength3(vec4f a) { return 1.0f / length3(a); }
    __forceinline float invlength4(vec4f a) { return 1.0f / length4(a); }
#else
    __forceinline float invlength2(vec4f a){vec4f v = a; v = v_mul(v,v); return v_extract_x(v_rsqrt_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float invlength3(vec4f a){return v_extract_x(v_rsqrt_x(v_length3_sq_x(a)));}
    __forceinline float invlength4(vec4f a){return v_extract_x(v_rsqrt_x(v_length4_sq_x(a)));}
#endif

    __forceinline float invlengthSq2(vec4f a){vec4f v = a; v = v_mul(v,v); return v_extract_x(v_rcp_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float invlengthSq3(vec4f a){return v_extract_x(v_rcp_x(v_length3_sq_x(a)));}
    __forceinline float invlengthSq4(vec4f a){return v_extract_x(v_rcp_x(v_length4_sq_x(a)));}

    __forceinline float distance2     (vec4f a, vec4f b){return length2(v_sub(a,b));}
    __forceinline float invdistance2  (vec4f a, vec4f b){return invlength2(v_sub(a,b));}
    __forceinline float distanceSq2   (vec4f a, vec4f b){return lengthSq2(v_sub(a,b));}
    __forceinline float invdistanceSq2(vec4f a, vec4f b){return invlengthSq2(v_sub(a,b));}

    __forceinline float distance3     (vec4f a, vec4f b){return v_extract_x(v_length3_x(v_sub(a, b)));}
    __forceinline float invdistance3  (vec4f a, vec4f b){return v_extract_x(v_rcp_x(v_length3_x(v_sub(a, b))));}
    __forceinline float distanceSq3   (vec4f a, vec4f b){return v_extract_x(v_length3_sq_x(v_sub(a, b)));}
    __forceinline float invdistanceSq3(vec4f a, vec4f b){return v_extract_x(v_rcp_x(v_length3_sq_x(v_sub(a,b))));}

    __forceinline float distance4     (vec4f a, vec4f b){return v_extract_x(v_length4_x(v_sub(a, b)));}
    __forceinline float invdistance4  (vec4f a, vec4f b){return v_extract_x(v_rcp_x(v_length4_x(v_sub(a, b))));}
    __forceinline float distanceSq4   (vec4f a, vec4f b){return v_extract_x(v_length4_sq_x(v_sub(a, b)));}
    __forceinline float invdistanceSq4(vec4f a, vec4f b){return v_extract_x(v_rcp_x(v_length4_sq_x(v_sub(a,b))));}

    __forceinline float dot2(vec4f a, vec4f b){vec4f v = v_mul(a, b); return v_extract_x(v_add_x(v, v_rot_1(v)));}
    __forceinline float dot3(vec4f a, vec4f b){return v_extract_x(v_dot3_x(a, b));}
    __forceinline float dot4(vec4f a, vec4f b){return v_extract_x(v_dot4_x(a, b));}

    __forceinline vec4f normalize2(vec4f a){vec4f v = v_mul(a, a); return v_div(a, v_splat_x(v_sqrt_x(v_add_x(v, v_rot_1(v))))); }
    __forceinline vec4f normalize3(vec4f a){return v_norm3(a); }
    __forceinline vec4f normalize4(vec4f a){return v_norm4(a); }
    __forceinline vec4f safe_normalize2(vec4f a){vec4f v = v_mul(a, a); return v_remove_not_finite(v_div(a, v_splat_x(v_sqrt_x(v_add_x(v, v_rot_1(v)))))); }
    __forceinline vec4f safe_normalize3(vec4f a){return v_norm3_safe(a); }
    __forceinline vec4f safe_normalize4(vec4f a){return v_norm4_safe(a); }

    __forceinline vec4f cross3(vec4f a, vec4f b){vec4f v = v_cross3(a,b); return v;}

#if defined(__GNUC__) || defined(__clang__)
#if !defined(__clang__)
#define DAS_FINITE_MATH __attribute__((optimize("no-finite-math-only")))
#else
#define DAS_FINITE_MATH
#endif
#if defined(__clang__) && !defined(__arm64__)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif
#if __clang_major__ < 12 && defined(__clang__) && defined(__FAST_MATH__)
    //unfortunately older clang versions do not work with float_control
    __forceinline DAS_FINITE_MATH bool   fisnan(float  a) { volatile float b = a; return b != a; }
    __forceinline DAS_FINITE_MATH bool   disnan(double  a) { volatile double b = a; return b != a; }
#else
    __forceinline DAS_FINITE_MATH bool   fisnan(float  a) { return __builtin_isnan(a); }
    __forceinline DAS_FINITE_MATH bool   disnan(double  a) { return __builtin_isnan(a); }
#endif
    __forceinline DAS_FINITE_MATH bool   fisfinite(float  a) { return !__builtin_isinf(a); }
    __forceinline DAS_FINITE_MATH bool   disfinite(double  a) { return !__builtin_isinf(a); }
#undef DAS_FINITE_MATH
#if defined(__clang__) && !defined(__arm64__)
#pragma float_control(pop)
#endif
#else
    //msvc just does not optimize fast math
    __forceinline bool   fisfinite(float  a) { return !isinf(a); }
    __forceinline bool   fisnan(float  a) { return isnan(a); }
    __forceinline bool   disfinite(double  a) { return !isinf(a); }
    __forceinline bool   disnan(double  a) { return isnan(a); }
#endif


    __forceinline double dsign (double a){return (a == 0) ? 0 : (a > 0) ? 1 : -1;}
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

    __forceinline void sincosF ( float a, float & sv, float & cv ) {
        vec4f s,c;
        v_sincos_x(v_set_x(a), s, c);
        sv = v_extract_x(s);
        cv = v_extract_x(c);
    }

    __forceinline void sincosD ( double a, double & sv, double & cv ) {
        sv = sin(a);
        cv = cos(a);
    }

    // def reflect(v,n:float3)
    //  return v - float(2.) * dot(v, n) * n
    __forceinline vec4f reflect ( vec4f v, vec4f n ) {
        vec4f t = v_mul(v_dot3(v,n),n);
        return v_sub(v, v_add(t,t));
    }

    // def refract(v,n:float3;nint:float;outRefracted:float3&)
    // let dt = dot(v,n)
    // let discr = 1. - nint*nint*(1.-dt*dt)
    // if discr > 0.
    //     outRefracted = nint*(v-n*dt)-n*sqrt(discr)
    //     return true
    // return false
    __forceinline vec4f refract(vec4f v, vec4f n, float nint) {
        vec4f dtv = v_dot3(v, n);
        float dt = v_extract_x(dtv);
        float discr = 1.0f - nint*nint*(1.0f - dt*dt);
        if (discr > 0.0f) {
            vec4f nintv = v_splats(nint);
            vec4f sqrt_discr = v_perm_xxxx(v_sqrt_x(v_set_x(discr)));
            return v_sub(v_mul(nintv, v_sub(v, v_mul(n, dtv))), v_mul(n, sqrt_discr));
        } else {
            return v_zero();
        }
    }

    __forceinline uint32_t pack_float_to_byte ( float4 value ) {
        return v_float_to_byte(value);
    }

    __forceinline float4 unpack_byte_to_float ( uint32_t value ) {
        return v_byte_to_float(value);
    }

    __forceinline vec4f v_gather ( const void * _ptr, vec4f index ) {
        // read 4 floats from memory, using 4 uint32_t indices
        #if defined(__AVX2__)
            // note: this is not faster than the scalar version
            return (_mm_i32gather_ps((const float *) _ptr, v_cast_vec4i(index), 4));
        #else
            auto ptr = (const float *) _ptr;
            auto i = v_cast_vec4i(index);
            return v_make_vec4f(
                ptr[uint32_t(v_extract_xi(i))],
                ptr[uint32_t(v_extract_yi(i))],
                ptr[uint32_t(v_extract_zi(i))],
                ptr[uint32_t(v_extract_wi(i))]
            );
        #endif
    }

    __forceinline void v_scatter ( void * _ptr, vec4f index, vec4f value ) {
        // write 4 floats to memory, using 4 uint32_t indices
        auto ptr = (float *) _ptr;
        auto i = v_cast_vec4i(index);
        ptr[uint32_t(v_extract_xi(i))] = v_extract_x(value);
        ptr[uint32_t(v_extract_yi(i))] = v_extract_y(value);
        ptr[uint32_t(v_extract_zi(i))] = v_extract_z(value);
        ptr[uint32_t(v_extract_wi(i))] = v_extract_w(value);
    }

    __forceinline void v_scatter_mask ( void * _ptr, vec4f index, vec4f value, vec4f mask_v ) {
        // write 4 floats to memory, using 4 uint32_t indices, but only for floats, where value[i]!=mask_v[i]
        auto ptr = (float *) _ptr;
        auto mask = v_cmp_eqi(v_cast_vec4i(mask_v), v_cast_vec4i(value));
        auto i = v_cast_vec4i(index);
        if ( !v_extract_xi(mask) ) ptr[uint32_t(v_extract_xi(i))] = v_extract_x(value);
        if ( !v_extract_yi(mask) ) ptr[uint32_t(v_extract_yi(i))] = v_extract_y(value);
        if ( !v_extract_zi(mask) ) ptr[uint32_t(v_extract_zi(i))] = v_extract_z(value);
        if ( !v_extract_wi(mask) ) ptr[uint32_t(v_extract_wi(i))] = v_extract_w(value);
    }

    __forceinline void v_store_mask ( void * _ptr, vec4f value, vec4f mask_v ) {
        // write 4 floats to memory, using 4 uint32_t indices, but only for floats, where value[i]!=mask_v[i]
        auto ptr = (float *) _ptr;
        auto mask = v_cast_vec4f(v_cmp_eqi(v_cast_vec4i(mask_v), v_cast_vec4i(value)));
        v_stu(ptr, v_or(v_and(mask,v_ldu(ptr)),v_andnot(mask, value)));
    }

    __forceinline void v_gather_scatter ( void * _to, vec4f to_index, const void * _from, vec4f from_index ) {
        // read 4 floats from memory, using 4 uint32_t indices and then write them to memory
        auto ptr = (uint32_t *) _from;
        auto to =  (uint32_t *) _to;
        auto i = v_cast_vec4i(from_index);
        auto j = v_cast_vec4i(to_index);
        to[uint32_t(v_extract_xi(j))] = ptr[uint32_t(v_extract_xi(i))];
        to[uint32_t(v_extract_yi(j))] = ptr[uint32_t(v_extract_yi(i))];
        to[uint32_t(v_extract_zi(j))] = ptr[uint32_t(v_extract_zi(i))];
        to[uint32_t(v_extract_wi(j))] = ptr[uint32_t(v_extract_wi(i))];
    }

    __forceinline void v_gather_scatter_mask ( void * _to, vec4f to_index, const void * _from, vec4f from_index, vec4f mask_v ) {
        // read 4 floats from memory, using 4 uint32_t indices and then write them to memory, but only for floats, where value[i]!=mask_v[i]
        auto ptr = (uint32_t *) _from;
        auto to =  (uint32_t *) _to;
        auto i = v_cast_vec4i(from_index);
        auto j = v_cast_vec4i(to_index);
        auto m = v_cast_vec4i(mask_v);
        auto X = ptr[uint32_t(v_extract_xi(i))];    if ( X != uint32_t(v_extract_xi(m)) ) to[uint32_t(v_extract_xi(j))] = X;
        auto Y = ptr[uint32_t(v_extract_yi(i))];    if ( Y != uint32_t(v_extract_yi(m)) ) to[uint32_t(v_extract_yi(j))] = Y;
        auto Z = ptr[uint32_t(v_extract_zi(i))];    if ( Z != uint32_t(v_extract_zi(m)) ) to[uint32_t(v_extract_zi(j))] = Z;
        auto W = ptr[uint32_t(v_extract_wi(i))];    if ( W != uint32_t(v_extract_wi(m)) ) to[uint32_t(v_extract_wi(j))] = W;
    }

    __forceinline vec4f v_gather_store_mask ( void * _to, const void * _from, vec4f from_index, vec4f mask_v ) {
        // read 4 floats from memory, using 4 uint32_t indices and then write them to memory, but only for floats, where value[i]!=mask_v[i]
        auto from = (const float *) _from;
        auto to =  (float *) _to;
        auto value = v_gather(from, from_index);
        v_store_mask ( to, value, mask_v );
        return value;
    }

    __forceinline void v_gather_store_stride ( void * _to, int stride, const void * _from, vec4f from_index ) {
        // read 4 floats from memory, using 4 uint32_t indices and then write them to memory, but only for floats, where value[i]!=mask_v[i]
        auto ptr = (uint32_t *) _from;
        auto to =  (uint32_t *) _to;
        auto i = v_cast_vec4i(from_index);
        to[       0] = ptr[uint32_t(v_extract_xi(i))];
        to[  stride] = ptr[uint32_t(v_extract_yi(i))];
        to[2*stride] = ptr[uint32_t(v_extract_zi(i))];
        to[3*stride] = ptr[uint32_t(v_extract_wi(i))];
    }
}
