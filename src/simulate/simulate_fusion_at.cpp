#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/ast/ast_typedecl.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    struct SimNode_Op2At : SimNode_Op2Fusion {
        virtual SimNode * visit(SimVisitor & vis) override {
            V_BEGIN();
            string name = op;
            name += getSimSourceName(l.type);
            name += getSimSourceName(r.type);
            if (baseType != Type::none) {
                vis.op(name.c_str(), getTypeBaseSize(baseType), das_to_string(baseType));
            } else {
                vis.op(name.c_str());
            }
            l.visit(vis);
            r.visit(vis);
            V_ARG(stride);
            V_ARG(offset);
            V_ARG(range);
            V_END();
        }
        uint32_t  stride, offset, range;
    };

#define INLINE

#define IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,COMPUTEL) \
    struct SimNode_##OPNAME##_##COMPUTEL##_Any : SimNode_Op2At { \
        INLINE auto compute ( Context & context ) { \
            auto pl = l.compute##COMPUTEL(context); \
            auto rr = uint32_t(r.subexpr->evalInt(context)); \
            if ( rr >= range ) context.throw_error_at(debugInfo,"index out of range"); \
            return *((CTYPE *)(pl + rr*stride + offset)); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

#define IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,COMPUTEL,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER : SimNode_Op2At { \
        INLINE auto compute ( Context & context ) { \
            auto pl = l.compute##COMPUTEL(context); \
            auto rr = *((uint32_t *)r.compute##COMPUTEL(context)); \
            if ( rr >= range ) context.throw_error_at(debugInfo,"index out of range"); \
            return *((CTYPE *)(pl + rr*stride + offset)); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

#define IMPLEMENT_OP2_SET_SETUP_NODE(result,node) \
    auto rn = (SimNode_Op2At *)result; \
    auto sn = (SimNode_At *)node; \
    rn->stride = sn->stride; \
    rn->offset = sn->offset; \
    rn->range = sn->range;

#include "daScript/simulate/simulate_fusion_op2_set_impl.h"
#include "daScript/simulate/simulate_fusion_op2_set_perm.h"

    IMPLEMENT_SETOP_SCALAR(AtR2V);

#define IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,COMPUTEL) \
    struct SimNode_##OPNAME##_##COMPUTEL##_Any : SimNode_Op2At { \
        INLINE auto compute ( Context & context ) { \
            auto pl = l.compute##COMPUTEL(context); \
            auto rr = uint32_t(r.subexpr->evalInt(context)); \
            if ( rr >= range ) context.throw_error_at(debugInfo,"index out of range"); \
            return pl + rr*stride + offset; \
        } \
        DAS_PTR_NODE; \
    }; 

#define IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,COMPUTEL,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER : SimNode_Op2At { \
        INLINE auto compute ( Context & context ) { \
            auto pl = l.compute##COMPUTEL(context); \
            auto rr = *((uint32_t *)r.compute##COMPUTEL(context)); \
            if ( rr >= range ) context.throw_error_at(debugInfo,"index out of range"); \
            return pl + rr*stride + offset; \
        } \
        DAS_PTR_NODE; \
    }; 

#undef IMPLEMENT_OP2_SET_SETUP_NODE
#define IMPLEMENT_OP2_SET_SETUP_NODE(result,node) \
    auto rn = (SimNode_Op2At *)result; \
    auto sn = (SimNode_At *)node; \
    rn->stride = sn->stride; \
    rn->offset = sn->offset; \
    rn->range = sn->range; \
    rn->baseType = Type::none;

#include "daScript/simulate/simulate_fusion_op2_set_impl.h"
#include "daScript/simulate/simulate_fusion_op2_set_perm.h"

    typedef char * Dummy;
    IMPLEMENT_ANY_SETOP(At, Ptr, Dummy);

    void createFusionEngine_at() {
        REGISTER_SETOP_SCALAR(AtR2V);
        (*g_fusionEngine)["At"].push_back(make_shared<FusionPoint_Set_At_Dummy>());
    }
}
