#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das {

    struct SimPolicy_MathFloat {
        static __forceinline float Abs   ( float a, Context & )          { return v_extract_x(v_abs(v_splats(a))); }
        static __forceinline float Floor ( float a, Context & )          { return v_extract_x(v_floor(v_splats(a))); }
        static __forceinline float Ceil  ( float a, Context & )          { return v_extract_x(v_ceil(v_splats(a))); }
        static __forceinline float Sqrt  ( float a, Context & )          { return v_extract_x(v_sqrt_x(v_splats(a))); }

        static __forceinline float Exp   ( float a, Context & )          { return v_extract_x(v_exp(v_splats(a))); }
        static __forceinline float Log   ( float a, Context & )          { return v_extract_x(v_log(v_splats(a))); }
        static __forceinline float Exp2  ( float a, Context & )          { return v_extract_x(v_exp2(v_splats(a))); }
        static __forceinline float Log2  ( float a, Context & )          { return v_extract_x(v_log2_est_p5(v_splats(a))); }
        static __forceinline float Pow   ( float a, float b, Context & ) { return v_extract_x(v_pow(v_splats(a), v_splats(b))); }

        static __forceinline float Sin   ( float a, Context & )          { vec4f s,c; v_sincos4(v_splats(a), s, c);return v_extract_x(s); }
        static __forceinline float Cos   ( float a, Context & )          { vec4f s,c; v_sincos4(v_splats(a), s, c);return v_extract_x(c); }
        static __forceinline float Tan   ( float a, Context & )          { vec4f s,c; v_sincos4(v_splats(a), s, c);return v_extract_x(v_div_x(s,c)); }
        static __forceinline float ATan   ( float a, Context & )         { return v_extract_x(v_atan(v_splats(a))); }
        static __forceinline float ATan2 ( float a, float b, Context & ) { return v_extract_x(v_atan2(v_splats(a), v_splats(b))); }
        static __forceinline float ATan2_est ( float a, float b, Context & ) { return v_extract_x(v_atan2_est(v_splats(a), v_splats(b))); }
    };

    struct SimPolicy_MathVec {
        static __forceinline vec4f Abs   ( vec4f a, Context & )          { return v_abs(a); }
        static __forceinline vec4f Floor ( vec4f a, Context & )          { return v_floor(a); }
        static __forceinline vec4f Ceil  ( vec4f a, Context & )          { return v_ceil(a); }
        static __forceinline vec4f Sqrt  ( vec4f a, Context & )          { return v_sqrt4(a); }

        static __forceinline vec4f Exp   ( vec4f a, Context & )          { return v_exp(a); }
        static __forceinline vec4f Log   ( vec4f a, Context & )          { return v_log(a); }
        static __forceinline vec4f Exp2  ( vec4f a, Context & )          { return v_exp2(a); }
        static __forceinline vec4f Log2  ( vec4f a, Context & )          { return v_log2_est_p5(a); }
        static __forceinline vec4f Pow   ( vec4f a, vec4f b, Context & ) { return v_pow(a, b); }

        static __forceinline vec4f Sin ( vec4f a, Context & ) { vec4f s,c; v_sincos4(a, s, c);return s; }
        static __forceinline vec4f Cos ( vec4f a, Context & ) { vec4f s,c; v_sincos4(a, s, c);return c; }
        static __forceinline vec4f Tan ( vec4f a, Context & ) { vec4f s,c; v_sincos4(a, s, c);return v_div(s,c); }
        static __forceinline vec4f ATan( vec4f a, Context & ) { return v_atan(a); }
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

    template <typename TT>
    void addFunctionPow(Module & mod, const ModuleLibrary & lib) {
        //                                     policy       ret   arg1 arg2     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Exp<TT>,  TT,   TT> >("exp",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Log<TT>,  TT,   TT> >("log",   lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Exp2<TT>, TT,   TT> >("exp2",  lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Log2<TT>, TT,   TT> >("log2",  lib) );

        mod.addFunction( make_shared<BuiltInFn<Sim_Pow<TT>,  TT,   TT,   TT> >("pow",   lib) );
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
            //common
            addFunctionCommon<float>(*this, lib);
            addFunctionCommon<float2>(*this,lib);
            addFunctionCommon<float3>(*this,lib);
            addFunctionCommon<float4>(*this,lib);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Math,das);
