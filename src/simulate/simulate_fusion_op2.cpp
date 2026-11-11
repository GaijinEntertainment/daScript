#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "simulate_fusion.h"

#if DAS_FUSION

#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast.h"
#include "simulate_fusion_op2.h"

namespace das {

    IMPLEMENT_OP2_NUMERIC(Add);
    IMPLEMENT_OP2_NUMERIC(Sub);
    IMPLEMENT_OP2_NUMERIC(Div);
    IMPLEMENT_OP2_NUMERIC(Mod);
    IMPLEMENT_OP2_NUMERIC(Mul);

    static constexpr FusionEntry fusionEntries_op2[] = {
        REGISTER_OP2_NUMERIC(Add)
        REGISTER_OP2_NUMERIC(Sub)
        REGISTER_OP2_NUMERIC(Div)
        REGISTER_OP2_NUMERIC(Mod)
        REGISTER_OP2_NUMERIC(Mul)
    };

    FUSION_TABLE(fusionTable_op2, fusionEntries_op2)
}

#endif



