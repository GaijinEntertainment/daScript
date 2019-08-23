#pragma once

#define FUSION_OP2_PTR(CTYPE,expr)              (((CTYPE *)(expr)))
#define FUSION_OP2_RVALUE(CTYPE,expr)           (*((CTYPE *)(expr)))
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
                auto rr = FUSION_OP2_RVALUE(CTYPE,r.compute##COMPUTER(context)); \
                SimPolicy<CTYPE>::OPNAME(FUSION_OP2_PTR_TO_LVALUE(pl),rr,context); \
                return v_zero(); \
            } \
        }; \

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
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,AnyPtr); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,CMResOfs); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,LocalRefOff); \
        IMPLEMENT_OP2_SET_NODE_ANY(OPNAME,TYPE,CTYPE,ArgumentRef); \
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode *, SimNode * node_l, SimNode * node_r, Context * context) override { \
            if ( false ) {} \
            MATCH_OP2_SET(OPNAME,"GetLocal","ConstValue",Local,Const) \
            MATCH_OP2_SET(OPNAME,"GetLocal","GetLocalR2V",Local,Local) \
            MATCH_OP2_SET_ANY(OPNAME,"GetLocal",Local) \
            MATCH_OP2_SET_ANY(OPNAME,"GetCMResOfs",CMResOfs) \
            MATCH_OP2_SET_ANY(OPNAME,"GetLocalRefOff",LocalRefOff) \
            MATCH_OP2_SET_ANY(OPNAME,"GetArgument",ArgumentRef) \
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



