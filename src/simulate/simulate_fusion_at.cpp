#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

/*
 TODO:
    ArrayAt
    AtGlobConst - 'do not check index, if index is out of range - do AOT error'
 */

#define FUSION_AT_PTR_TO_RESULT(CTYPE,expr)     (expr)

#define IMPLEMENT_ANY_AT_FUSION_POINT(TYPE,CTYPE,RTYPE,CNAME,ATNAME) \
    struct AtFusionPoint##CNAME : FusionPoint { \
        AtFusionPoint##CNAME () {} \
        struct SimNode_AtGlobAny : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtGlobAny ( const LineInfo & at, SimNode * idx, uint32_t strd, uint32_t o, uint32_t rng, uint32_t lvo ) \
                : SimNode_At(at, nullptr, idx, strd, o, rng), goffset_l(lvo) {}\
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##GlobAny); \
                V_ARG(goffset_l); \
                V_SUB(index); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = context.globals + goffset_l; \
                uint32_t idx = uint32_t(index->evalInt(context)); \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            uint32_t goffset_l; \
        }; \
        struct SimNode_AtGlobLocR2VI : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtGlobLocR2VI ( const LineInfo & at, uint32_t strd, uint32_t o, uint32_t rng, uint32_t lvo, uint32_t rvsp ) \
                : SimNode_At(at, nullptr, nullptr, strd, o, rng), goffset_l(lvo), stackTop_r(rvsp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##GlobLocR2VI); \
                V_ARG(goffset_l); \
                V_SP(stackTop_r); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = context.globals + goffset_l; \
                uint32_t idx = *(uint32_t *)(context.stack.sp() + stackTop_r); \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            uint32_t goffset_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_AtGlobConst : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtGlobConst ( const LineInfo & at, uint32_t strd, uint32_t o, uint32_t rng, uint32_t lvo, uint32_t rvc ) \
                : SimNode_At(at, nullptr, nullptr, strd, o, rng), goffset_l(lvo), const_r(rvc) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##GlobConst); \
                V_ARG(goffset_l); \
                V_ARG(const_r); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = context.globals + goffset_l; \
                uint32_t idx = const_r; \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            uint32_t goffset_l; \
            uint32_t const_r; \
        }; \
        struct SimNode_AtLocAny : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtLocAny ( const LineInfo & at, SimNode * idx, uint32_t strd, uint32_t o, uint32_t rng, uint32_t spl ) \
                : SimNode_At(at, nullptr, idx, strd, o, rng), stackTop_l(spl) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##LocAny); \
                V_ARG(stackTop_l); \
                V_SUB(index); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = context.stack.sp() + stackTop_l; \
                uint32_t idx = uint32_t(index->evalInt(context)); \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            uint32_t stackTop_l; \
        }; \
        struct SimNode_AtLocLocR2VI : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtLocLocR2VI ( const LineInfo & at, uint32_t strd, uint32_t o, uint32_t rng, uint32_t spl, uint32_t rvsp ) \
                : SimNode_At(at, nullptr, nullptr, strd, o, rng), stackTop_l(spl), stackTop_r(rvsp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##LocLocR2VI); \
                V_ARG(stackTop_l); \
                V_ARG(stackTop_r); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = context.stack.sp() + stackTop_l; \
                uint32_t idx = *(uint32_t *)(context.stack.sp() + stackTop_r); \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_AtArgAny : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtArgAny ( const LineInfo & at, SimNode * idx, uint32_t strd, uint32_t o, uint32_t rng, int32_t il ) \
                : SimNode_At(at, nullptr, idx, strd, o, rng), index_l(il) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##ArgAny); \
                V_ARG(index_l); \
                V_SUB(index); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = *(char **)(context.abiArguments()+index_l); \
                uint32_t idx = uint32_t(index->evalInt(context)); \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            int32_t index_l; \
        }; \
        struct SimNode_AtArgLocR2VI : SimNode_At { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_AtArgLocR2VI ( const LineInfo & at, uint32_t strd, uint32_t o, uint32_t rng, int32_t il, uint32_t rvsp ) \
                : SimNode_At(at, nullptr, nullptr, strd, o, rng), index_l(il), stackTop_r(rvsp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                V_OP(ATNAME##ArgLocR2VI); \
                V_ARG(index_l); \
                V_SP(stackTop_r); \
                V_ARG(stride); \
                V_ARG(offset); \
                V_ARG(range); \
                V_END(); \
            } \
            __forceinline RTYPE compute (Context & context) { \
                auto pValue = *(char **)(context.abiArguments()+index_l); \
                uint32_t idx = *(uint32_t *)(context.stack.sp() + stackTop_r); \
                if (idx >= range) context.throw_error_at(debugInfo,"index out of range"); \
                return FUSION_AT_PTR_TO_RESULT(CTYPE,pValue + idx*stride + offset); \
            } \
            int32_t index_l; \
            uint32_t stackTop_r; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto atnode = static_cast<SimNode_At *>(node); \
            /* At(GetGlobal,*) */ \
            if ( is(info,atnode->value,"GetGlobal") ) { \
                auto gnode_l = static_cast<SimNode_GetGlobal *>(atnode->value); \
                /* At(GetGlobal,GetLocalR2V<int|uint>) */ \
                if ( is(info,atnode->index,"GetLocalR2V","int") || is(info,atnode->index,"GetLocalR2V","uint") ) { \
                    auto gnode_r = static_cast<SimNode_GetLocalR2V<int32_t> *>(atnode->index); \
                    return context->code->makeNode<SimNode_AtGlobLocR2VI>(node->debugInfo, \
                        atnode->stride, atnode->offset, atnode->range, \
                            gnode_l->offset, gnode_r->subexpr.stackTop); \
                /* At(GetGlobal,ConstValue) */ \
                } else if ( is(info,atnode->index,"ConstValue") ) { \
                    auto cnode_r = static_cast<SimNode_ConstValue *>(atnode->index); \
                    return context->code->makeNode<SimNode_AtGlobConst>(node->debugInfo, \
                        atnode->stride, atnode->offset, atnode->range, \
                            gnode_l->offset, cnode_r->valueU); \
                } else { \
                    return context->code->makeNode<SimNode_AtGlobAny>(node->debugInfo, \
                        atnode->index, atnode->stride, atnode->offset, atnode->range, \
                            gnode_l->offset); \
                } \
            /* At(GetLocal,*) */ \
            } else if ( is(info,atnode->value,"GetLocal") ) { \
                auto lnode_l = static_cast<SimNode_GetLocal *>(atnode->value); \
                /* At(GetLocal,GetLocalR2V<int|uint>) */ \
                if ( is(info,atnode->index,"GetLocalR2V","int") || is(info,atnode->index,"GetLocalR2V","uint") ) { \
                    auto gnode_r = static_cast<SimNode_GetLocalR2V<int32_t> *>(atnode->index); \
                    return context->code->makeNode<SimNode_AtLocLocR2VI>(node->debugInfo, \
                        atnode->stride, atnode->offset, atnode->range, \
                            lnode_l->subexpr.stackTop, gnode_r->subexpr.stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_AtLocAny>(node->debugInfo, \
                        atnode->index, atnode->stride, atnode->offset, atnode->range, \
                            lnode_l->subexpr.stackTop); \
                } \
            /* At(GetArgument,*) */ \
            } else if ( is(info,atnode->value,"GetArgument") ) { \
                auto anode_l = static_cast<SimNode_GetArgument *>(atnode->value); \
                /* At(GetArgument,GetLocalR2V<int|uint>) */ \
                if ( is(info,atnode->index,"GetLocalR2V","int") || is(info,atnode->index,"GetLocalR2V","uint") ) { \
                    auto gnode_r = static_cast<SimNode_GetLocalR2V<int32_t> *>(atnode->index); \
                    return context->code->makeNode<SimNode_AtArgLocR2VI>(node->debugInfo, \
                        atnode->stride, atnode->offset, atnode->range, \
                            anode_l->index, gnode_r->subexpr.stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_AtArgAny>(node->debugInfo, \
                        atnode->index, atnode->stride, atnode->offset, atnode->range, \
                            anode_l->index); \
                } \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_ATR2V_FUSION_POINT(TYPE,CTYPE) \
    IMPLEMENT_ANY_AT_FUSION_POINT(TYPE,CTYPE,CTYPE,_##CTYPE,AtR2V);

#define IMPLEMENT_ATR2V_VEC_FUSION_POINT(CTYPE) \
    IMPLEMENT_ANY_AT_FUSION_POINT(,CTYPE,char *,_##CTYPE,AtR2V);

#define REGISTER_ATR2V_FUSION_POINT(CTYPE) \
    (*g_fusionEngine)[fuseName("AtR2V",typeName<CTYPE>::name())].push_back(make_shared<AtFusionPoint_##CTYPE>());

// IMPLEMENT AT
    IMPLEMENT_ANY_AT_FUSION_POINT(Ptr,char *,char *,,At);

// IMPLEMENT AtR2V for scalar types
#undef FUSION_AT_PTR_TO_RESULT
#define FUSION_AT_PTR_TO_RESULT(CTYPE,expr)     (*((CTYPE *)(expr)))

    IMPLEMENT_ATR2V_FUSION_POINT(Int,int32_t);
    IMPLEMENT_ATR2V_FUSION_POINT(UInt,uint32_t);
    IMPLEMENT_ATR2V_FUSION_POINT(Int64,int64_t);
    IMPLEMENT_ATR2V_FUSION_POINT(UInt64,uint64_t);
    IMPLEMENT_ATR2V_FUSION_POINT(Float,float);
    IMPLEMENT_ATR2V_FUSION_POINT(Double,double);
    IMPLEMENT_ATR2V_FUSION_POINT(Bool,bool);

// IMPLEMENT AtR2V for vector types
    #undef DAS_NODE
    #define DAS_NODE(TYPE,CTYPE)                                    \
        virtual vec4f eval ( das::Context & context ) override {    \
            return v_ldu((const float *)compute(context));          \
        }

    #undef  FUSION_AT_PTR_TO_RESULT
    #define FUSION_AT_PTR_TO_RESULT(CTYPE,expr)     (expr)

    IMPLEMENT_ATR2V_VEC_FUSION_POINT(int2);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(int3);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(int4);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(uint2);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(uint3);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(uint4);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(float2);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(float3);
    IMPLEMENT_ATR2V_VEC_FUSION_POINT(float4);

    void createFusionEngine_at() {
        // regular At
        (*g_fusionEngine)["At"].push_back(make_shared<AtFusionPoint>());
        // AtR2V for all the workhorse types
        REGISTER_ATR2V_FUSION_POINT(int32_t);
        REGISTER_ATR2V_FUSION_POINT(uint32_t);
        REGISTER_ATR2V_FUSION_POINT(int64_t);
        REGISTER_ATR2V_FUSION_POINT(uint64_t);
        REGISTER_ATR2V_FUSION_POINT(float);
        REGISTER_ATR2V_FUSION_POINT(double);
        REGISTER_ATR2V_FUSION_POINT(bool);
        // AtR2V for all vector types
        REGISTER_ATR2V_FUSION_POINT(int2);
        REGISTER_ATR2V_FUSION_POINT(int3);
        REGISTER_ATR2V_FUSION_POINT(int4);
        REGISTER_ATR2V_FUSION_POINT(uint2);
        REGISTER_ATR2V_FUSION_POINT(uint3);
        REGISTER_ATR2V_FUSION_POINT(uint4);
        REGISTER_ATR2V_FUSION_POINT(float2);
        REGISTER_ATR2V_FUSION_POINT(float3);
        REGISTER_ATR2V_FUSION_POINT(float4);
    }
}
