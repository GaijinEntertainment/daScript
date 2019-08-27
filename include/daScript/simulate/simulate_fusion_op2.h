#pragma once

#define FUSION_OP2_PTR(CTYPE,expr)              (((CTYPE *)(expr)))
#define FUSION_OP2_RVALUE_LEFT(CTYPE,expr)      (*((CTYPE *)(expr)))
#define FUSION_OP2_RVALUE_RIGHT(CTYPE,expr)     (*((CTYPE *)(expr)))
#define FUSION_OP2_PTR_TO_LVALUE(expr)          (*(expr))

#define IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,COMPUTEL) \
    struct SimNode_##OPNAME##_##COMPUTEL##_Any : SimNode_Op2Fusion { \
        virtual vec4f eval ( Context & context ) override { \
            auto pl = FUSION_OP2_PTR(CTYPE,l.compute##COMPUTEL(context)); \
            auto rr = r.subexpr->eval##TYPE(context); \
            SimPolicy<CTYPE>::OPNAME(FUSION_OP2_PTR_TO_LVALUE(pl),rr,context); \
            return v_zero(); \
        } \
    }; 

#define IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,COMPUTEL,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER : SimNode_Op2Fusion { \
        virtual vec4f eval ( Context & context ) override { \
            auto pl = FUSION_OP2_PTR(CTYPE,l.compute##COMPUTEL(context)); \
            auto rr = FUSION_OP2_RVALUE_RIGHT(CTYPE,r.compute##COMPUTER(context)); \
            SimPolicy<CTYPE>::OPNAME(FUSION_OP2_PTR_TO_LVALUE(pl),rr,context); \
            return v_zero(); \
        } \
    }; 

// extra node settings
#define IMPLEMENT_OP2_SET_SETUP_NODE(result,node)

#include "daScript/simulate/simulate_fusion_op2_set_impl.h"
#include "daScript/simulate/simulate_fusion_op2_set_perm.h"

// OP(COMPUTEL,*) 
#define IMPLEMENT_OP2_NODE_ANYR(INLINE,OPNAME,TYPE,CTYPE,COMPUTEL) \
    struct SimNode_##OPNAME##_Any_##COMPUTEL : SimNode_Op2Fusion { \
        INLINE auto compute ( Context & context ) { \
            auto ll = l.subexpr->eval##TYPE(context); \
            auto rr = FUSION_OP2_RVALUE_RIGHT(CTYPE,r.compute##COMPUTEL(context)); \
            return SimPolicy<CTYPE>::OPNAME(ll,rr,context); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

// OP(*,COMPUTER)
#define IMPLEMENT_OP2_NODE_ANYL(INLINE,OPNAME,TYPE,CTYPE,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTER##_Any : SimNode_Op2Fusion { \
        INLINE auto compute ( Context & context ) { \
            auto ll = FUSION_OP2_RVALUE_LEFT (CTYPE,l.compute##COMPUTER(context)); \
            auto rr = r.subexpr->eval##TYPE(context); \
            return SimPolicy<CTYPE>::OPNAME(ll,rr,context); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

// OP(COMPUTEL,COMPUTER)
#define IMPLEMENT_OP2_NODE(INLINE,OPNAME,TYPE,CTYPE,COMPUTEL,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER : SimNode_Op2Fusion { \
        INLINE auto compute ( Context & context ) { \
            auto ll = FUSION_OP2_RVALUE_LEFT (CTYPE,l.compute##COMPUTEL(context)); \
            auto rr = FUSION_OP2_RVALUE_RIGHT(CTYPE,r.compute##COMPUTER(context)); \
            return SimPolicy<CTYPE>::OPNAME(ll,rr,context); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

// extra node settings
#define IMPLEMENT_OP2_SETUP_NODE(result,node)

#include "daScript/simulate/simulate_fusion_op2_impl.h"
#include "daScript/simulate/simulate_fusion_op2_perm.h"


