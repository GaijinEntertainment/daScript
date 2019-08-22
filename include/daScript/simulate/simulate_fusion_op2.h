#pragma once

/*
 TODO:
    Op1

    Op2AnyArg
    Op2ArgR2VOffAny +
    Op2LocR2VOffAny +
 */

#ifndef FUSION_OP_EVAL_CAST
#define FUSION_OP_EVAL_CAST(expr)          (*(expr))
#endif

#ifndef FUSION_OP_PTR_RVALUE
#define FUSION_OP_PTR_RVALUE(CTYPE,expr)   ((CTYPE *)((expr)))
#endif

#ifndef FUSION_OP_PTR_VALUE
#define FUSION_OP_PTR_VALUE(CTYPE,expr)    *((CTYPE *)((expr)))
#endif

#ifndef FUSION_OP_PTR_VALUE_LEFT
#define FUSION_OP_PTR_VALUE_LEFT(CTYPE,expr)    *((CTYPE *)((expr)))
#endif

#ifndef FUSION_OP_PTR_VALUE_RIGHT
#define FUSION_OP_PTR_VALUE_RIGHT(CTYPE,expr)   *((CTYPE *)((expr)))
#endif

#ifndef FUSION_OP_ARG_VALUE
#define FUSION_OP_ARG_VALUE(CTYPE,expr)    *((CTYPE *)(&(expr)))
#endif

