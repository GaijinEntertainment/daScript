#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"

#include "daScript/simulate/simulate_fusion_op2.h"

namespace das {

    IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(Equ);
    IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(NotEqu);
    IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(LessEqu);
    IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(GtEqu);
    IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(Gt);
    IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(Less);

    void createFusionEngine_op2_bool()
    {
        REGISTER_OP2_NUMERIC_FUSION_POINT(Equ);
        REGISTER_OP2_NUMERIC_FUSION_POINT(NotEqu);
        REGISTER_OP2_NUMERIC_FUSION_POINT(LessEqu);
        REGISTER_OP2_NUMERIC_FUSION_POINT(GtEqu);
        REGISTER_OP2_NUMERIC_FUSION_POINT(Gt);
        REGISTER_OP2_NUMERIC_FUSION_POINT(Less);
    }
}


