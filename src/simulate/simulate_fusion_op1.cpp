#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

// (BinNot_TT<uint> (GetLocalR2V_TT<uint> #448)
// (Inc_TT<int> (GetLocal #48))

#define FUSION_OP_PTR_VALUE(CTYPE,expr)    *((CTYPE *)((expr)))
#define FUSION_OP_PTR_RVALUE(CTYPE,expr)   ((CTYPE *)((expr)))
#define FUSION_OP_EVAL_CAST(expr)          (*(expr))

#define IMPLEMENT_ANY_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE,RTYPE) \
    struct Op1FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op1FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        struct SimNode_Op1R2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op1R2V ( const LineInfo & at, uint32_t sp ) \
                : SimNode(at),stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2V", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,context); \
            } \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op1Arg : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op1Arg ( const LineInfo & at, int32_t idx ) \
                : SimNode(at),index(idx) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "Arg", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(index); \
                V_END(); \
            } \
            __forceinline RTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,context.abiArguments()+index); \
                return SimPolicy<CTYPE>::OPNAME (lv,context); \
            } \
            int32_t index; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op1 *>(node); \
            /* OP(GetLocalR2V,*) */ \
            if ( is(info,tnode->x,"GetLocalR2V") ) { \
                auto r2vnode = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->x); \
                return context->code->makeNode<SimNode_Op1R2V>(node->debugInfo, r2vnode->stackTop); \
            /* OP(GetLocalR2V,*) */ \
            } else if ( is(info,tnode->x,"GetArgument") ) { \
                auto argnode = static_cast<SimNode_GetArgument *>(tnode->x); \
                return context->code->makeNode<SimNode_Op1Arg>(node->debugInfo, argnode->index); \
            } \
            return node; \
        } \
    };

#define IMPLEMENT_SET_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE,RTYPE) \
    struct Op1FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op1FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        struct SimNode_SetOp1Loc : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_SetOp1Loc ( const LineInfo & at, uint32_t sp ) \
                : SimNode(at),stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "Loc", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline RTYPE compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (FUSION_OP_EVAL_CAST(lv),context); \
            } \
            uint32_t stackTop; \
        }; \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op1 *>(node); \
            /* OP(GetLocal,*) */ \
            if ( is(info,tnode->x,"GetLocal") ) { \
                auto r2vnode = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->x); \
                return context->code->makeNode<SimNode_SetOp1Loc>(node->debugInfo, r2vnode->stackTop); \
            } \
            return node; \
        } \
    };

#define REGISTER_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<Op1FusionPoint_##OPNAME##_##CTYPE>());

// int and numeric

#define IMPLEMENT_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    IMPLEMENT_ANY_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE,CTYPE)

#define IMPLEMENT_OP1_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP1_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_OP1_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_OP1_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_OP1_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP1_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP1_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP1_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_OP1_FUSION_POINT(OPNAME,Double,double);

// set int and numeric

#define IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    IMPLEMENT_SET_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE,CTYPE)

#define IMPLEMENT_OP1_SET_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP1_SET_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_OP1_SET_FUSION_POINT(OPNAME,Double,double);

// register

#define REGISTER_OP1_INTEGER_FUSION_POINT(OPNAME) \
    REGISTER_OP1_FUSION_POINT(OPNAME,Int,int32_t); \
    REGISTER_OP1_FUSION_POINT(OPNAME,UInt,uint32_t); \
    REGISTER_OP1_FUSION_POINT(OPNAME,Int64,int64_t); \
    REGISTER_OP1_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define REGISTER_OP1_NUMERIC_FUSION_POINT(OPNAME) \
    REGISTER_OP1_INTEGER_FUSION_POINT(OPNAME); \
    REGISTER_OP1_FUSION_POINT(OPNAME,Float,float); \
    REGISTER_OP1_FUSION_POINT(OPNAME,Double,double);


    // binary
    IMPLEMENT_OP1_INTEGER_FUSION_POINT(BinNot);
    // boolean
    IMPLEMENT_ANY_OP1_FUSION_POINT(BoolNot,Bool,bool,bool);
    // inc and dec
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(Inc);
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(Dec);
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(IncPost);
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(DecPost);

    void createFusionEngine_op1()
    {
        // binary
        REGISTER_OP1_INTEGER_FUSION_POINT(BinNot);
        // boolean
        REGISTER_OP1_FUSION_POINT(BoolNot,Bool,bool);
        // inc and dec
        REGISTER_OP1_NUMERIC_FUSION_POINT(Inc);
        REGISTER_OP1_NUMERIC_FUSION_POINT(Dec);
        REGISTER_OP1_NUMERIC_FUSION_POINT(IncPost);
        REGISTER_OP1_NUMERIC_FUSION_POINT(DecPost);
    }

}
