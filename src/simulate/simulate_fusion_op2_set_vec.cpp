#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"

#undef DAS_NODE
#define DAS_NODE(TYPE,CTYPE)                                    \
    virtual vec4f eval ( das::Context & context ) override {    \
        compute(context);                                       \
        return v_zero();                                        \
    }

#include "daScript/simulate/simulate_fusion_vec_settings.h"
#include "daScript/simulate/simulate_fusion_op2.h"

namespace das {

    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetAdd);
    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetSub);
    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetDiv);
    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetMod);
    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetMul);

    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(SetBinAnd);
    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(SetBinOr);
    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(SetBinXor);
    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(SetBinShl);
    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(SetBinShr);

#undef FUSION_OP_PTR_VALUE_RIGHT
#define FUSION_OP_PTR_VALUE_RIGHT(CTYPE,expr)   (v_ld_x((const float *)(expr)))

    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetDivScal);
    IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(SetMulScal);

    void createFusionEngine_op2_set_vec()
    {
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetAdd);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetSub);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetDiv);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetMod);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetMul);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetDivScal);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(SetMulScal);

        REGISTER_OP2_VEC_INTEGER_FUSION_POINT(SetBinAnd);
        REGISTER_OP2_VEC_INTEGER_FUSION_POINT(SetBinOr);
        REGISTER_OP2_VEC_INTEGER_FUSION_POINT(SetBinXor);
        REGISTER_OP2_VEC_INTEGER_FUSION_POINT(SetBinShl);
        REGISTER_OP2_VEC_INTEGER_FUSION_POINT(SetBinShr);
    }
}


