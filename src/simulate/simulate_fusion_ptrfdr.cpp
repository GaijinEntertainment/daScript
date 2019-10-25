#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"

#if DAS_FUSION

#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast_typedecl.h"
#include "daScript/simulate/simulate_fusion_op1.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    struct SimNode_Op1PtrFdr : SimNode_Op1Fusion {
        virtual SimNode * visit(SimVisitor & vis) override {
            V_BEGIN();
            string name = op;
            name += getSimSourceName(subexpr.type);
            if (baseType != Type::none) {
                vis.op(name.c_str(), getTypeBaseSize(baseType), das_to_string(baseType));
            } else {
                vis.op(name.c_str());
            }
            subexpr.visit(vis);
            V_ARG(offset);
            V_END();
        }
        uint32_t  offset;
    };

    /* PtrFdr Any */

#undef IMPLEMENT_OP1_SET_SETUP_NODE
#define IMPLEMENT_OP1_SET_SETUP_NODE(result,node) \
    auto rn = (SimNode_Op1PtrFdr *)result; \
    auto sn = (SimNode_PtrFieldDeref *)node; \
    rn->offset = sn->offset; \
    rn->baseType = Type::none;

#undef IMPLEMENT_ANY_OP1_NODE
#define IMPLEMENT_ANY_OP1_NODE(INLINE,OPNAME,TYPE,CTYPE,COMPUTE) \
    struct SimNode_Op1##COMPUTE : SimNode_Op1PtrFdr { \
        DAS_PTR_NODE; \
        __forceinline char * compute(Context & context) { \
            if ( auto prv = subexpr.compute##COMPUTE(context) ) { \
                return prv + offset; \
            } else { \
                context.throw_error_at(debugInfo,"dereferencing null pointer"); \
                return nullptr; \
            } \
        } \
    };

#define FUSION_OP1_SUBEXPR(CTYPE,node)      ((static_cast<SimNode_PtrFieldDeref*>(node))->subexpr)

#include "daScript/simulate/simulate_fusion_op1_impl.h"
#include "daScript/simulate/simulate_fusion_op1_perm.h"

    IMPLEMENT_ANY_OP1_FUSION_POINT(__forceinline,PtrFieldDeref,,vec4f)

#undef REGISTER_OP1_FUSION_POINT
#define REGISTER_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[#OPNAME].push_back(make_shared<Op1FusionPoint_##OPNAME##_##CTYPE>());

#include "daScript/simulate/simulate_fusion_op1_reg.h"

    void createFusionEngine_ptrfdr()
    {
        // (*g_fusionEngine)["PtrFieldDeref"].push_back(make_shared<Op1FusionPoint_PtrFieldDeref_vec4f>());
    }
}

#endif