#define IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE,RTYPE,RCTYPE,CONSTTYPE) \
    struct Op2FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op2FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        struct SimNode_Op2Glrf2VGlrf2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2Glrf2VGlrf2V ( const LineInfo & at, uint32_t sp_l, uint32_t ofs_l, uint32_t sp_r, uint32_t ofs_r ) \
                : SimNode(at), stackTop_l(sp_l), offset_l(ofs_l), stackTop_r(sp_r), offset_r(ofs_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "Glrf2VGlrf2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SP(stackTop_r); \
                V_SP(offset_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_VALUE_LEFT(CTYPE,((*(char **)(context.stack.sp() + stackTop_l)) + offset_l)); \
                auto rv = FUSION_OP_PTR_VALUE_RIGHT(CTYPE,((*(char **)(context.stack.sp() + stackTop_r)) + offset_r)); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            uint32_t stackTop_r; \
            uint32_t offset_r; \
        }; \
        struct SimNode_Op2Glrf2VAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2Glrf2VAny ( const LineInfo & at, uint32_t spl, uint32_t ofsl, SimNode * rn ) \
                : SimNode(at), stackTop_l(spl), offset_l(ofsl), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "Glrf2VAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv = FUSION_OP_PTR_VALUE_LEFT(CTYPE,((*(char **)(context.stack.sp() + stackTop_l)) + offset_l)); \
                auto rv =  r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            SimNode * r; \
        }; \
        struct SimNode_Op2AnyGlrf2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyGlrf2V ( const LineInfo & at, SimNode * ln, uint32_t spr, uint32_t ofsr ) \
                : SimNode(at), l(ln), stackTop_r(spr), offset_r(ofsr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyGlrf2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SUB(l); \
                V_SP(stackTop_r); \
                V_SP(offset_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,((*(char **)(context.stack.sp() + stackTop_r)) + offset_r)); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * l; \
            uint32_t stackTop_r; \
            uint32_t offset_r; \
        }; \
        struct SimNode_Op2R2VR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE_LEFT(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2R2VGtbaR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VGtbaR2V ( const LineInfo & at, uint32_t sp_l, int32_t irr ) \
                : SimNode(at),stackTop_l(sp_l), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VGtbaR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE_LEFT(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,*(CTYPE **)(context.abiThisBlockArguments()+index_r)); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            int32_t  index_r; \
        }; \
        struct SimNode_Op2ArgArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgArg ( const LineInfo & at, int32_t i_l, int32_t i_r ) \
                : SimNode(at),index_l(i_l), index_r(i_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgArg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_l]); \
                auto rv = FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_Op2ArgAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgAny ( const LineInfo & at, int32_t i_l, SimNode * rn ) \
                : SimNode(at),index_l(i_l), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_l]); \
                auto rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            SimNode * r; \
        }; \
        struct SimNode_Op2GtbaGtba : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GtbaGtba ( const LineInfo & at, int32_t i_l, int32_t i_r ) \
                : SimNode(at),index_l(i_l), index_r(i_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaGtba", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_ARG_VALUE(CTYPE,context.abiThisBlockArguments()[index_l]); \
                auto rv = FUSION_OP_ARG_VALUE(CTYPE,context.abiThisBlockArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_Op2GtbaAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GtbaAny ( const LineInfo & at, int32_t i_l, SimNode * rn ) \
                : SimNode(at),index_l(i_l), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_ARG_VALUE(CTYPE,context.abiThisBlockArguments()[index_l]); \
                auto rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            SimNode * r; \
        }; \
        struct SimNode_Op2GtbaArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GtbaArg ( const LineInfo & at, int32_t i_l, int32_t i_r ) \
                : SimNode(at), index_l(i_l), index_r(i_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaArg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_ARG_VALUE(CTYPE,context.abiThisBlockArguments()[index_l]); \
                auto rv = FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_Op2R2VAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VAny ( const LineInfo & at, uint32_t spl, SimNode * rn ) \
                : SimNode(at), stackTop_l(spl), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE_LEFT(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv =  r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            SimNode * r; \
        }; \
        struct SimNode_Op2AnyR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyR2V ( const LineInfo & at, SimNode * ln, uint32_t spr ) \
                : SimNode(at), l(ln), stackTop_r(spr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SUB(l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2GlrfR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GlrfR2V ( const LineInfo & at, uint32_t sp_l, uint32_t ofs_l, uint32_t sp_r ) \
                : SimNode(at), stackTop_l(sp_l), offset_l(ofs_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_VALUE_LEFT(CTYPE,((*(char **)(context.stack.sp() + stackTop_l)) + offset_l)); \
                auto rv = FUSION_OP_PTR_VALUE_RIGHT(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2ArgR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgR2V ( const LineInfo & at, int32_t irr, uint32_t spl ) \
                : SimNode(at), index_l(irr), stackTop_r(spl) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_l]); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t  index_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2R2VArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VArg ( const LineInfo & at, uint32_t(spl), int32_t irr ) \
                : SimNode(at), stackTop_l(spl), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VArg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE_LEFT(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            int32_t  index_r; \
        }; \
        struct SimNode_Op2R2VConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VConst ( const LineInfo & at, uint32_t spl, CONSTTYPE cvr ) \
                : SimNode(at), stackTop_l(spl), c_r(cvr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VConst", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_ARG(c_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE_LEFT(CTYPE,context.stack.sp() + stackTop_l); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c_r,context); \
            } \
            uint32_t stackTop_l; \
            CONSTTYPE c_r; \
        }; \
        struct SimNode_Op2ArgConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgConst ( const LineInfo & at, int32_t ill, CONSTTYPE cvr ) \
                : SimNode(at), index_l(ill), c_r(cvr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgConst", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(c_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_l]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c_r,context); \
            } \
            int32_t index_l; \
            CONSTTYPE c_r; \
        }; \
        struct SimNode_Op2AnyConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyConst ( const LineInfo & at, SimNode * ll, CONSTTYPE cvr ) \
                : SimNode(at), l(ll), c_r(cvr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyConst", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SUB(l); \
                V_ARG(c_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c_r,context); \
            } \
            SimNode * l; \
            CONSTTYPE c_r; \
        }; \
        struct SimNode_Op2ConstAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstAny ( const LineInfo & at, CONSTTYPE cvl, SimNode * rr ) \
                : SimNode(at), c_l(cvl), r(rr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(c_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (c_l,rv,context); \
            } \
            CONSTTYPE c_l; \
            SimNode * r; \
        }; \
        struct SimNode_Op2ConstR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstR2V ( const LineInfo & at, CONSTTYPE cvl, uint32_t spr ) \
                : SimNode(at), c_l(cvl), stackTop_r(spr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(c_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (c_l,rv,context); \
            } \
            CONSTTYPE c_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2ConstArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstArg ( const LineInfo & at, CONSTTYPE cvl, int32_t irr ) \
                : SimNode(at), c_l(cvl), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstArg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(c_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv = FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (c_l,rv,context); \
            } \
            CONSTTYPE c_l; \
            int32_t index_r; \
        }; \
        struct SimNode_Op2GtbaR2VGtba : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GtbaR2VGtba ( const LineInfo & at, int32_t i_l, int32_t i_r ) \
                : SimNode(at),index_l(i_l), index_r(i_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaR2VGtba", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_VALUE_LEFT(CTYPE,*(CTYPE **)(context.abiThisBlockArguments()+index_l)); \
                auto rv = FUSION_OP_ARG_VALUE(CTYPE,context.abiThisBlockArguments()[index_r]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_Op2GtbaR2VAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GtbaR2VAny ( const LineInfo & at, int32_t i_l, SimNode * rn ) \
                : SimNode(at),index_l(i_l), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaR2VAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_VALUE_LEFT(CTYPE,*(CTYPE **)(context.abiThisBlockArguments()+index_l)); \
                auto rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t index_l; \
            SimNode * r; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op2 *>(node); \
            /* OP(*,ConstValue) */ \
            if ( is(info,tnode->r,"ConstValue") ) { \
                auto cnode = static_cast<SimNode_ConstValue *>(tnode->r); \
                auto cvalue = cast<CONSTTYPE>::to(cnode->value); \
                /* OP(GetLocalR2V,ConstValue) */ \
                if ( is(info,tnode->l,"GetLocalR2V") ) { \
                    auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2R2VConst>(node->debugInfo, r2vnode_l->subexpr.stackTop, cvalue); \
                /* OP(GetArgument,ConstValue) */ \
                } else if ( is(info,tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgConst>(node->debugInfo, argnode_l->index, cvalue); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyConst>(node->debugInfo, tnode->l, cvalue); \
                } \
            /* OP(ConstValue,*) */ \
            } else if ( is(info,tnode->l,"ConstValue") ) { \
                auto cnode = static_cast<SimNode_ConstValue *>(tnode->l); \
                auto cvalue = cast<CONSTTYPE>::to(cnode->value); \
                /* OP(ConstValue,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstR2V>(node->debugInfo, cvalue, r2vnode_r->subexpr.stackTop); \
                /* OP(ConstValue,GetArgument) */ \
                } else if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstArg>(node->debugInfo, cvalue, argnode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2ConstAny>(node->debugInfo, cvalue, tnode->r); \
                } \
            /* OP(GetLocalR2V,*) */ \
            } else if ( is(info,tnode->l,"GetLocalR2V") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                /* OP(GetLocalR2V,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VR2V>(node->debugInfo, r2vnode_l->subexpr.stackTop, r2vnode_r->subexpr.stackTop); \
                /* OP(GetLocalR2V,GetArgument) */ \
                } else if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VArg>(node->debugInfo, r2vnode_l->subexpr.stackTop, argnode_r->index); \
                /* OP(GetLocalR2V,GetThisBlockArgumentR2V) */ \
                } else if ( is(info,tnode->r,"GetThisBlockArgumentR2V") ) { \
                    auto argnode_r = static_cast<SimNode_GetThisBlockArgumentR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VGtbaR2V>(node->debugInfo, r2vnode_l->subexpr.stackTop, argnode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2R2VAny>(node->debugInfo, r2vnode_l->subexpr.stackTop, tnode->r); \
                } \
            /* OP(*,GetLocalR2V) */ \
            } else if ( is(info,tnode->r, "GetLocalR2V") ) { \
                auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                /* OP(GetArgument,GetLocalR2V) */ \
                if ( is(info,tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgR2V>(node->debugInfo, \
                        argnode_l->index, r2vnode_r->subexpr.stackTop); \
                /* OP(GetLocalRefR2VOff,GetLocalR2V) */ \
                } else if ( is(info,tnode->l,"GetLocalRefR2VOff") ) { \
                    auto r2vonode_l = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2GlrfR2V>(node->debugInfo, \
                        r2vonode_l->subexpr.stackTop, r2vonode_l->subexpr.offset, r2vnode_r->subexpr.stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyR2V>(node->debugInfo, tnode->l, r2vnode_r->subexpr.stackTop); \
                } \
            /* OP(GetArgument,* */ \
            } else if ( is(info,tnode->l,"GetArgument") ) { \
                auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                /* OP(GetArgument,GetArgument) */ \
                if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ArgArg>(node->debugInfo, argnode_l->index, argnode_r->index); \
                } else {\
                    return context->code->makeNode<SimNode_Op2ArgAny>(node->debugInfo, argnode_l->index, tnode->r); \
                } \
            /* OP(GetThisBlockArgument,* */ \
            } else if ( is(info,tnode->l,"GetThisBlockArgument") ) { \
                auto argnode_l = static_cast<SimNode_GetThisBlockArgument *>(tnode->l); \
                /* OP(GetThisBlockArgument,GetThisBlockArgument) */ \
                if ( is(info,tnode->r,"GetThisBlockArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetThisBlockArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2GtbaGtba>(node->debugInfo, argnode_l->index, argnode_r->index); \
                /* OP(GetThisBlockArgument,GetArgument) */ \
                } else if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2GtbaArg>(node->debugInfo, argnode_l->index, argnode_r->index); \
                } else {\
                    return context->code->makeNode<SimNode_Op2GtbaAny>(node->debugInfo, argnode_l->index, tnode->r); \
                } \
            /* OP(GetThisBlockArgumentR2V,* */ \
            } else if ( is(info,tnode->l,"GetThisBlockArgumentR2V") ) { \
                auto argnode_l = static_cast<SimNode_GetThisBlockArgumentR2V<CTYPE> *>(tnode->l); \
                /* OP(GetThisBlockArgumentR2V,GetThisBlockArgument) */ \
                if ( is(info,tnode->r,"GetThisBlockArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetThisBlockArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2GtbaR2VGtba>(node->debugInfo, argnode_l->index, argnode_r->index); \
                } else {\
                    return context->code->makeNode<SimNode_Op2GtbaR2VAny>(node->debugInfo, argnode_l->index, tnode->r); \
                } \
            /* OP(GetLocalRefR2VOff,*) */ \
            } else if ( is(info,tnode->l,"GetLocalRefR2VOff") ) { \
                auto r2vonode_l = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->l); \
                if ( is(info,tnode->r,"GetLocalRefR2VOff") ) { \
                    auto r2vonode_r = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2Glrf2VGlrf2V>(node->debugInfo, \
                        r2vonode_l->subexpr.stackTop, r2vonode_l->subexpr.offset, r2vonode_r->subexpr.stackTop, r2vonode_r->subexpr.offset); \
                } else { \
                    return context->code->makeNode<SimNode_Op2Glrf2VAny>(node->debugInfo, \
                        r2vonode_l->subexpr.stackTop, r2vonode_l->subexpr.offset, tnode->r); \
                } \
            /* OP(*,GetLocalRefR2VOff) */ \
            } else if ( is(info,tnode->r,"GetLocalRefR2VOff") ) { \
                auto r2vonode_r = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->r); \
                return context->code->makeNode<SimNode_Op2AnyGlrf2V>(node->debugInfo, tnode->l, \
                    r2vonode_r->subexpr.stackTop, r2vonode_r->subexpr.offset); \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE,TYPE,CTYPE,CTYPE)

#define IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE,Bool,bool,CTYPE)

#define IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,,CTYPE,,vec4f,vec4f)

#define IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_ANY_OP2_FUSION_POINT(OPNAME,,CTYPE,Bool,bool,vec4f)

#define IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    struct Op2FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op2FusionPoint_##OPNAME##_##CTYPE () {} \
        struct SimNode_SetOp2LocR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2LocR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "LocR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_RVALUE(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv = FUSION_OP_PTR_VALUE_RIGHT(CTYPE,context.stack.sp() + stackTop_r); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_SetOp2LocAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2LocAny ( const LineInfo & at, uint32_t spl, SimNode * rn ) \
                : SimNode(at), stackTop_l(spl), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "LocAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            SimNode * r; \
        }; \
        struct SimNode_SetOp2GlrfAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2GlrfAny ( const LineInfo & at, uint32_t spl, uint32_t ofsl, SimNode * rn ) \
                : SimNode(at), stackTop_l(spl), offset_l(ofsl), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.stack.sp() + stackTop_l)) + offset_l); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            SimNode * r; \
        }; \
        struct SimNode_SetOp2GlrfGlrf2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_SetOp2GlrfGlrf2V ( const LineInfo & at, uint32_t sp_l, uint32_t ofs_l, uint32_t sp_r, uint32_t ofs_r ) \
                : SimNode(at), stackTop_l(sp_l), offset_l(ofs_l), stackTop_r(sp_r), offset_r(ofs_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfGlrf2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SP(stackTop_r); \
                V_SP(offset_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.stack.sp() + stackTop_l)) + offset_l); \
                auto rv = FUSION_OP_PTR_VALUE_RIGHT(CTYPE,(*(char **)(context.stack.sp() + stackTop_r)) + offset_r); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            uint32_t stackTop_r; \
            uint32_t offset_r; \
        }; \
        struct SimNode_SetOp2GlrfArg : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2GlrfArg ( const LineInfo & at, uint32_t spl, uint32_t ofsl, int32_t irr ) \
                : SimNode(at), stackTop_l(spl), offset_l(ofsl), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfArg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.stack.sp() + stackTop_l)) + offset_l); \
                auto rv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_r]); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            int32_t index_r; \
        }; \
        struct SimNode_SetOp2ArgOffAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2ArgOffAny ( const LineInfo & at, int32_t il, uint32_t ol, SimNode * rn ) \
                : SimNode(at), index_l(il), offset_l(ol), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgOffAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(offset_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiArguments()+index_l)) + offset_l); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            int32_t index_l; \
            uint32_t offset_l; \
            SimNode * r; \
        }; \
        struct SimNode_SetOp2ArgOffArgOffR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2ArgOffArgOffR2V ( const LineInfo & at, int32_t ill, uint32_t oll, int32_t irr, uint32_t orr ) \
                : SimNode(at), index_l(ill), offset_l(oll), index_r(irr), offset_r(orr) {}  \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgOffArgOffR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(offset_l); \
                V_ARG(index_r); \
                V_ARG(offset_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiArguments()+index_l)) + offset_l); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,(*(char **)(context.abiArguments()+index_r)) + offset_r); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            int32_t index_l; \
            uint32_t offset_l; \
            int32_t index_r; \
            uint32_t offset_r; \
        }; \
        struct SimNode_SetOp2GtbaAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2GtbaAny ( const LineInfo & at, int32_t ill, SimNode * rn ) \
                : SimNode(at), index_l(ill), r(rn) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiThisBlockArguments()+index_l))); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            int32_t index_l; \
            SimNode * r; \
        }; \
        struct SimNode_SetOp2GtbaGtba : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2GtbaGtba ( const LineInfo & at, int32_t ill, int32_t irr ) \
                : SimNode(at), index_l(ill), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaGtba", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiThisBlockArguments()+index_l))); \
                auto rv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiThisBlockArguments()[index_r]); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_SetOp2GtbaGtbaR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2GtbaGtbaR2V ( const LineInfo & at, int32_t ill, int32_t irr ) \
                : SimNode(at), index_l(ill), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GtbaGtbaR2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiThisBlockArguments()+index_l))); \
                auto rv =  FUSION_OP_PTR_VALUE_RIGHT(CTYPE,*(CTYPE **)(context.abiThisBlockArguments()+index_r)); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        struct SimNode_SetOp2ArgAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2ArgAny ( const LineInfo & at, SimNode * rn, int32_t ill ) \
                : SimNode(at), r(rn), index_l(ill) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiArguments()+index_l))); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            int32_t index_l; \
        }; \
        struct SimNode_SetOp2ArgArg : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp2ArgArg ( const LineInfo & at, int32_t ill, int32_t irr ) \
                : SimNode(at), index_l(ill), index_r(irr) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgArg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_ARG(index_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.abiArguments()+index_l))); \
                auto rv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index_r]); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            int32_t index_l; \
            int32_t index_r; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op2 *>(node); \
            /* OP(GetLocal,*) */ \
            if ( is(info,tnode->l,"GetLocal") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocal *>(tnode->l); \
                /* OP(GetLocal,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2LocR2V>(node->debugInfo, \
                        r2vnode_l->subexpr.stackTop, r2vnode_r->subexpr.stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_SetOp2LocAny>(node->debugInfo, \
                        r2vnode_l->subexpr.stackTop, tnode->r); \
                } \
            /* OP(GetLocalRefOff,*) */ \
            } else if ( is(info,tnode->l,"GetLocalRefOff") ) { \
                auto glrfnode_l = static_cast<SimNode_GetLocalRefOff *>(tnode->l); \
                /* OP(GetLocalRefOff,GetLocalRefR2VOff) */ \
                if ( is(info,tnode->r,"GetLocalRefR2VOff") ) { \
                    auto r2vonode_r = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2GlrfGlrf2V>(node->debugInfo, \
                        glrfnode_l->subexpr.stackTop, glrfnode_l->subexpr.offset, \
                        r2vonode_r->subexpr.stackTop, r2vonode_r->subexpr.offset); \
                /* OP(GetLocalRefOff,GetArgument) */ \
                } else if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2GlrfArg>(node->debugInfo,\
                        glrfnode_l->subexpr.stackTop, glrfnode_l->subexpr.offset, \
                        argnode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_SetOp2GlrfAny>(node->debugInfo, \
                        glrfnode_l->subexpr.stackTop, glrfnode_l->subexpr.offset, tnode->r); \
                } \
            /* OP(GetArgumentOff,*) */ \
            } else if ( is(info,tnode->l,"GetArgumentOff") ) { \
                auto aofnode_l = static_cast<SimNode_GetArgumentOff *>(tnode->l); \
                /* OP(GetArgumentOff,GetArgumentR2VOff) */ \
                if ( is(info,tnode->r,"GetArgumentR2VOff") ) { \
                    auto aofnode_r = static_cast<SimNode_GetArgumentR2VOff<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2ArgOffArgOffR2V>(node->debugInfo, \
                        aofnode_l->index, aofnode_l->offset, aofnode_r->index, aofnode_r->offset); \
                } else { \
                    return context->code->makeNode<SimNode_SetOp2ArgOffAny>(node->debugInfo, \
                        aofnode_l->index, aofnode_l->offset, tnode->r); \
                } \
            /* OP(GetThisBlockArgument,*) */ \
            } else if ( is(info,tnode->l,"GetThisBlockArgument") ) { \
                auto tbanode_l = static_cast<SimNode_GetThisBlockArgument *>(tnode->l); \
                /* OP(GetThisBlockArgument,GetThisBlockArgument) */ \
                if ( is(info,tnode->r,"GetThisBlockArgument") ) { \
                    auto tbanode_r = static_cast<SimNode_GetThisBlockArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2GtbaGtba>(node->debugInfo, \
                        tbanode_l->index, tbanode_r->index); \
                /* OP(GetThisBlockArgument,GetThisBlockArgument) */ \
                } else if ( is(info,tnode->r,"GetThisBlockArgumentR2V") ) { \
                    auto tbanode_r = static_cast<SimNode_GetThisBlockArgumentR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2GtbaGtbaR2V>(node->debugInfo, \
                        tbanode_l->index, tbanode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_SetOp2GtbaAny>(node->debugInfo, \
                        tbanode_l->index, tnode->r); \
                } \
             /* OP(GetArgument,*) */ \
            } else if ( is(info,tnode->l,"GetArgument") ) { \
                auto tbanode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                /* OP(GetArgument,GetArgument) */ \
                if ( is(info,tnode->r,"GetArgument") ) { \
                    auto tbanode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_SetOp2ArgArg>(node->debugInfo, \
                        tbanode_l->index, tbanode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_SetOp2ArgAny>(node->debugInfo, \
                        tnode->r, tbanode_l->index); \
                } \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,,CTYPE)

