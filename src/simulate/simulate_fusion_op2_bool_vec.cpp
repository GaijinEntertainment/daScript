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

    IMPLEMENT_BOOL_OP2_VEC_NUMERIC_FUSION_POINT(Equ);
    IMPLEMENT_BOOL_OP2_VEC_NUMERIC_FUSION_POINT(NotEqu);

    void createFusionEngine_op2_bool_vec()
    {
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(Equ);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(NotEqu);
    }
}


