#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast_typedecl.h"
#include "daScript/simulate/simulate_fusion_op2.h"


namespace das {

    IMPLEMENT_OP2_NUMERIC(Equ);
    IMPLEMENT_OP2_NUMERIC(NotEqu);
    IMPLEMENT_OP2_NUMERIC(LessEqu);
    IMPLEMENT_OP2_NUMERIC(GtEqu);
    IMPLEMENT_OP2_NUMERIC(Gt);
    IMPLEMENT_OP2_NUMERIC(Less);

    void createFusionEngine_op2_bool()
    {
        REGISTER_OP2_NUMERIC(Equ);
        REGISTER_OP2_NUMERIC(NotEqu);
        REGISTER_OP2_NUMERIC(LessEqu);
        REGISTER_OP2_NUMERIC(GtEqu);
        REGISTER_OP2_NUMERIC(Gt);
        REGISTER_OP2_NUMERIC(Less);
    }
}