#define REGISTER_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<Op2FusionPoint_##OPNAME##_##CTYPE>());

#define REGISTER_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<Op2FusionPoint_##OPNAME##_##CTYPE>());

#define IMPLEMENT_OP2_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Double,double);

#define IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_SET_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP2_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,Double,double);

#define IMPLEMENT_BOOL_OP2_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_BOOL_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_BOOL_OP2_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_BOOL_OP2_FUSION_POINT(OPNAME,Double,double);

#define REGISTER_OP2_INTEGER_FUSION_POINT(OPNAME) \
    REGISTER_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define REGISTER_OP2_NUMERIC_FUSION_POINT(OPNAME) \
    REGISTER_OP2_INTEGER_FUSION_POINT(OPNAME); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Float,float); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Double,double);

// vectors

#define IMPLEMENT_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define IMPLEMENT_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME)  \
    IMPLEMENT_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    IMPLEMENT_OP2_VEC_FUSION_POINT(OPNAME,float4 );

#define IMPLEMENT_BOOL_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define IMPLEMENT_BOOL_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME)  \
    IMPLEMENT_BOOL_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    IMPLEMENT_BOOL_OP2_VEC_FUSION_POINT(OPNAME,float4 );

#define IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define IMPLEMENT_SET_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME)  \
    IMPLEMENT_SET_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,float4 );

#define REGISTER_OP2_VEC_INTEGER_FUSION_POINT(OPNAME) \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,int2 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,uint2); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,int3 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,uint3); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,int4 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,uint4);

#define REGISTER_OP2_VEC_NUMERIC_FUSION_POINT(OPNAME) \
    REGISTER_OP2_VEC_INTEGER_FUSION_POINT(OPNAME); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,float2 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,float3 ); \
    REGISTER_OP2_VEC_FUSION_POINT(OPNAME,float4 );
