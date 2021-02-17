#pragma once

namespace das {
    const float R_PI = 3.14159265358979323846f;

    const int32_t LCG_RAND_MAX = 32767;

    __forceinline vec4i _randomSeed ( int32_t seed ) {
        return v_make_vec4i(seed,seed+1,seed+2,seed+3);
    }

    __forceinline int4 randomSeed ( int32_t seed ) {
        vec4i res = _randomSeed(seed);
        return cast<int4>::to(v_cast_vec4f(res));
    }

    __forceinline DAS_SUPRESS_UB int32_t randomInt ( int4 & seed ) {
        seed.x = 214013*seed.x+2531011;
        return (seed.x>>16) & LCG_RAND_MAX;
    }

    __forceinline vec4i _randomInt4 ( int4 & seed ) {
        vec4i newSeed = v_addi(v_muli(v_ldui(&seed.x),v_splatsi(214013)),v_splatsi(2531011));
        v_stu_w(&seed, newSeed);
        vec4i ns16 = v_srli(newSeed, 16);
        vec4i mask =  v_splatsi(LCG_RAND_MAX);
        vec4i res = v_andi(ns16,mask);
        return res;
    }

    __forceinline int4 randomInt4 ( int4 & seed ) {
        vec4i res = _randomInt4(seed);
        return cast<int4>::to(v_cast_vec4f(res));
    }

    __forceinline float randomFloat ( int4 & seed ) {
        return float(randomInt(seed)) * (1.0f/float(LCG_RAND_MAX));
    }

    __forceinline vec4f _randomFloat4 ( int4 & seed ) {
        vec4i randi = _randomInt4(seed);
        vec4f rand4 = v_cvt_vec4f(randi);
        return v_mul ( rand4, v_splats(1.0f/LCG_RAND_MAX) );
    }

    __forceinline float4 randomFloat4 ( int4 & seed ) {
        vec4f res = _randomFloat4(seed);
        return *((float4 *)&res);
    }

    __forceinline float3 randomUnitVector( int4 & seed ) {
        float4 zz = randomFloat4(seed);
        float z = zz.x * 2.f - 1.f;
        float a = zz.y * ( 2.f * R_PI );
        float r = v_extract_x(v_sqrt_x(v_splats(1.f - z * z)));
        vec4f s,c;
        v_sincos4(v_splats(a), s, c);
        float x = v_extract_x(s);
        float y = v_extract_x(c);
        return { r * x, r * y, z };
    }

    __forceinline float3 randomInUnitSphere ( int4 & seed ) {
        for ( ;; ) {
            vec4f r = _randomFloat4(seed);
            vec4f p = v_sub(v_add(r,r), v_splats(1.f));
            if ( v_extract_x(v_length3_sq(p)) <= 1.0f ) {
                return *((float3 *)&p);
            }
        }
    }

    __forceinline float3 randomInUnitDisk ( int4 & seed ) {
        for ( ;; ) {
            vec4f r = _randomFloat4(seed);
            union { vec4f vec; float3 f3; } p;
            p.vec = v_sub(v_add(r,r), v_splats(1.f));
            p.f3.z = 0.0f;
            if ( v_extract_x(v_length3_sq(p.vec)) <= 1.0f ) {
                return p.f3;
            }
        }
    }
}

