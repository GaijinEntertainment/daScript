#include <math.h>
#include "daScript/misc/platform.h"

#include <dag_noise/dag_uint_noise.h>

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

#include "daScript/simulate/sim_policy.h"

namespace das {
#define MATH_FUN_OP1(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP1_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP1_INT(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP1_FUNCTION_POLICY_EX(fun,Int,int32_t,Float,float);\
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP2_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2I(fun)\
      IMPLEMENT_OP2_FUNCTION_POLICY(fun,Int,int32_t);\
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, int2);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, int3);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, int4);

#define MATH_FUN_OP3(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP3_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2A(fun)                              \
    MATH_FUN_OP2(fun);                                  \
    MATH_FUN_OP2I(fun);                                 \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,UInt,uint32_t);   \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,Int64,int64_t);   \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,UInt64,uint64_t); \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,Double,double);

    // everything
    MATH_FUN_OP2A(Min)
    MATH_FUN_OP2A(Max)

    //common
    MATH_FUN_OP1(Abs)
    MATH_FUN_OP1(Floor)
    MATH_FUN_OP1(Ceil)
    MATH_FUN_OP1(Sqrt)
    MATH_FUN_OP1(Sat)
    MATH_FUN_OP3(Clamp)
    MATH_FUN_OP3(Mad)
    MATH_FUN_OP3(Lerp)

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
    MATH_FUN_OP1(Rcp)
    MATH_FUN_OP1(RcpEst)

    //trig
    MATH_FUN_OP1(Sin)
    MATH_FUN_OP1(Cos)
    MATH_FUN_OP1(Tan)
    MATH_FUN_OP1(ATan)
    MATH_FUN_OP1(ASin)
    MATH_FUN_OP1(ACos)

    MATH_FUN_OP2(ATan2)
    MATH_FUN_OP2(ATan2_est)

    DEFINE_POLICY(MadS)     // vector_a*scalar_b + vector_c
    IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(MadS,float2);
    IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(MadS,float3);
    IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(MadS,float4);

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
    void addFunctionCommonTyped(Module & mod, const ModuleLibrary & lib) {
        mod.addFunction( make_shared<BuiltInFn<Sim_Min <TT>, TT,   TT,   TT>   >("min",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Max <TT>, TT,   TT,   TT>   >("max",    lib) );
    }

    template <typename TT>
    void addFunctionCommon(Module & mod, const ModuleLibrary & lib) {
        //                                     policy        ret   arg1     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Abs<TT>,  TT,   TT>   >("abs",       lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Floor<TT>,TT,   TT>   >("floor",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Ceil<TT>, TT,   TT>   >("ceil",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sqrt<TT>, TT,   TT>   >("sqrt",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sat<TT>,  TT,   TT>   >("saturate",  lib) );
    }
    template <typename Ret, typename TT>
    void addFunctionCommonConversion(Module & mod, const ModuleLibrary & lib) {
        //                                     policy          ret    arg1     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Floori<TT>, Ret,   TT>   >("floori",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Ceili <TT>, Ret,   TT>   >("ceili",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Roundi<TT>, Ret,   TT>   >("roundi",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Trunci<TT>, Ret,   TT>   >("trunci",  lib) );
    }

    template <typename TT>
    void addFunctionPow(Module & mod, const ModuleLibrary & lib) {
        //                                     policy           ret   arg1   name
        mod.addFunction( make_shared<BuiltInFn<Sim_Exp<TT>,     TT,   TT> >("exp",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Log<TT>,     TT,   TT> >("log",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Exp2<TT>,    TT,   TT> >("exp2",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Log2<TT>,    TT,   TT> >("log2",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Rcp<TT>,     TT,   TT> >("rcp",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_RcpEst<TT>,  TT,   TT> >("rcp_est",  lib) );

        mod.addFunction( make_shared<BuiltInFn<Sim_Pow<TT>,  TT,   TT,   TT> >("pow",   lib) );
    }

    template <typename TT>
    void addFunctionOp3(Module & mod, const ModuleLibrary & lib) {
        //                                     policy         ret arg1 arg2 arg3   name
        mod.addFunction( make_shared<BuiltInFn<Sim_Lerp<TT>,  TT, TT,  TT,  TT> >("lerp",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Clamp<TT>, TT, TT,  TT,  TT> >("clamp",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Mad<TT>,   TT, TT,  TT,  TT> >("mad",    lib) );
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
            // op3
            addFunctionOp3<float >(*this,lib);
            addFunctionOp3<float2>(*this,lib);
            addFunctionOp3<float3>(*this,lib);
            addFunctionOp3<float4>(*this,lib);

            addFunction( make_shared<BuiltInFn<Sim_MadS<float2>,   float2, float2,  float,  float2> >("mad",    lib) );
            addFunction( make_shared<BuiltInFn<Sim_MadS<float3>,   float3, float3,  float,  float3> >("mad",    lib) );
            addFunction( make_shared<BuiltInFn<Sim_MadS<float4>,   float4, float4,  float,  float4> >("mad",    lib) );

            //common
            addFunctionCommon<float>(*this, lib);
            addFunctionCommon<float2>(*this,lib);
            addFunctionCommon<float3>(*this,lib);
            addFunctionCommon<float4>(*this,lib);

            addFunctionCommonTyped<int32_t>(*this, lib);
            addFunctionCommonTyped<int2>(*this, lib);
            addFunctionCommonTyped<int3>(*this, lib);
            addFunctionCommonTyped<int4>(*this, lib);

            addFunctionCommonTyped<uint32_t>(*this, lib);
            //addFunctionCommonTyped<uint2>(*this, lib);
            //addFunctionCommonTyped<uint3>(*this, lib);
            //addFunctionCommonTyped<uint4>(*this, lib);

            addFunctionCommonTyped<float>(*this, lib);
            addFunctionCommonTyped<float2>(*this, lib);
            addFunctionCommonTyped<float3>(*this, lib);
            addFunctionCommonTyped<float4>(*this, lib);

            addFunctionCommonTyped<double>(*this, lib);

            addFunctionCommonTyped<int64_t>(*this, lib);
            addFunctionCommonTyped<uint64_t>(*this, lib);

            addExtern<DAS_BIND_FUN(uint32_hash)>(*this, lib, "uint32_hash", SideEffects::none);
            addExtern<DAS_BIND_FUN(uint_noise1D)>(*this, lib, "uint_noise1D", SideEffects::none);
            addExtern<DAS_BIND_FUN(uint_noise2D_int2)>(*this, lib, "uint_noise2D", SideEffects::none);
            addExtern<DAS_BIND_FUN(uint_noise3D_int3)>(*this, lib, "uint_noise3D", SideEffects::none);

            addExtern<DAS_BIND_FUN(dot2)>(*this, lib, "dot", SideEffects::none);
            addExtern<DAS_BIND_FUN(dot3)>(*this, lib, "dot", SideEffects::none);
            addExtern<DAS_BIND_FUN(dot4)>(*this, lib, "dot", SideEffects::none);

            addExtern<DAS_BIND_FUN(cross3)>(*this, lib, "cross", SideEffects::none);

            // addExtern<DAS_BIND_FUN(normalize2)>(*this, lib, "normalize", SideEffects::none);
            addExtern<DAS_BIND_FUN(normalize3)>(*this, lib, "normalize", SideEffects::none);
            addExtern<DAS_BIND_FUN(normalize4)>(*this, lib, "normalize", SideEffects::none);

            addExtern<DAS_BIND_FUN(length2)>(*this, lib, "length", SideEffects::none);
            addExtern<DAS_BIND_FUN(length3)>(*this, lib, "length", SideEffects::none);
            addExtern<DAS_BIND_FUN(length4)>(*this, lib, "length", SideEffects::none);
            addExtern<DAS_BIND_FUN(invlength2)>(*this, lib, "invLength", SideEffects::none);
            addExtern<DAS_BIND_FUN(invlength3)>(*this, lib, "invLength", SideEffects::none);
            addExtern<DAS_BIND_FUN(invlength4)>(*this, lib, "invLength", SideEffects::none);
            addExtern<DAS_BIND_FUN(invlengthSq2)>(*this, lib, "invLengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(invlengthSq3)>(*this, lib, "invLengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(invlengthSq4)>(*this, lib, "invLengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(lengthSq2)>(*this, lib, "lengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(lengthSq3)>(*this, lib, "lengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(lengthSq4)>(*this, lib, "lengthSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(distance3)>(*this, lib, "distance", SideEffects::none);
            addExtern<DAS_BIND_FUN(distanceSq3)>(*this, lib, "distanceSq", SideEffects::none);
            addExtern<DAS_BIND_FUN(invdistance3)>(*this, lib, "invDistance", SideEffects::none);
            addExtern<DAS_BIND_FUN(invdistanceSq3)>(*this, lib, "invDistanceSq", SideEffects::none);

            //double functions
            addExtern<DAS_BIND_FUN(dabs)>(*this, lib, "abs", SideEffects::none);
            addExtern<DAS_BIND_FUN(dsqrt)>(*this, lib, "sqrt", SideEffects::none);
            addExtern<DAS_BIND_FUN(dexp)>(*this, lib, "exp", SideEffects::none);
            addExtern<DAS_BIND_FUN(drcp)>(*this, lib, "rcp", SideEffects::none);
            addExtern<DAS_BIND_FUN(dlog)>(*this, lib, "log", SideEffects::none);
            addExtern<DAS_BIND_FUN(dpow)>(*this, lib, "pow", SideEffects::none);
            addExtern<DAS_BIND_FUN(dexp2)>(*this, lib, "exp2", SideEffects::none);
            addExtern<DAS_BIND_FUN(dlog2)>(*this, lib, "log2", SideEffects::none);
            addExtern<DAS_BIND_FUN(dsin)>(*this, lib, "sin", SideEffects::none);
            addExtern<DAS_BIND_FUN(dcos)>(*this, lib, "cos", SideEffects::none);
            addExtern<DAS_BIND_FUN(dasin)>(*this, lib, "asin", SideEffects::none);
            addExtern<DAS_BIND_FUN(dacos)>(*this, lib, "acos", SideEffects::none);
            addExtern<DAS_BIND_FUN(dtan)>(*this, lib, "tan", SideEffects::none);
            addExtern<DAS_BIND_FUN(datan)>(*this, lib, "atan", SideEffects::none);
            addExtern<DAS_BIND_FUN(datan2)>(*this, lib, "atan2", SideEffects::none);

            addExtern<DAS_BIND_FUN(sincosF)>(*this, lib, "sincos", SideEffects::modifyArgument);
            addExtern<DAS_BIND_FUN(sincosD)>(*this, lib, "sincos", SideEffects::modifyArgument);

            addExtern<DAS_BIND_FUN(reflect)>(*this, lib, "reflect", SideEffects::none);
            addExtern<DAS_BIND_FUN(refract)>(*this, lib, "refract", SideEffects::modifyArgument);

            addFunctionCommonConversion<int, float>(*this, lib);
            addFunctionCommonConversion<int2, float2>(*this,lib);
            addFunctionCommonConversion<int3, float3>(*this,lib);
            addFunctionCommonConversion<int4, float4>(*this,lib);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Math,das);
