#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/ast/ast_typedecl.h"

namespace das {

#define FUSION_OP_PTR_VALUE(CTYPE,expr)    *((CTYPE *)((expr)))
#define FUSION_OP_PTR_RVALUE(CTYPE,expr)   ((CTYPE *)((expr)))
#define FUSION_OP_EVAL_CAST(expr)          (*(expr))

#define IMPLEMENT_ANY_OP1_NODE(INLINE,OPNAME,TYPE,CTYPE,COMPUTE) \
    struct SimNode_Op1##COMPUTE : SimNode_Op1Fusion { \
            INLINE auto compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_VALUE(CTYPE,subexpr.compute##COMPUTE(context)); \
                return SimPolicy<CTYPE>::OPNAME(lv,context); \
            } \
            DAS_NODE(TYPE,CTYPE); \
        }; 

#define IMPLEMENT_ANY_OP1_SET_NODE(INLINE,OPNAME,TYPE,CTYPE,COMPUTE) \
    struct SimNode_Op1##COMPUTE : SimNode_Op1Fusion { \
            INLINE auto compute ( Context & context ) { \
                auto lv =  FUSION_OP_PTR_RVALUE(CTYPE,subexpr.compute##COMPUTE(context)); \
                return SimPolicy<CTYPE>::OPNAME(FUSION_OP_EVAL_CAST(lv),context); \
            } \
            DAS_NODE(TYPE,CTYPE); \
        }; 

#define MATCH_ANY_OP1_NODE(NODENAME,COMPUTE) \
    else if ( is(info,tnode->x,NODENAME) ) { result = context->code->makeNode<SimNode_Op1##COMPUTE>(); } \

#define IMPLEMENT_ANY_OP1_FUSION_POINT(INLINE,OPNAME,TYPE,CTYPE) \
    struct Op1FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op1FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        IMPLEMENT_ANY_OP1_NODE(INLINE,OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_ANY_OP1_NODE(INLINE,OPNAME,TYPE,CTYPE,Argument); \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            SimNode_Op1Fusion * result = nullptr; \
            auto tnode = static_cast<SimNode_Op1 *>(node); \
            if ( false ) {} \
            MATCH_ANY_OP1_NODE("GetLocalR2V",Local) \
            MATCH_ANY_OP1_NODE("GetArgument",Argument) \
            if ( result ) { \
                result->set(#OPNAME,Type(ToBasicType<CTYPE>::type),tnode->debugInfo); \
                SimNode_SourceBase * sbnode = static_cast<SimNode_SourceBase *>(tnode->x); \
                result->subexpr = sbnode->subexpr; \
                return result; \
            } else { \
                return node; \
            } \
        } \
    };

#define IMPLEMENT_SET_OP1_FUSION_POINT(INLINE,OPNAME,TYPE,CTYPE) \
    struct Op1FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op1FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        IMPLEMENT_ANY_OP1_SET_NODE(INLINE,OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_ANY_OP1_SET_NODE(INLINE,OPNAME,TYPE,CTYPE,Argument); \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            SimNode_Op1Fusion * result = nullptr; \
            auto tnode = static_cast<SimNode_Op1 *>(node); \
            if ( false ) {} \
            MATCH_ANY_OP1_NODE("GetLocal",Local) \
            MATCH_ANY_OP1_NODE("GetArgument",Argument) \
            MATCH_ANY_OP1_NODE("GetArgumentRef",Argument) \
            if ( result ) { \
                result->set(#OPNAME,Type(ToBasicType<CTYPE>::type),tnode->debugInfo); \
                SimNode_SourceBase * sbnode = static_cast<SimNode_SourceBase *>(tnode->x); \
                result->subexpr = sbnode->subexpr; \
                return result; \
            } else { \
                return node; \
            } \
        } \
    };

#define REGISTER_OP1_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<Op1FusionPoint_##OPNAME##_##CTYPE>());

// int and numeric

#define IMPLEMENT_OP1_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_ANY_OP1_FUSION_POINT(__forceinline,OPNAME,Int,int32_t); \
    IMPLEMENT_ANY_OP1_FUSION_POINT(__forceinline,OPNAME,UInt,uint32_t); \
    IMPLEMENT_ANY_OP1_FUSION_POINT(_msc_inline_bug,OPNAME,Int64,int64_t); \
    IMPLEMENT_ANY_OP1_FUSION_POINT(_msc_inline_bug,OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP1_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP1_INTEGER_FUSION_POINT(OPNAME); \
   IMPLEMENT_ANY_OP1_FUSION_POINT(__forceinline,OPNAME,Float,float); \
   IMPLEMENT_ANY_OP1_FUSION_POINT(__forceinline,OPNAME,Double,double);

// set int and numeric

#define IMPLEMENT_OP1_SET_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_SET_OP1_FUSION_POINT(__forceinline,OPNAME,Int,int32_t); \
    IMPLEMENT_SET_OP1_FUSION_POINT(__forceinline,OPNAME,UInt,uint32_t); \
    IMPLEMENT_SET_OP1_FUSION_POINT(_msc_inline_bug,OPNAME,Int64,int64_t); \
    IMPLEMENT_SET_OP1_FUSION_POINT(_msc_inline_bug,OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP1_SET_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_SET_OP1_FUSION_POINT(__forceinline,OPNAME,Float,float); \
    IMPLEMENT_SET_OP1_FUSION_POINT(__forceinline,OPNAME,Double,double);

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

    // unary
    IMPLEMENT_OP1_NUMERIC_FUSION_POINT(Unm);
    IMPLEMENT_OP1_NUMERIC_FUSION_POINT(Unp);
    // binary
    IMPLEMENT_OP1_INTEGER_FUSION_POINT(BinNot);
    // boolean
    IMPLEMENT_ANY_OP1_FUSION_POINT(__forceinline,BoolNot,Bool,bool);
    // inc and dec
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(Inc);
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(Dec);
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(IncPost);
    IMPLEMENT_OP1_SET_NUMERIC_FUSION_POINT(DecPost);

    void createFusionEngine_op1()
    {
        // unary
        REGISTER_OP1_NUMERIC_FUSION_POINT(Unm);
        REGISTER_OP1_NUMERIC_FUSION_POINT(Unp);
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
