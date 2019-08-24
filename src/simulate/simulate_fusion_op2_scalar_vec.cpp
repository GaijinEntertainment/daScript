#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast_typedecl.h"
#include "daScript/simulate/simulate_fusion_op2.h"
#include "daScript/simulate/simulate_fusion_op2_vec_settings.h"

namespace das {

    /*
#undef FUSION_OP_PTR_VALUE_RIGHT
#define FUSION_OP_PTR_VALUE_RIGHT(CTYPE,expr)   (v_ld_x((const float *)(expr)))

    IMPLEMENT_OP2_VEC_NUMERIC_FUSION_POINT(MulVecScal);
    IMPLEMENT_OP2_VEC_NUMERIC_FUSION_POINT(DivVecScal);

#undef FUSION_OP_PTR_VALUE_RIGHT
#define FUSION_OP_PTR_VALUE_RIGHT(CTYPE,expr)   (v_ldu((const float *)(expr)))
#undef FUSION_OP_PTR_VALUE_LEFT
#define FUSION_OP_PTR_VALUE_LEFT(CTYPE,expr)    (v_ld_x((const float *)(expr)))

    IMPLEMENT_OP2_VEC_NUMERIC_FUSION_POINT(MulScalVec);
    IMPLEMENT_OP2_VEC_NUMERIC_FUSION_POINT(DivScalVec);
    */

    void createFusionEngine_op2_scalar_vec()
    {
        /*
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(MulVecScal);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(DivVecScal);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(MulScalVec);
        REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(DivScalVec);
        */
    }
}


