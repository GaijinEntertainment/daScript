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

#define MATCH_OP2_SET(OPNAME,LNODENAME,RNODENAME,COMPUTEL,COMPUTER) \
    else if ( is(info, node_l,LNODENAME) &&  is(info,node_r,RNODENAME) ) { \
        return context->code->makeNode<SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER>(); \
    } 

#define MATCH_OP2_SET_ANY(OPNAME,LNODENAME,COMPUTEL) \
    else if ( is(info, node_l,LNODENAME) ) { \
        anyRight = true; \
        return context->code->makeNode<SimNode_##OPNAME##_##COMPUTEL##_Any>(); \
    } 

    //  SimPolicy::Set##OPNAME(a,b)
#define IMPLEMENT_ANY_SETOP(OPNAME,TYPE,CTYPE) \
    struct FusionPoint_Set_##OPNAME##_##CTYPE : FusionPointOp2 { \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,Local,Const); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,Local,Local); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,Local,LocalRefOff); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,LocalRefOff,LocalRefOff); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,LocalRefOff,Argument); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,LocalRefOff,Const); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,ArgumentRefOff,ArgumentRefOff); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,ArgumentRefOff,Local); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,ArgumentRef,Argument); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,ThisBlockArgumentRef,ThisBlockArgument); \
        IMPLEMENT_OP2_SET_NODE(OPNAME,TYPE,CTYPE,ThisBlockArgumentRef,ThisBlockArgumentRef); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,AnyPtr); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,CMResOfs); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,LocalRefOff); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,ArgumentRef); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,ArgumentRefOff); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,ThisBlockArgumentRef); \
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode *, SimNode * node_l, SimNode * node_r, Context * context) override { \
            /* match set */ if ( false ) {} \
            MATCH_OP2_SET(OPNAME,"GetLocal","ConstValue",Local,Const) \
            MATCH_OP2_SET(OPNAME,"GetLocal","GetLocalR2V",Local,Local) \
            MATCH_OP2_SET(OPNAME,"GetLocal","GetLocalRefOffR2V",Local,LocalRefOff) \
            MATCH_OP2_SET(OPNAME,"GetLocalRefOff","GetLocalRefOffR2V",LocalRefOff,LocalRefOff) \
            MATCH_OP2_SET(OPNAME,"GetLocalRefOff","GetArgument",LocalRefOff,Argument) \
            MATCH_OP2_SET(OPNAME,"GetLocalRefOff","ConstValue",LocalRefOff,Const) \
            MATCH_OP2_SET(OPNAME,"GetArgumentRefOff","GetArgumentRefOffR2V",ArgumentRefOff,ArgumentRefOff) \
            MATCH_OP2_SET(OPNAME,"GetArgumentRefOff","GetLocalR2V",ArgumentRefOff,Local) \
            MATCH_OP2_SET(OPNAME,"GetArgument","GetArgument",ArgumentRef,Argument) \
            MATCH_OP2_SET(OPNAME,"GetThisBlockArgument","GetThisBlockArgument",ThisBlockArgumentRef,ThisBlockArgument) \
            MATCH_OP2_SET(OPNAME,"GetThisBlockArgument","GetThisBlockArgumentR2V",ThisBlockArgumentRef,ThisBlockArgumentRef) \
            MATCH_OP2_SET_ANY(OPNAME,"GetLocal",Local) \
            MATCH_OP2_SET_ANY(OPNAME,"GetCMResOfs",CMResOfs) \
            MATCH_OP2_SET_ANY(OPNAME,"GetLocalRefOff",LocalRefOff) \
            MATCH_OP2_SET_ANY(OPNAME,"GetArgument",ArgumentRef) \
            MATCH_OP2_SET_ANY(OPNAME,"GetArgumentRefOff",ArgumentRefOff) \
            MATCH_OP2_SET_ANY(OPNAME,"GetThisBlockArgument",ThisBlockArgumentRef) \
            return nullptr; \
        } \
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) override { \
            result->set(#OPNAME,Type(ToBasicType<CTYPE>::type),node->debugInfo); \
        } \
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override { \
            auto cnode = static_cast<SimNode_Set<CTYPE> *>(node); \
            return fuseOp2(info, node, cnode->l, cnode->r, context); \
        } \
    };

