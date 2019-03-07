#include <math.h>
#include "daScript/misc/platform.h"

#include <dag_noise/dag_uint_noise.h>

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das {

    const float R_PI = 3.14159265358979323846f;

    const int32_t LCG_RAND_MAX = 32767;

    __forceinline vec4i _randomSeed ( int32_t seed ) {
        return v_make_vec4i(seed,seed+1,seed+2,seed+3);
    }

    __forceinline int4 randomSeed ( int32_t seed ) {
        vec4i res = _randomSeed(seed);
        return *((int4 *)&res);
    }

    __forceinline int32_t randomInt ( int4 & seed ) {
        seed.x = 214013*seed.x+2531011;
        return (seed.x>>16) & LCG_RAND_MAX;
    }

    __forceinline vec4i _randomInt4 ( int4 & seed ) {
        vec4i newSeed = v_addi(v_muli(v_ldu_w(&seed.x),v_splatsi(214013)),v_splatsi(2531011));
        v_stu_w(&seed, newSeed);
        vec4i ns16 = v_srli(newSeed, 16);
        vec4i mask =  v_splatsi(LCG_RAND_MAX);
        vec4i res = v_andi(ns16,mask);
        return res;
    }

    __forceinline int4 randomInt4 ( int4 & seed ) {
        vec4i res = _randomInt4(seed);
        return *((int4 *)&res);
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

    class Module_Random : public Module {
    public:
        Module_Random() : Module("random") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // random functions
            addExtern<DAS_BIND_FUN(randomSeed)>(*this, lib, "randomSeed", SideEffects::none);
            addExtern<DAS_BIND_FUN(randomInt )>(*this, lib, "randomInt",  SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomInt4)>(*this, lib, "randomInt4", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomFloat )>(*this, lib, "randomFloat",  SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomFloat4)>(*this, lib, "randomFloat4", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomUnitVector  )>(*this, lib, "randomUnitVector",   SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomInUnitSphere)>(*this, lib, "randomInUnitSphere", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(randomInUnitDisk  )>(*this, lib, "randomInUnitDisk",   SideEffects::modifyArgument);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Random,das);
