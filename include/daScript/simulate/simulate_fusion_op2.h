#pragma once

/*
 TODO:
    Op2ArgAny
    Op2AnyArg
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

#ifndef FUSION_OP_ARG_VALUE
#define FUSION_OP_ARG_VALUE(CTYPE,expr)    cast<CTYPE>::to(expr)
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
                vis.op(#OPNAME "Glrf2VGlrf2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SP(stackTop_r); \
                V_SP(offset_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_VALUE(CTYPE,((*(char **)(context.stack.sp() + stackTop_l)) + offset_l)); \
                auto rv = FUSION_OP_PTR_VALUE(CTYPE,((*(char **)(context.stack.sp() + stackTop_r)) + offset_r)); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            uint32_t stackTop_r; \
            uint32_t offset_r; \
        }; \
        struct SimNode_Op2Glrf2VAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2Glrf2VAny ( const LineInfo & at, SimNode * rn, uint32_t sp, uint32_t ofs ) \
                : SimNode(at), r(rn), stackTop(sp), offset(ofs) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "Glrf2VAny", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_SP(offset); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                CTYPE lv = FUSION_OP_PTR_VALUE(CTYPE,((*(char **)(context.stack.sp() + stackTop)) + offset)); \
                auto rv =  r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
            uint32_t offset; \
        }; \
        struct SimNode_Op2AnyGlrf2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyGlrf2V ( const LineInfo & at, SimNode * ln, uint32_t sp, uint32_t ofs ) \
                : SimNode(at), l(ln), stackTop(sp), offset(ofs) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyGlrf2V", sizeof(CTYPE), #CTYPE); \
                V_SUB(l); \
                V_SP(stackTop); \
                V_SP(offset); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                auto rv =  FUSION_OP_PTR_VALUE(CTYPE,((*(char **)(context.stack.sp() + stackTop)) + offset)); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * l; \
            uint32_t stackTop; \
            uint32_t offset; \
        }; \
        struct SimNode_Op2R2VR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VR2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2ArgArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgArg ( const LineInfo & at, int32_t i_l, int32_t i_r ) \
                : SimNode(at),index_l(i_l), index_r(i_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgArg", sizeof(CTYPE), #CTYPE); \
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
        struct SimNode_Op2R2VAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VAny ( const LineInfo & at, SimNode * rn, uint32_t sp ) \
                : SimNode(at), r(rn), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VAny", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                auto rv =  r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2AnyR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyR2V ( const LineInfo & at, SimNode * ln, uint32_t sp ) \
                : SimNode(at), l(ln), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyR2V", sizeof(CTYPE), #CTYPE); \
                V_SUB(l); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                auto rv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * l; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2GlrfR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GlrfR2V ( const LineInfo & at, uint32_t sp_l, uint32_t ofs_l, uint32_t sp_r ) \
                : SimNode(at), stackTop_l(sp_l), offset_l(ofs_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfR2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_VALUE(CTYPE,((*(char **)(context.stack.sp() + stackTop_l)) + offset_l)); \
                auto rv = FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2ArgR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgR2V ( const LineInfo & at, int32_t i, uint32_t sp ) \
                : SimNode(at), index(i), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgR2V", sizeof(CTYPE), #CTYPE); \
                V_ARG(index); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index]); \
                auto rv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t  index; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2R2VArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VArg ( const LineInfo & at, int32_t i, uint32_t sp ) \
                : SimNode(at), index(i), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VArg", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_ARG(index); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                auto rv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t  index; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2R2VConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2R2VConst ( const LineInfo & at, CONSTTYPE cv, uint32_t sp ) \
                : SimNode(at), c(cv), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VConst", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            CONSTTYPE c; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ArgConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ArgConst ( const LineInfo & at, CONSTTYPE cv, int32_t i ) \
                : SimNode(at), c(cv), index(i) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgConst", sizeof(CTYPE), #CTYPE); \
                V_ARG(index); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            CONSTTYPE c; \
            int32_t index; \
        }; \
        struct SimNode_Op2AnyConst : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2AnyConst ( const LineInfo & at, SimNode * ll, CONSTTYPE cv ) \
                : SimNode(at), l(ll), c(cv) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyConst", sizeof(CTYPE), #CTYPE); \
                V_SUB(l); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto lv = l->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            SimNode * l; \
            CONSTTYPE c; \
        }; \
        struct SimNode_Op2ConstAny : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstAny ( const LineInfo & at, SimNode * rr, CONSTTYPE cv ) \
                : SimNode(at), r(rr), c(cv) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstAny", sizeof(CTYPE), #CTYPE); \
                V_ARG(c); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            SimNode * r; \
            CONSTTYPE c; \
        }; \
        struct SimNode_Op2ConstR2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstR2V ( const LineInfo & at, CONSTTYPE cv, uint32_t sp ) \
                : SimNode(at), c(cv), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstR2V", sizeof(CTYPE), #CTYPE); \
                V_ARG(c); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            CONSTTYPE c; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ConstArg : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2ConstArg ( const LineInfo & at, CONSTTYPE cv, int32_t i ) \
                : SimNode(at), c(cv), index(i) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstArg", sizeof(CTYPE), #CTYPE); \
                V_ARG(c); \
                V_ARG(index); \
                V_END(); \
            } \
            __forceinline RCTYPE compute ( Context & context ) { \
                auto rv = FUSION_OP_ARG_VALUE(CTYPE,context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            CONSTTYPE c; \
            int32_t index; \
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
                    return context->code->makeNode<SimNode_Op2R2VConst>(node->debugInfo, cvalue, r2vnode_l->stackTop); \
                /* OP(GetArgument,ConstValue) */ \
                } else if ( is(info,tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgConst>(node->debugInfo, cvalue, argnode_l->index); \
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
                    return context->code->makeNode<SimNode_Op2ConstR2V>(node->debugInfo, cvalue, r2vnode_r->stackTop); \
                /* OP(ConstValue,GetArgument) */ \
                } else if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstArg>(node->debugInfo, cvalue, argnode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2ConstAny>(node->debugInfo, tnode->r, cvalue); \
                } \
            /* OP(GetLocalR2V,*) */ \
            } else if ( is(info,tnode->l,"GetLocalR2V") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                /* OP(GetLocalR2V,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VR2V>(node->debugInfo, r2vnode_l->stackTop, r2vnode_r->stackTop); \
                /* OP(GetLocalR2V,GetLocalArg) */ \
                } else if ( is(info,tnode->r,"GetLocalArg") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VArg>(node->debugInfo, argnode_r->index, r2vnode_l->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2R2VAny>(node->debugInfo, tnode->r, r2vnode_l->stackTop); \
                } \
            /* OP(*,GetLocalR2V) */ \
            } else if ( is(info,tnode->r, "GetLocalR2V") ) { \
                auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                /* OP(GetArgument,GetLocalR2V) */ \
                if ( is(info,tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgR2V>(node->debugInfo, argnode_l->index, r2vnode_r->stackTop); \
                /* OP(GetLocalRefR2VOff,GetLocalR2V) */ \
                } else if ( is(info,tnode->l,"GetLocalRefR2VOff") ) { \
                    auto r2vonode_l = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2GlrfR2V>(node->debugInfo,r2vonode_l->stackTop, r2vonode_l->offset, r2vnode_r->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyR2V>(node->debugInfo, tnode->l, r2vnode_r->stackTop); \
                } \
            /* OP(GetArgument,* */ \
            } else if ( is(info,tnode->l,"GetArgument") ) { \
                auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                /* OP(GetArgument,GetArgument) */ \
                if ( is(info,tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ArgArg>(node->debugInfo, argnode_l->index, argnode_r->index); \
                } \
            /* OP(GetLocalRefR2VOff,*) */ \
            } else if ( is(info,tnode->l,"GetLocalRefR2VOff") ) { \
                auto r2vonode_l = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->l); \
                if ( is(info,tnode->r,"GetLocalRefR2VOff") ) { \
                    auto r2vonode_r = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2Glrf2VGlrf2V>(node->debugInfo, r2vonode_l->stackTop, r2vonode_l->offset, r2vonode_r->stackTop, r2vonode_r->offset); \
                } else { \
                    return context->code->makeNode<SimNode_Op2Glrf2VAny>(node->debugInfo, tnode->r, r2vonode_l->stackTop, r2vonode_l->offset); \
                } \
            /* OP(*,GetLocalRefR2VOff) */ \
            } else if ( is(info,tnode->r,"GetLocalRefR2VOff") ) { \
                auto r2vonode_r = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->r); \
                return context->code->makeNode<SimNode_Op2AnyGlrf2V>(node->debugInfo, tnode->l, r2vonode_r->stackTop, r2vonode_r->offset); \
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
        struct SimNode_Op2LocR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2LocR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "LocR2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_RVALUE(CTYPE,context.stack.sp() + stackTop_l); \
                auto rv = FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop_r); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2LocAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2LocAny ( const LineInfo & at, SimNode * rn, uint32_t sp ) \
                : SimNode(at), r(rn), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "LocAny", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,context.stack.sp() + stackTop); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2GlrfAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2GlrfAny ( const LineInfo & at, SimNode * rn, uint32_t sp, uint32_t ofs ) \
                : SimNode(at), r(rn), stackTop(sp), offset(ofs) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfAny", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop); \
                V_SP(offset); \
                V_SUB(r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.stack.sp() + stackTop)) + offset); \
                auto rv =  r->eval##TYPE(context); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
            uint32_t offset; \
        }; \
        struct SimNode_Op2GlrfGlrf2V : SimNode { \
            DAS_NODE(RTYPE,RCTYPE); \
            SimNode_Op2GlrfGlrf2V ( const LineInfo & at, uint32_t sp_l, uint32_t ofs_l, uint32_t sp_r, uint32_t ofs_r ) \
                : SimNode(at), stackTop_l(sp_l), offset_l(ofs_l), stackTop_r(sp_r), offset_r(ofs_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "GlrfGlrf2V", sizeof(CTYPE), #CTYPE); \
                V_SP(stackTop_l); \
                V_SP(offset_l); \
                V_SP(stackTop_r); \
                V_SP(offset_r); \
                V_END(); \
            } \
            __forceinline void compute ( Context & context ) { \
                auto lv = FUSION_OP_PTR_RVALUE(CTYPE,(*(char **)(context.stack.sp() + stackTop_l)) + offset_l); \
                auto rv = FUSION_OP_PTR_VALUE(CTYPE,(*(char **)(context.stack.sp() + stackTop_r)) + offset_r); \
                SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            uint32_t stackTop_r; \
            uint32_t offset_r; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op2 *>(node); \
            /* OP(GetLocal,*) */ \
            if ( is(info,tnode->l,"GetLocal") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocal *>(tnode->l); \
                /* OP(GetLocal,GetLocalR2V) */ \
                if ( is(info,tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2LocR2V>(node->debugInfo, r2vnode_l->stackTop, r2vnode_r->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2LocAny>(node->debugInfo, tnode->r, r2vnode_l->stackTop); \
                } \
            /* OP(GetLocalRefOff,*) */ \
            } else if ( is(info,tnode->l,"GetLocalRefOff") ) { \
                auto glrfnode_l = static_cast<SimNode_GetLocalRefOff *>(tnode->l); \
                /* OP(GetLocalRefOff,GetLocalRefR2VOff) */ \
                if ( is(info,tnode->r,"GetLocalRefR2VOff") ) { \
                    auto r2vonode_r = static_cast<SimNode_GetLocalRefR2VOff<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2GlrfGlrf2V>(node->debugInfo, glrfnode_l->stackTop, glrfnode_l->offset, r2vonode_r->stackTop, r2vonode_r->offset); \
                } else { \
                    return context->code->makeNode<SimNode_Op2GlrfAny>(node->debugInfo, tnode->r, glrfnode_l->stackTop, glrfnode_l->offset); \
                } \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_SET_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    IMPLEMENT_SET_OP2_FUSION_POINT(OPNAME,,CTYPE)

#define REGISTER_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,#CTYPE)].push_back(make_shared<Op2FusionPoint_##OPNAME##_##CTYPE>());

#define REGISTER_OP2_VEC_FUSION_POINT(OPNAME,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,#CTYPE)].push_back(make_shared<Op2FusionPoint_##OPNAME##_##CTYPE>());

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