#define IMPLEMENT_VEC_SETOP(CTYPE) \
    IMPLEMENT_ANY_SETOP_VALUE(,CTYPE)

#define REGISTER_SETOP(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<FusionPoint_Set_##OPNAME##_##CTYPE>());

#define IMPLEMENT_SETOP_INTEGER(OPNAME) \
    IMPLEMENT_ANY_SETOP(OPNAME,Int,int32_t); \
    IMPLEMENT_ANY_SETOP(OPNAME,UInt,uint32_t); \
    IMPLEMENT_ANY_SETOP(OPNAME,Int64,int64_t); \
    IMPLEMENT_ANY_SETOP(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_SETOP_NUMERIC(OPNAME) \
    IMPLEMENT_SETOP_INTEGER(OPNAME); \
    IMPLEMENT_ANY_SETOP(OPNAME,Float,float); \
    IMPLEMENT_ANY_SETOP(OPNAME,Double,double);

#define IMPLEMENT_SETOP_SCALAR(OPNAME) \
    IMPLEMENT_ANY_SETOP(OPNAME,Bool,bool); \
    IMPLEMENT_SETOP_NUMERIC(OPNAME);

#define REGISTER_SETOP_INTEGER(OPNAME) \
    REGISTER_SETOP(OPNAME,Int,int32_t); \
    REGISTER_SETOP(OPNAME,UInt,uint32_t); \
    REGISTER_SETOP(OPNAME,Int64,int64_t); \
    REGISTER_SETOP(OPNAME,UInt64,uint64_t);

#define REGISTER_SETOP_NUMERIC(OPNAME) \
    REGISTER_SETOP_INTEGER(OPNAME); \
    REGISTER_SETOP(OPNAME,Float,float); \
    REGISTER_SETOP(OPNAME,Double,double);

#define REGISTER_SETOP_SCALAR(OPNAME) \
    REGISTER_SETOP(OPNAME,Bool,bool); \
    REGISTER_SETOP_NUMERIC(OPNAME); 

#define IMPLEMENT_SETOP_VEC(OPNAME,CTYPE) \
    IMPLEMENT_ANY_SETOP(OPNAME,,CTYPE)

#define IMPLEMENT_SETOP_INTEGER_VEC(OPNAME) \
    IMPLEMENT_SETOP_VEC(OPNAME,int2 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,uint2); \
    IMPLEMENT_SETOP_VEC(OPNAME,int3 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,uint3); \
    IMPLEMENT_SETOP_VEC(OPNAME,int4 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,uint4);

#define IMPLEMENT_SETOP_NUMERIC_VEC(OPNAME)  \
    IMPLEMENT_SETOP_INTEGER_VEC(OPNAME); \
    IMPLEMENT_SETOP_VEC(OPNAME,float2 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,float3 ); \
    IMPLEMENT_SETOP_VEC(OPNAME,float4 );

#define REGISTER_SETOP_VEC(OPNAME,CTYPE) \
    REGISTER_SETOP(OPNAME,,CTYPE)

#define REGISTER_SETOP_INTEGER_VEC(OPNAME) \
    REGISTER_SETOP_VEC(OPNAME,int2 ); \
    REGISTER_SETOP_VEC(OPNAME,uint2); \
    REGISTER_SETOP_VEC(OPNAME,int3 ); \
    REGISTER_SETOP_VEC(OPNAME,uint3); \
    REGISTER_SETOP_VEC(OPNAME,int4 ); \
    REGISTER_SETOP_VEC(OPNAME,uint4);

#define REGISTER_SETOP_NUMERIC_VEC(OPNAME) \
    REGISTER_SETOP_INTEGER_VEC(OPNAME); \
    REGISTER_SETOP_VEC(OPNAME,float2 ); \
    REGISTER_SETOP_VEC(OPNAME,float3 ); \
    REGISTER_SETOP_VEC(OPNAME,float4 );

// OP(COMPUTEL,*) 
#define IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,COMPUTEL) \
    struct SimNode_##OPNAME##_Any_##COMPUTEL : SimNode_Op2Fusion { \
        __forceinline auto compute ( Context & context ) { \
            auto ll = l.subexpr->eval##TYPE(context); \
            auto rr = FUSION_OP2_RVALUE_RIGHT(CTYPE,r.compute##COMPUTEL(context)); \
            return SimPolicy<CTYPE>::OPNAME(ll,rr,context); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

// OP((*,COMPUTER)
#define IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTER##_Any : SimNode_Op2Fusion { \
        __forceinline auto compute ( Context & context ) { \
            auto ll = FUSION_OP2_RVALUE_LEFT (CTYPE,l.compute##COMPUTER(context)); \
            auto rr = r.subexpr->eval##TYPE(context); \
            return SimPolicy<CTYPE>::OPNAME(ll,rr,context); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

#define IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,COMPUTEL,COMPUTER) \
    struct SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER : SimNode_Op2Fusion { \
        __forceinline auto compute ( Context & context ) { \
            auto ll = FUSION_OP2_RVALUE_LEFT (CTYPE,l.compute##COMPUTEL(context)); \
            auto rr = FUSION_OP2_RVALUE_RIGHT(CTYPE,r.compute##COMPUTER(context)); \
            return SimPolicy<CTYPE>::OPNAME(ll,rr,context); \
        } \
        DAS_NODE(TYPE,CTYPE); \
    }; 

#define MATCH_OP2(OPNAME,LNODENAME,RNODENAME,COMPUTEL,COMPUTER) \
    else if ( is(info,node_l,LNODENAME) &&  is(info,node_r,RNODENAME) ) { \
        return context->code->makeNode<SimNode_##OPNAME##_##COMPUTEL##_##COMPUTER>(); \
    } 

#define MATCH_OP2_ANYR(OPNAME,LNODENAME,COMPUTEL) \
    else if ( is(info,node_l,LNODENAME) ) { \
        anyRight = true; \
        return context->code->makeNode<SimNode_##OPNAME##_##COMPUTEL##_Any>(); \
    } 

#define MATCH_OP2_ANYL(OPNAME,RNODENAME,COMPUTER) \
    else if ( is(info,node_r,RNODENAME) ) { \
        anyLeft = true; \
        return context->code->makeNode<SimNode_##OPNAME##_Any_##COMPUTER>(); \
    } 

//  SimPolicy::OPNAME(a,b)
#define IMPLEMENT_ANY_OP2(OPNAME,TYPE,CTYPE) \
    struct FusionPoint_##OPNAME##_##CTYPE : FusionPointOp2 { \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,ThisBlockArgumentRef,ThisBlockArgument); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,ThisBlockArgument,ThisBlockArgument); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,ThisBlockArgument,Argument); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Argument,Argument); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Argument,Local); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Argument,Const); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,LocalRefOff,LocalRefOff) \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,LocalRefOff,Local) \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,LocalRefOff,Argument) \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Local,ThisBlockArgumentRef); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Local,Local); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Local,Argument); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Local,Const); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Const,Local); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Const,LocalRefOff); \
        IMPLEMENT_OP2_NODE(OPNAME,TYPE,CTYPE,Const,Argument); \
        IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,Argument); \
        IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,Argument); \
        IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,ArgumentRefOff); \
        IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,ArgumentRefOff); \
        IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,ThisBlockArgument); \
        IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,ThisBlockArgument); \
        IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,Const); \
        IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,Const); \
        IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_OP2_NODE_ANYR(OPNAME,TYPE,CTYPE,LocalRefOff); \
        IMPLEMENT_OP2_NODE_ANYL(OPNAME,TYPE,CTYPE,LocalRefOff); \
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode *, SimNode * node_l, SimNode * node_r, Context * context) override { \
            /* match op2 */ if ( false ) {} \
            MATCH_OP2(OPNAME,"GetThisBlockArgumentR2V","GetThisBlockArgument",ThisBlockArgumentRef,ThisBlockArgument) \
            MATCH_OP2(OPNAME,"GetThisBlockArgument","GetThisBlockArgument",ThisBlockArgument,ThisBlockArgument) \
            MATCH_OP2(OPNAME,"GetThisBlockArgument","GetArgument",ThisBlockArgument,Argument) \
            \
            MATCH_OP2(OPNAME,"GetArgument","GetArgument",Argument,Argument) \
            MATCH_OP2(OPNAME,"GetArgument","GetLocalR2V",Argument,Local) \
            MATCH_OP2(OPNAME,"GetArgument","ConstValue",Argument,Const) \
            \
            MATCH_OP2(OPNAME,"GetLocalRefOffR2V","GetLocalRefOffR2V",LocalRefOff,LocalRefOff) \
            MATCH_OP2(OPNAME,"GetLocalRefOffR2V","GetLocalR2V",LocalRefOff,Local) \
            MATCH_OP2(OPNAME,"GetLocalRefOffR2V","GetArgument",LocalRefOff,Argument) \
            \
            MATCH_OP2(OPNAME,"GetLocalR2V","GetLocalR2V",Local,Local) \
            MATCH_OP2(OPNAME,"GetLocalR2V","GetThisBlockArgumentR2V",Local,ThisBlockArgumentRef) \
            MATCH_OP2(OPNAME,"GetLocalR2V","GetArgument",Local,Argument) \
            MATCH_OP2(OPNAME,"GetLocalR2V","ConstValue",Local,Const) \
            \
            MATCH_OP2(OPNAME,"ConstValue","GetLocalR2V",Const,Local) \
            MATCH_OP2(OPNAME,"ConstValue","GetLocalRefOffR2V",Const,LocalRefOff) \
            MATCH_OP2(OPNAME,"ConstValue","GetArgument",Const,Argument) \
            \
            MATCH_OP2_ANYR(OPNAME,"GetArgument",Argument) \
            MATCH_OP2_ANYL(OPNAME,"GetArgument",Argument) \
            MATCH_OP2_ANYR(OPNAME,"GetArgumentRefOffR2V",ArgumentRefOff) \
            MATCH_OP2_ANYL(OPNAME,"GetArgumentRefOffR2V",ArgumentRefOff) \
            MATCH_OP2_ANYR(OPNAME,"GetThisBlockArgument",ThisBlockArgument) \
            MATCH_OP2_ANYL(OPNAME,"GetThisBlockArgument",ThisBlockArgument) \
            MATCH_OP2_ANYR(OPNAME,"ConstValue",Const) \
            MATCH_OP2_ANYL(OPNAME,"ConstValue",Const) \
            MATCH_OP2_ANYR(OPNAME,"GetLocalR2V",Local) \
            MATCH_OP2_ANYL(OPNAME,"GetLocalR2V",Local) \
            MATCH_OP2_ANYR(OPNAME,"GetLocalRefOffR2V",LocalRefOff) \
            MATCH_OP2_ANYL(OPNAME,"GetLocalRefOffR2V",LocalRefOff) \
            return nullptr; \
        } \
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) override { \
            result->set(#OPNAME,Type(ToBasicType<CTYPE>::type),node->debugInfo); \
        } \
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override { \
            auto cnode = static_cast<SimNode_Set<CTYPE> *>(node); \
            return fuseOp2(info, node, cnode->l, cnode->r, context); \
        } \
    };

