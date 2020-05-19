#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot_builtin_math.h"

#include "daScript/misc/performance_time.h"

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
    MATH_FUN_OP1(RSqrt)
    MATH_FUN_OP1(RSqrtEst)
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
        mod.addFunction( make_smart<BuiltInFn<Sim_Sin<TT>,        TT,   TT>        >("sin",       lib, "Sin") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Cos<TT>,        TT,   TT>        >("cos",       lib, "Cos") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Tan<TT>,        TT,   TT>        >("tan",       lib, "Tan") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ATan<TT>,       TT,   TT>        >("atan",      lib, "ATan") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ASin<TT>,       TT,   TT>        >("asin",      lib, "ASin") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ACos<TT>,       TT,   TT>        >("acos",      lib, "ACos") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ATan2<TT>,      TT,   TT,  TT>   >("atan2",     lib, "ATan2") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ATan2_est<TT>,  TT,   TT,  TT>   >("atan2_est", lib, "ATan2_est") );
    }

    template <typename TT>
    void addFunctionCommonTyped(Module & mod, const ModuleLibrary & lib) {
        mod.addFunction( make_smart<BuiltInFn<Sim_Min <TT>, TT,   TT,   TT>   >("min", lib, "Min") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Max <TT>, TT,   TT,   TT>   >("max", lib, "Max") );
    }

    template <typename TT>
    void addFunctionCommon(Module & mod, const ModuleLibrary & lib) {
        //                                     policy            ret   arg1     name
        mod.addFunction( make_smart<BuiltInFn<Sim_Abs<TT>,      TT,   TT>   >("abs",         lib, "Abs") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Floor<TT>,    TT,   TT>   >("floor",       lib, "Floor") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Ceil<TT>,     TT,   TT>   >("ceil",        lib, "Ceil") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Sqrt<TT>,     TT,   TT>   >("sqrt",        lib, "Sqrt") );
        mod.addFunction( make_smart<BuiltInFn<Sim_RSqrt<TT>,    TT,   TT>   >("rsqrt",       lib, "RSqrt") );
        mod.addFunction( make_smart<BuiltInFn<Sim_RSqrtEst<TT>, TT,   TT>   >("rsqrt_est",   lib, "RSqrtEst") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Sat<TT>,      TT,   TT>   >("saturate",    lib, "Sat") );
    }
    template <typename Ret, typename TT>
    void addFunctionCommonConversion(Module & mod, const ModuleLibrary & lib) {
        //                                     policy          ret    arg1     name
        mod.addFunction( make_smart<BuiltInFn<Sim_Floori<TT>, Ret,   TT>   >("floori",  lib, "Floori") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Ceili <TT>, Ret,   TT>   >("ceili",   lib, "Ceili") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Roundi<TT>, Ret,   TT>   >("roundi",  lib, "Roundi") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Trunci<TT>, Ret,   TT>   >("trunci",  lib, "Trunci") );
    }

    template <typename TT>
    void addFunctionPow(Module & mod, const ModuleLibrary & lib) {
        //                                     policy           ret   arg1   name
        mod.addFunction( make_smart<BuiltInFn<Sim_Exp<TT>,     TT,   TT> >("exp",      lib, "Exp") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Log<TT>,     TT,   TT> >("log",      lib, "Log") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Exp2<TT>,    TT,   TT> >("exp2",     lib, "Exp2") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Log2<TT>,    TT,   TT> >("log2",     lib, "Log2") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Rcp<TT>,     TT,   TT> >("rcp",      lib, "Rcp") );
        mod.addFunction( make_smart<BuiltInFn<Sim_RcpEst<TT>,  TT,   TT> >("rcp_est",  lib, "RcpEst") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Pow<TT>,  TT,   TT,   TT> >("pow",   lib, "Pow") );
    }

    template <typename TT>
    void addFunctionOp3(Module & mod, const ModuleLibrary & lib) {
        //                                     policy         ret arg1 arg2 arg3   name
        mod.addFunction( make_smart<BuiltInFn<Sim_Lerp<TT>,  TT, TT,  TT,  TT> >("lerp",  lib, "Lerp") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Clamp<TT>, TT, TT,  TT,  TT> >("clamp", lib, "Clamp") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Mad<TT>,   TT, TT,  TT,  TT> >("mad",   lib, "Mad") );
    }

    class Module_Math : public Module {
    public:
        Module_Math() : Module("math") {
            DAS_PROFILE_SECTION("Module_Math");

            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();

            // constants
            addConstant(*this,"PI",(float)M_PI);
            addConstant(*this,"FLT_EPSILON",FLT_EPSILON);

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

            addFunction( make_smart<BuiltInFn<Sim_MadS<float2>,   float2, float2,  float,  float2> >("mad", lib, "MadS") );
            addFunction( make_smart<BuiltInFn<Sim_MadS<float3>,   float3, float3,  float,  float3> >("mad", lib, "MadS") );
            addFunction( make_smart<BuiltInFn<Sim_MadS<float4>,   float4, float4,  float,  float4> >("mad", lib, "MadS") );

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

            addExtern<DAS_BIND_FUN(uint32_hash)>(*this, lib, "uint32_hash", SideEffects::none, "uint32_hash");
            addExtern<DAS_BIND_FUN(uint_noise1D)>(*this, lib, "uint_noise_1D", SideEffects::none, "uint_noise1D");
            addExtern<DAS_BIND_FUN(uint_noise2D_int2)>(*this, lib, "uint_noise_2D", SideEffects::none, "uint_noise2D_int2");
            addExtern<DAS_BIND_FUN(uint_noise3D_int3)>(*this, lib, "uint_noise_3D", SideEffects::none, "uint_noise3D_int3");

            addExternEx<float(float2,float2),DAS_BIND_FUN(dot2)>(*this, lib, "dot", SideEffects::none, "dot2");
            addExternEx<float(float3,float3),DAS_BIND_FUN(dot3)>(*this, lib, "dot", SideEffects::none, "dot3");
            addExternEx<float(float4,float4),DAS_BIND_FUN(dot4)>(*this, lib, "dot", SideEffects::none, "dot4");

            addExternEx<float3(float3,float3),DAS_BIND_FUN(cross3)>(*this, lib, "cross", SideEffects::none, "cross3");

            addExternEx<float2(float2),DAS_BIND_FUN(normalize2)>(*this, lib, "fast_normalize", SideEffects::none, "normalize2");
            addExternEx<float3(float3),DAS_BIND_FUN(normalize3)>(*this, lib, "fast_normalize", SideEffects::none, "normalize3");
            addExternEx<float4(float4),DAS_BIND_FUN(normalize4)>(*this, lib, "fast_normalize", SideEffects::none, "normalize4");

            addExternEx<float2(float2),DAS_BIND_FUN(safe_normalize2)>(*this, lib, "normalize", SideEffects::none, "safe_normalize2");
            addExternEx<float3(float3),DAS_BIND_FUN(safe_normalize3)>(*this, lib, "normalize", SideEffects::none, "safe_normalize3");
            addExternEx<float4(float4),DAS_BIND_FUN(safe_normalize4)>(*this, lib, "normalize", SideEffects::none, "safe_normalize4");

            addExternEx<float(float2),DAS_BIND_FUN(length2)>(*this, lib, "length", SideEffects::none, "length2");
            addExternEx<float(float3),DAS_BIND_FUN(length3)>(*this, lib, "length", SideEffects::none, "length3");
            addExternEx<float(float4),DAS_BIND_FUN(length4)>(*this, lib, "length", SideEffects::none, "length4");
            addExternEx<float(float2),DAS_BIND_FUN(invlength2)>(*this, lib, "inv_length", SideEffects::none,"invlength2");
            addExternEx<float(float3),DAS_BIND_FUN(invlength3)>(*this, lib, "inv_length", SideEffects::none,"invlength3");
            addExternEx<float(float4),DAS_BIND_FUN(invlength4)>(*this, lib, "inv_length", SideEffects::none,"invlength4");
            addExternEx<float(float2),DAS_BIND_FUN(invlengthSq2)>(*this, lib, "inv_length_sq", SideEffects::none, "invlengthSq2");
            addExternEx<float(float3),DAS_BIND_FUN(invlengthSq3)>(*this, lib, "inv_length_sq", SideEffects::none, "invlengthSq3");
            addExternEx<float(float4),DAS_BIND_FUN(invlengthSq4)>(*this, lib, "inv_length_sq", SideEffects::none, "invlengthSq4");
            addExternEx<float(float2),DAS_BIND_FUN(lengthSq2)>(*this, lib, "length_sq", SideEffects::none, "lengthSq2");
            addExternEx<float(float3),DAS_BIND_FUN(lengthSq3)>(*this, lib, "length_sq", SideEffects::none, "lengthSq3");
            addExternEx<float(float4),DAS_BIND_FUN(lengthSq4)>(*this, lib, "length_sq", SideEffects::none, "lengthSq4");
            addExternEx<float(float3,float3),DAS_BIND_FUN(distance3)>(*this, lib, "distance", SideEffects::none, "distance3");
            addExternEx<float(float3,float3),DAS_BIND_FUN(distanceSq3)>(*this, lib, "distance_sq", SideEffects::none, "distanceSq3");
            addExternEx<float(float3,float3),DAS_BIND_FUN(invdistance3)>(*this, lib, "inv_distance", SideEffects::none, "invdistance3");
            addExternEx<float(float3,float3),DAS_BIND_FUN(invdistanceSq3)>(*this, lib, "inv_distance_sq", SideEffects::none, "invdistanceSq3");

            //double functions
            addExtern<DAS_BIND_FUN(dabs)>(*this, lib, "abs",     SideEffects::none, "dabs");
            addExtern<DAS_BIND_FUN(dsqrt)>(*this, lib, "sqrt",   SideEffects::none, "dsqrt");
            addExtern<DAS_BIND_FUN(dexp)>(*this, lib, "exp",     SideEffects::none, "dexp");
            addExtern<DAS_BIND_FUN(drcp)>(*this, lib, "rcp",     SideEffects::none, "drcp");
            addExtern<DAS_BIND_FUN(dlog)>(*this, lib, "log",     SideEffects::none, "dlog");
            addExtern<DAS_BIND_FUN(dpow)>(*this, lib, "pow",     SideEffects::none, "dpow");
            addExtern<DAS_BIND_FUN(dexp2)>(*this, lib, "exp2",   SideEffects::none, "dexp2");
            addExtern<DAS_BIND_FUN(dlog2)>(*this, lib, "log2",   SideEffects::none, "dlog2");
            addExtern<DAS_BIND_FUN(dsin)>(*this, lib, "sin",     SideEffects::none, "dsin");
            addExtern<DAS_BIND_FUN(dcos)>(*this, lib, "cos",     SideEffects::none, "dcos");
            addExtern<DAS_BIND_FUN(dasin)>(*this, lib, "asin",   SideEffects::none, "dasin");
            addExtern<DAS_BIND_FUN(dacos)>(*this, lib, "acos",   SideEffects::none, "dacos");
            addExtern<DAS_BIND_FUN(dtan)>(*this, lib, "tan",     SideEffects::none, "dtan");
            addExtern<DAS_BIND_FUN(datan)>(*this, lib, "atan",   SideEffects::none, "datan");
            addExtern<DAS_BIND_FUN(datan2)>(*this, lib, "atan2", SideEffects::none, "datan2");

            addExtern<DAS_BIND_FUN(sincosF)>(*this, lib, "sincos", SideEffects::modifyArgument, "sincosF");
            addExtern<DAS_BIND_FUN(sincosD)>(*this, lib, "sincos", SideEffects::modifyArgument, "sincosD");

            addExternEx<float3(float3,float3),DAS_BIND_FUN(reflect)>(*this, lib, "reflect", SideEffects::none, "reflect");
            addExternEx<bool(float3,float3,float,float3&),DAS_BIND_FUN(refract)>(*this, lib, "refract", SideEffects::modifyArgument, "refract");

            addFunctionCommonConversion<int, float>  (*this, lib);
            addFunctionCommonConversion<int2, float2>(*this,lib);
            addFunctionCommonConversion<int3, float3>(*this,lib);
            addFunctionCommonConversion<int4, float4>(*this,lib);

            // lets make sure its all aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_math.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Math,das);
