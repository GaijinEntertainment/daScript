#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast_typedecl.h"
#include "daScript/simulate/simulate_fusion_op2.h"

// fake DAS_NODE to support boolean eval
#define DAS_NODE(TYPE,CTYPE)                                    \
    virtual bool evalBool ( das::Context & context ) override { \
        return compute(context);                                \
    }                                                           \
    virtual vec4f eval ( das::Context & context ) override {    \
        return cast<bool>::from(compute(context));              \
    }

namespace das {

    IMPLEMENT_OP2_SCALAR(Equ);
    IMPLEMENT_OP2_SCALAR(NotEqu);
    
    IMPLEMENT_OP2_NUMERIC(LessEqu);
    IMPLEMENT_OP2_NUMERIC(GtEqu);
    IMPLEMENT_OP2_NUMERIC(Gt);
    IMPLEMENT_OP2_NUMERIC(Less);

    void createFusionEngine_op2_bool()
    {
        REGISTER_OP2_SCALAR(Equ);
        REGISTER_OP2_SCALAR(NotEqu);

        REGISTER_OP2_NUMERIC(LessEqu);
        REGISTER_OP2_NUMERIC(GtEqu);
        REGISTER_OP2_NUMERIC(Gt);
        REGISTER_OP2_NUMERIC(Less);
    }
}


