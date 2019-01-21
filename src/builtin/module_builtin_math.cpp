#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das {
    
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
    
    template <> struct SimPolicy<float2> : SimPolicy_MathVec {};
    template <> struct SimPolicy<float3> : SimPolicy_MathVec {};
    template <> struct SimPolicy<float4> : SimPolicy_MathVec {};
    
    IMPLEMENT_OP1_FUNCTION_POLICY(Sin,Float,float);
    IMPLEMENT_OP1_FUNCTION_POLICY(Cos,Float,float);
    IMPLEMENT_OP1_FUNCTION_POLICY(Tan,Float,float);
    IMPLEMENT_OP2_FUNCTION_POLICY(ATan2,Float,float);
    
    DEFINE_EVAL_TRIG_POLICY(float2);
    DEFINE_EVAL_TRIG_POLICY(float3);
    DEFINE_EVAL_TRIG_POLICY(float4);
    
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
