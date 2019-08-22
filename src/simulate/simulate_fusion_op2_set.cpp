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

#include "daScript/simulate/simulate_fusion_op2.h"

namespace das {
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetAdd);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetSub);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetDiv);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetMod);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetMul);

    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinAnd);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinOr);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinXor);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinShl);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinShr);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinRotl);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinRotr);

    void createFusionEngine_op2_set()
    {
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetAdd);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetSub);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetDiv);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetMod);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetMul);

        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinAnd);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinOr);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinXor);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinShl);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinShr);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinRotl);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinRotr);
    }
}


