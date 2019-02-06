#include "daScript/misc/platform.h"

#include <dag_noise/dag_uint_noise.h>

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das {

    struct SimPolicy_MathFloat {
        static __forceinline float Abs   ( float a, Context & )          { return v_extract_x(v_abs(v_splats(a))); }
        static __forceinline float Floor ( float a, Context & )          { return v_extract_x(v_floor(v_splats(a))); }
        static __forceinline float Ceil  ( float a, Context & )          { return v_extract_x(v_ceil(v_splats(a))); }
        static __forceinline float Sqrt  ( float a, Context & )          { return v_extract_x(v_sqrt_x(v_splats(a))); }

        static __forceinline int Trunci ( float a, Context & )          { return v_extract_xi(v_cvt_vec4i(v_splats(a))); }
        static __forceinline int Roundi ( float a, Context & )          { return v_extract_xi(v_cvt_roundi(v_splats(a))); }
        static __forceinline int Floori ( float a, Context & )          { return v_extract_xi(v_cvt_floori(v_splats(a))); }
        static __forceinline int Ceili  ( float a, Context & )          { return v_extract_xi(v_cvt_ceili(v_splats(a))); }

        static __forceinline float Exp   ( float a, Context & )          { return v_extract_x(v_exp(v_splats(a))); }
        static __forceinline float Log   ( float a, Context & )          { return v_extract_x(v_log(v_splats(a))); }
        static __forceinline float Exp2  ( float a, Context & )          { return v_extract_x(v_exp2(v_splats(a))); }
        static __forceinline float Log2  ( float a, Context & )          { return v_extract_x(v_log2_est_p5(v_splats(a))); }
        static __forceinline float Pow   ( float a, float b, Context & ) { return v_extract_x(v_pow(v_splats(a), v_splats(b))); }

        static __forceinline float Sin   ( float a, Context & )          { vec4f s,c; v_sincos4(v_splats(a), s, c);return v_extract_x(s); }
        static __forceinline float Cos   ( float a, Context & )          { vec4f s,c; v_sincos4(v_splats(a), s, c);return v_extract_x(c); }
        static __forceinline float Tan   ( float a, Context & )          { vec4f s,c; v_sincos4(v_splats(a), s, c);return v_extract_x(v_div_x(s,c)); }
        static __forceinline float ATan   ( float a, Context & )         { return v_extract_x(v_atan(v_splats(a))); }
        static __forceinline float ASin   ( float a, Context & )         { return v_extract_x(v_asin_x(v_splats(a))); }
        static __forceinline float ACos   ( float a, Context & )         { return v_extract_x(v_acos_x(v_splats(a))); }
        static __forceinline float ATan2 ( float a, float b, Context & ) { return v_extract_x(v_atan2(v_splats(a), v_splats(b))); }
        static __forceinline float ATan2_est ( float a, float b, Context & ) { return v_extract_x(v_atan2_est(v_splats(a), v_splats(b))); }
    };

    struct SimPolicy_MathVec {
        static __forceinline vec4f Abs   ( vec4f a, Context & )          { return v_abs(a); }
        static __forceinline vec4f Floor ( vec4f a, Context & )          { return v_floor(a); }
        static __forceinline vec4f Ceil  ( vec4f a, Context & )          { return v_ceil(a); }
        static __forceinline vec4f Sqrt  ( vec4f a, Context & )          { return v_sqrt4(a); }

        static __forceinline vec4f Trunci ( vec4f a, Context & )          { return v_cast_vec4f(v_cvt_vec4i(a)); }
        static __forceinline vec4f Roundi ( vec4f a, Context & )          { return v_cast_vec4f(v_cvt_roundi(a)); }
        static __forceinline vec4f Floori ( vec4f a, Context & )          { return v_cast_vec4f(v_cvt_floori(a)); }
        static __forceinline vec4f Ceili  ( vec4f a, Context & )          { return v_cast_vec4f(v_cvt_ceili(a)); }

        static __forceinline vec4f Exp   ( vec4f a, Context & )          { return v_exp(a); }
        static __forceinline vec4f Log   ( vec4f a, Context & )          { return v_log(a); }
        static __forceinline vec4f Exp2  ( vec4f a, Context & )          { return v_exp2(a); }
        static __forceinline vec4f Log2  ( vec4f a, Context & )          { return v_log2_est_p5(a); }
        static __forceinline vec4f Pow   ( vec4f a, vec4f b, Context & ) { return v_pow(a, b); }

        static __forceinline vec4f Sin ( vec4f a, Context & ) { vec4f s,c; v_sincos4(a, s, c);return s; }
        static __forceinline vec4f Cos ( vec4f a, Context & ) { vec4f s,c; v_sincos4(a, s, c);return c; }
        static __forceinline vec4f Tan ( vec4f a, Context & ) { vec4f s,c; v_sincos4(a, s, c);return v_div(s,c); }
        static __forceinline vec4f ATan( vec4f a, Context & ) { return v_atan(a); }
        static __forceinline vec4f ASin( vec4f a, Context & ) { return v_asin(a); }
        static __forceinline vec4f ACos( vec4f a, Context & ) { return v_acos(a); }
        static __forceinline vec4f ATan2 ( vec4f a, vec4f b, Context & ) { return v_atan2(a, b); }
        static __forceinline vec4f ATan2_est ( vec4f a, vec4f b, Context & ) { return v_atan2_est(a, b); }
    };

    template <> struct SimPolicy<float>  : SimPolicy_MathFloat {};
    template <> struct SimPolicy<float2> : SimPolicy_MathVec {};
    template <> struct SimPolicy<float3> : SimPolicy_MathVec {};
    template <> struct SimPolicy<float4> : SimPolicy_MathVec {};

#define MATH_FUN_OP1(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP1_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP1_INT(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP1_FUNCTION_POLICY_EX(fun,Int,int,Float,float);\
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP2_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float4);

    //common
    MATH_FUN_OP1(Abs)
    MATH_FUN_OP1(Floor)
    MATH_FUN_OP1(Ceil)
    MATH_FUN_OP1(Sqrt)

    MATH_FUN_OP1_INT(Trunci)
    MATH_FUN_OP1_INT(Floori)
    MATH_FUN_OP1_INT(Ceili)
    MATH_FUN_OP1_INT(Roundi)

    //exp
    MATH_FUN_OP1(Exp)
    MATH_FUN_OP1(Log)
    MATH_FUN_OP1(Exp2)
    MATH_FUN_OP1(Log2)
    MATH_FUN_OP2(Pow)

    //trig
    MATH_FUN_OP1(Sin)
    MATH_FUN_OP1(Cos)
    MATH_FUN_OP1(Tan)
    MATH_FUN_OP1(ATan)
    MATH_FUN_OP1(ASin)
    MATH_FUN_OP1(ACos)

    MATH_FUN_OP2(ATan2)
    MATH_FUN_OP2(ATan2_est)

    // trig types
    template <typename TT>
    void addFunctionTrig(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Sin<TT>,        TT,   TT>        >("sin",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Cos<TT>,        TT,   TT>        >("cos",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Tan<TT>,        TT,   TT>        >("tan",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_ATan<TT>,       TT,   TT>        >("atan",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_ASin<TT>,       TT,   TT>        >("asin",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_ACos<TT>,       TT,   TT>        >("acos",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_ATan2<TT>,      TT,   TT,  TT>   >("atan2",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_ATan2_est<TT>,  TT,   TT,  TT>   >("atan2_est",  lib) );
    }

    template <typename TT>
    void addFunctionCommon(Module & mod, const ModuleLibrary & lib) {
        //                                     policy       ret   arg1 arg2     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Abs<TT>,  TT,   TT>   >("abs",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Floor<TT>,TT,   TT>   >("floor",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Ceil<TT>, TT,   TT>   >("ceil",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sqrt<TT>, TT,   TT>   >("sqrt",   lib) );
    }
    template <typename Ret, typename TT>
    void addFunctionCommonConversion(Module & mod, const ModuleLibrary & lib) {
        //                                     policy            ret    arg1 arg2     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Floori<TT>, Ret,   TT>   >("floori",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Ceili <TT>, Ret,   TT>   >("ceili",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Roundi<TT>, Ret,   TT>   >("roundi",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Trunci<TT>, Ret,   TT>   >("trunci",  lib) );
    }

    template <typename TT>
    void addFunctionPow(Module & mod, const ModuleLibrary & lib) {
        //                                     policy       ret   arg1 arg2     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Exp<TT>,  TT,   TT> >("exp",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Log<TT>,  TT,   TT> >("log",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Exp2<TT>, TT,   TT> >("exp2",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Log2<TT>, TT,   TT> >("log2",  lib) );

        mod.addFunction( make_shared<BuiltInFn<Sim_Pow<TT>,  TT,   TT,   TT> >("pow",   lib) );
    }

    __forceinline unsigned int uint_noise2D_int2(das::int2 pos, unsigned int seed)
    {
      return uint_noise2D(pos.x, pos.y, seed);
    }
    __forceinline unsigned int uint_noise3D_int3(das::int3 pos, unsigned int seed)
    {
      return uint_noise3D(pos.x, pos.y, pos.z, seed);
    }

    __forceinline float length2(float2 a){vec4f v = v_ldu_half(&a.x); v = v_mul(v,v); return v_extract_x(v_sqrt_x(v_add_x(v, v_rot_1(v))));}
    __forceinline float length3(float3 a){return v_extract_x(v_length3_x(v_ldu(&a.x)));}
    __forceinline float length4(float4 a){return v_extract_x(v_length4_x(v_ldu(&a.x)));}
    __forceinline float lengthSq2(float2 a){vec4f v = v_ldu_half(&a.x); v = v_mul(v,v); return v_extract_x(v_add_x(v, v_rot_1(v)));}
    __forceinline float lengthSq3(float3 a){return v_extract_x(v_length3_sq_x(v_ldu(&a.x)));}
    __forceinline float lengthSq4(float4 a){return v_extract_x(v_length4_sq_x(v_ldu(&a.x)));}
    __forceinline float distance3(float3 a, float3 b){return v_extract_x(v_length3_x(v_sub(v_ldu(&a.x), v_ldu(&b.x))));}
    __forceinline float distanceSq3(float3 a, float3 b){return v_extract_x(v_length3_sq_x(v_sub(v_ldu(&a.x), v_ldu(&b.x))));}
    __forceinline float dot2(float2 a, float2 b){vec4f v = v_mul(v_ldu_half(&a.x), v_ldu_half(&b.x)); return v_extract_x(v_add_x(v, v_rot_1(v)));}
    __forceinline float dot3(float3 a, float3 b){return v_extract_x(v_dot3_x(v_ldu(&a.x), v_ldu(&b.x)));}
    __forceinline float dot4(float4 a, float4 b){return v_extract_x(v_dot4_x(v_ldu(&a.x), v_ldu(&b.x)));}

    __forceinline float3 cross3(float3 a, float3 b){vec4f v = v_cross3(v_ldu(&a.x), v_ldu(&b.x));return *(float3*)&v;}

    class Module_Math : public Module {
    public:
        Module_Math() : Module("math") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // trigonometry functions
            addFunctionTrig<float>(*this, lib);
            addFunctionTrig<float2>(*this,lib);
            addFunctionTrig<float3>(*this,lib);
            addFunctionTrig<float4>(*this,lib);
            // exp functions
            addFunctionPow<float>(*this, lib);
            addFunctionPow<float2>(*this,lib);
            addFunctionPow<float3>(*this,lib);
            addFunctionPow<float4>(*this,lib);
            //common
            addFunctionCommon<float>(*this, lib);
            addFunctionCommon<float2>(*this,lib);
            addFunctionCommon<float3>(*this,lib);
            addFunctionCommon<float4>(*this,lib);
            addExtern<DAS_BIND_FUN(uint32_hash)>(*this, lib, "uint32_hash", SideEffects::none);
            addExtern<DAS_BIND_FUN(uint_noise1D)>(*this, lib, "uint_noise1D", SideEffects::none);
            addExtern<DAS_BIND_FUN(uint_noise2D_int2)>(*this, lib, "uint_noise2D", SideEffects::none);
            addExtern<DAS_BIND_FUN(uint_noise3D_int3)>(*this, lib, "uint_noise3D", SideEffects::none);

            addExtern<DAS_BIND_FUN(dot2)>(*this, lib, "dot", SideEffects::none);
            addExtern<DAS_BIND_FUN(dot3)>(*this, lib, "dot", SideEffects::none);
            addExtern<DAS_BIND_FUN(dot4)>(*this, lib, "dot", SideEffects::none);

            addExtern<DAS_BIND_FUN(cross3)>(*this, lib, "cross", SideEffects::none);

            addExtern<DAS_BIND_FUN(length2)>(*this, lib, "length", SideEffects::none);
            addExtern<DAS_BIND_FUN(length3)>(*this, lib, "length", SideEffects::none);
            addExtern<DAS_BIND_FUN(length4)>(*this, lib, "length", SideEffects::none);
            addExtern<DAS_BIND_FUN(lengthSq2)>(*this, lib, "lengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(lengthSq3)>(*this, lib, "lengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(lengthSq4)>(*this, lib, "lengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(distance3)>(*this, lib, "distance", SideEffects::none);
            addExtern<DAS_BIND_FUN(distanceSq3)>(*this, lib, "distanceSq", SideEffects::none);

            addFunctionCommonConversion<int, float>(*this, lib);
            addFunctionCommonConversion<int2, float2>(*this,lib);
            addFunctionCommonConversion<int3, float3>(*this,lib);
            addFunctionCommonConversion<int4, float4>(*this,lib);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Math,das);