#define IMPLEMENT_VEC_OP2(CTYPE) \
    IMPLEMENT_ANY_OP2(,CTYPE)

#define REGISTER_OP2(OPNAME,TYPE,CTYPE) \
    (*g_fusionEngine)[fuseName(#OPNAME,typeName<CTYPE>::name())].push_back(make_shared<FusionPoint_##OPNAME##_##CTYPE>());

#define IMPLEMENT_OP2_INTEGER(OPNAME) \
    IMPLEMENT_ANY_OP2(OPNAME,Int,int32_t); \
    IMPLEMENT_ANY_OP2(OPNAME,UInt,uint32_t); \
    IMPLEMENT_ANY_OP2(OPNAME,Int64,int64_t); \
    IMPLEMENT_ANY_OP2(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP2_NUMERIC(OPNAME) \
    IMPLEMENT_OP2_INTEGER(OPNAME); \
    IMPLEMENT_ANY_OP2(OPNAME,Float,float); \
    IMPLEMENT_ANY_OP2(OPNAME,Double,double);

#define IMPLEMENT_OP2_SCALAR(OPNAME) \
    IMPLEMENT_ANY_OP2(OPNAME,Bool,bool); \
    IMPLEMENT_OP2_NUMERIC(OPNAME);

#define REGISTER_OP2_INTEGER(OPNAME) \
    REGISTER_OP2(OPNAME,Int,int32_t); \
    REGISTER_OP2(OPNAME,UInt,uint32_t); \
    REGISTER_OP2(OPNAME,Int64,int64_t); \
    REGISTER_OP2(OPNAME,UInt64,uint64_t);

#define REGISTER_OP2_NUMERIC(OPNAME) \
    REGISTER_OP2_INTEGER(OPNAME); \
    REGISTER_OP2(OPNAME,Float,float); \
    REGISTER_OP2(OPNAME,Double,double);

#define REGISTER_OP2_SCALAR(OPNAME) \
    REGISTER_OP2(OPNAME,Bool,bool); \
    REGISTER_OP2_NUMERIC(OPNAME); 

#define IMPLEMENT_OP2_VEC(OPNAME,CTYPE) \
    IMPLEMENT_ANY_OP2(OPNAME,,CTYPE)

#define IMPLEMENT_OP2_INTEGER_VEC(OPNAME) \
    IMPLEMENT_OP2_VEC(OPNAME,int2 ); \
    IMPLEMENT_OP2_VEC(OPNAME,uint2); \
    IMPLEMENT_OP2_VEC(OPNAME,int3 ); \
    IMPLEMENT_OP2_VEC(OPNAME,uint3); \
    IMPLEMENT_OP2_VEC(OPNAME,int4 ); \
    IMPLEMENT_OP2_VEC(OPNAME,uint4);

#define IMPLEMENT_OP2_NUMERIC_VEC(OPNAME)  \
    IMPLEMENT_OP2_INTEGER_VEC(OPNAME); \
    IMPLEMENT_OP2_VEC(OPNAME,float2 ); \
    IMPLEMENT_OP2_VEC(OPNAME,float3 ); \
    IMPLEMENT_OP2_VEC(OPNAME,float4 );

#define REGISTER_OP2_VEC(OPNAME,CTYPE) \
    REGISTER_OP2(OPNAME,,CTYPE)

#define REGISTER_OP2_INTEGER_VEC(OPNAME) \
    REGISTER_OP2_VEC(OPNAME,int2 ); \
    REGISTER_OP2_VEC(OPNAME,uint2); \
    REGISTER_OP2_VEC(OPNAME,int3 ); \
    REGISTER_OP2_VEC(OPNAME,uint3); \
    REGISTER_OP2_VEC(OPNAME,int4 ); \
    REGISTER_OP2_VEC(OPNAME,uint4);

#define REGISTER_OP2_NUMERIC_VEC(OPNAME) \
    REGISTER_OP2_INTEGER_VEC(OPNAME); \
    REGISTER_OP2_VEC(OPNAME,float2 ); \
    REGISTER_OP2_VEC(OPNAME,float3 ); \
    REGISTER_OP2_VEC(OPNAME,float4 );



