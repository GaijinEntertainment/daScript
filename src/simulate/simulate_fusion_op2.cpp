#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/ast/ast.h"
#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/simulate/simulate_fusion_op2.h"

namespace das {
    IMPLEMENT_OP2_NUMERIC_FUSION_POINT(Add);
    IMPLEMENT_OP2_NUMERIC_FUSION_POINT(Sub);
    IMPLEMENT_OP2_NUMERIC_FUSION_POINT(Div);
    IMPLEMENT_OP2_NUMERIC_FUSION_POINT(Mod);
    IMPLEMENT_OP2_NUMERIC_FUSION_POINT(Mul);

    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetAdd);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetSub);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetDiv);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetMod);
    IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(SetMul);

    void createFusionEngine_op2()
    {
        REGISTER_OP2_NUMERIC_FUSION_POINT(Add);
        REGISTER_OP2_NUMERIC_FUSION_POINT(Sub);
        REGISTER_OP2_NUMERIC_FUSION_POINT(Div);
        REGISTER_OP2_NUMERIC_FUSION_POINT(Mod);
        REGISTER_OP2_NUMERIC_FUSION_POINT(Mul);

        REGISTER_OP2_NUMERIC_FUSION_POINT(SetAdd);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetSub);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetDiv);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetMod);
        REGISTER_OP2_NUMERIC_FUSION_POINT(SetMul);
    }
}


