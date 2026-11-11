#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "simulate_fusion.h"

#if DAS_FUSION>=2

#include "daScript/simulate/sim_policy.h"
#include "simulate_fusion_op2.h"
#include "simulate_fusion_op2_vec_settings.h"

// fake DAS_NODE to support boolean eval
#undef DAS_NODE
#define DAS_NODE(TYPE,CTYPE)                                    \
    virtual bool evalBool ( das::Context & context ) override { \
        return compute(context);                                \
    }                                                           \
    DAS_EVAL_ABI virtual vec4f eval ( das::Context & context ) override {    \
        return cast<bool>::from(compute(context));              \
    }

namespace das {

    IMPLEMENT_OP2_NUMERIC_VEC(Equ);
    IMPLEMENT_OP2_NUMERIC_VEC(NotEqu);

    static constexpr FusionEntry fusionEntries_op2_bool_vec[] = {
        REGISTER_OP2_NUMERIC_VEC(Equ)
        REGISTER_OP2_NUMERIC_VEC(NotEqu)
    };

    FUSION_TABLE(fusionTable_op2_bool_vec, fusionEntries_op2_bool_vec)
}

#endif


