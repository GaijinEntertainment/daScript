#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das {
    
    struct SimPolicy_MathFloat {
        static __forceinline float Sin   ( float a, Context & )          { return sinf(a); }
        static __forceinline float Cos   ( float a, Context & )          { return cosf(a); }
        static __forceinline float Tan   ( float a, Context & )          { return tanf(a); }
        static __forceinline float ATan2 ( float a, float b, Context & ) { return atan2(a,b); }
    };
    
    struct SimPolicy_MathVec {
        static __forceinline vec4f Sin ( vec4f a, Context & ) {
            return v_make_vec4f(sinf(v_extract_x(a)),
                                sinf(v_extract_y(a)),
                                sinf(v_extract_z(a)),
                                sinf(v_extract_w(a)));
        }
        static __forceinline vec4f Cos ( vec4f a, Context & ) {
            return v_make_vec4f(cosf(v_extract_x(a)),
                                cosf(v_extract_y(a)),
                                cosf(v_extract_z(a)),
                                cosf(v_extract_w(a)));
        }
        static __forceinline vec4f Tan ( vec4f a, Context & ) {
            return v_make_vec4f(tanf(v_extract_x(a)),
                                tanf(v_extract_y(a)),
                                tanf(v_extract_z(a)),
                                tanf(v_extract_w(a)));
        }
        static __forceinline vec4f ATan2 ( vec4f a, vec4f b, Context & ) {
            return v_make_vec4f(atan2f(v_extract_x(a),v_extract_x(b)),
                                atan2f(v_extract_y(a),v_extract_x(b)),
                                atan2f(v_extract_z(a),v_extract_x(b)),
                                atan2f(v_extract_w(a),v_extract_x(b)));
        }
    };
    
    template <> struct SimPolicy<float>  : SimPolicy_MathFloat {};
    template <> struct SimPolicy<float2> : SimPolicy_MathVec {};
    template <> struct SimPolicy<float3> : SimPolicy_MathVec {};
    template <> struct SimPolicy<float4> : SimPolicy_MathVec {};
    
    DEFINE_POLICY(Sin);
    DEFINE_POLICY(Cos);
    DEFINE_POLICY(Tan);
    DEFINE_POLICY(ATan2);
    
    IMPLEMENT_OP1_FUNCTION_POLICY(Sin,Float,float);
    IMPLEMENT_OP1_FUNCTION_POLICY(Cos,Float,float);
    IMPLEMENT_OP1_FUNCTION_POLICY(Tan,Float,float);
    IMPLEMENT_OP2_FUNCTION_POLICY(ATan2,Float,float);
    
#define DEFINE_EVAL_TRIG_POLICY(CTYPE)                  \
    IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(Sin, CTYPE);     \
    IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(Cos, CTYPE);     \
    IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(Tan, CTYPE);     \
    IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(ATan2, CTYPE);
    
    DEFINE_EVAL_TRIG_POLICY(float2);
    DEFINE_EVAL_TRIG_POLICY(float3);
    DEFINE_EVAL_TRIG_POLICY(float4);
    
    // trig types
    template <typename TT>
    void addFunctionTrig(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2     name
        mod.addFunction( make_shared<BuiltInFn<Sim_Sin<TT>,        TT,   TT>        >("sin",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Cos<TT>,        TT,   TT>        >("cos",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Tan<TT>,        TT,   TT>        >("tan",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_ATan2<TT>,      TT,   TT,  TT>   >("atan2",  lib) );
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
            addExtern<decltype(powf),  powf >(*this, lib, "pow",  false);
            addExtern<decltype(expf),  expf >(*this, lib, "exp",  false);
            addExtern<decltype(logf),  logf >(*this, lib, "log",  false);
            addExtern<decltype(sqrtf), sqrtf>(*this, lib, "sqrt", false);
            //common
            addExtern<decltype(fabsf),  fabsf>(*this, lib,  "fabs",  false);
            addExtern<decltype(ceilf),  ceilf>(*this, lib,  "ceil",  false);
            addExtern<decltype(floorf), floorf>(*this, lib, "floor", false);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Math,das);
