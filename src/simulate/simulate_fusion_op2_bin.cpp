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
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinAnd);
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinOr);
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinXor);
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinShl);
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinShr);
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinRotl);
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(BinRotr);

    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinAnd);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinOr);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinXor);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinShl);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinShr);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinRotl);
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(SetBinRotr);

    void createFusionEngine_op2_bin()
    {
        REGISTER_OP2_INTEGER_FUSION_POINT(BinAnd);
        REGISTER_OP2_INTEGER_FUSION_POINT(BinOr);
        REGISTER_OP2_INTEGER_FUSION_POINT(BinXor);
        REGISTER_OP2_INTEGER_FUSION_POINT(BinShl);
        REGISTER_OP2_INTEGER_FUSION_POINT(BinShr);
        REGISTER_OP2_INTEGER_FUSION_POINT(BinRotl);
        REGISTER_OP2_INTEGER_FUSION_POINT(BinRotr);

        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinAnd);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinOr);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinXor);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinShl);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinShr);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinRotl);
        REGISTER_OP2_INTEGER_FUSION_POINT(SetBinRotr);
    }
}


