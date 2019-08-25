#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast_typedecl.h"
#include "daScript/simulate/simulate_fusion_op2.h"
#include "daScript/simulate/simulate_fusion_op2_vec_settings.h"

// fake DAS_NODE to support regular eval
#define DAS_NODE(TYPE,CTYPE)                                    \
    virtual vec4f eval ( das::Context & context ) override {    \
        return compute(context);                                \
    }

namespace das {

#define FUSION_OP_PTR_VALUE_RIGHT(CTYPE,expr)   (v_ld_x((const float *)(expr)))

    IMPLEMENT_OP2_NUMERIC_VEC(MulVecScal);
    IMPLEMENT_OP2_NUMERIC_VEC(DivVecScal);

#define FUSION_OP_PTR_VALUE_RIGHT(CTYPE,expr)   (v_ldu((const float *)(expr)))
#define FUSION_OP_PTR_VALUE_LEFT(CTYPE,expr)    (v_ld_x((const float *)(expr)))

    IMPLEMENT_OP2_NUMERIC_VEC(MulScalVec);
    IMPLEMENT_OP2_NUMERIC_VEC(DivScalVec);

    void createFusionEngine_op2_scalar_vec()
    {
        REGISTER_OP2_NUMERIC_VEC(MulVecScal);
        REGISTER_OP2_NUMERIC_VEC(DivVecScal);
        REGISTER_OP2_NUMERIC_VEC(MulScalVec);
        REGISTER_OP2_NUMERIC_VEC(DivScalVec);
    }
}


