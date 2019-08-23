#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/misc/copy_bytes.h"
#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/ast/ast_typedecl.h"

namespace das {

    //  &a = b
    struct FusionPointOp2 : FusionPoint {
        FusionPointOp2() {}
        virtual SimNode * match(const SimNodeInfoLookup &, SimNode *, SimNode *, SimNode *, Context *) {
            return nullptr;
        }
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) = 0;
        virtual SimNode * fuseOp2(const SimNodeInfoLookup & info, SimNode * node, SimNode * node_l, SimNode * node_r, Context * context) {
            anyLeft = anyRight = false;
            SimNode_Op2Fusion * result = (SimNode_Op2Fusion *) match(info,node,node_l,node_r,context);
            if ( result ) { 
                set(result,node); 
                if ( !anyLeft && node_l->rtti_isSourceBase() ) {
                    result->l = static_cast<SimNode_SourceBase *>(node_l)->subexpr;
                } else {
                    result->l.setSimNode(node_l);
                }
                if ( !anyRight && node_r->rtti_isSourceBase() ) {
                    result->r = static_cast<SimNode_SourceBase *>(node_r)->subexpr;
                } else {
                    result->r.setSimNode(node_r);
                }
                return result; 
            } else { 
                return node; 
            } 
        }
        bool anyLeft, anyRight;
    };

    // copy reference
    //  &a = b
    struct FusionPoint_MiscCopyReference : FusionPointOp2 {
        FusionPoint_MiscCopyReference() {}
        struct SimNode_CopyReferenceLocAny : SimNode_Op2Fusion {
            virtual vec4f eval ( Context & context ) override {
                char  ** pl = (char **)l.computeLocal(context);
                char * pr   = r.computeAnyPtr(context);
                *pl = pr;
                return v_zero();
            }
        };
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode *, SimNode * node_l, SimNode *, Context * context) override {
            if (false) {}
            else if ( is(info, node_l, "GetLocal" )) { anyRight = true; return context->code->makeNode<SimNode_CopyReferenceLocAny>();  }
            return nullptr;
        }
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) override {
            result->set("CopyReference",Type::none,node->debugInfo); 
        }
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override {
            SimNode_CopyReference * crnode = static_cast<SimNode_CopyReference *>(node);
            return fuseOp2(info, node, crnode->l, crnode->r, context);
        }
    };

    struct SimNode_Op2FusionCopyRef : SimNode_Op2Fusion {
        uint32_t size;
    };

#define IMPLEMENT_OP2_COPYREF_NODE(COMPUTEL,COMPUTER) \
    template <int typeSize> \
    struct SimNode_CopyRefValueFixed_##COMPUTEL##_##COMPUTER : SimNode_Op2FusionCopyRef { \
        virtual vec4f eval ( Context & context ) override { \
            auto pl = l.compute##COMPUTEL(context); \
            auto pr = r.compute##COMPUTER(context); \
            CopyBytes<typeSize>::copy(pl, pr); \
            return v_zero(); \
        } \
    }; \
    struct SimNode_CopyRefValue_##COMPUTEL##_##COMPUTER : SimNode_Op2FusionCopyRef { \
        virtual vec4f eval ( Context & context ) override { \
            auto pl = l.compute##COMPUTEL(context); \
            auto pr = r.compute##COMPUTER(context); \
            memcpy(pl, pr, size); \
            return v_zero(); \
        } \
    };

#define MATCH_OP2_COPYREF_NODE(COMPUTEL,COMPUTER) \
    if (isFastCopyBytes(size)) { \
        return context->code->makeNodeUnroll<SimNode_CopyRefValueFixed_##COMPUTEL##_##COMPUTER>(size); \
    } else { \
        return context->code->makeNode<SimNode_CopyRefValue_##COMPUTEL##_##COMPUTER>(); \
    }

#define MATCH_OP2_COPYREF_LEFT_ANY(NODENAME,COMPUTEL) \
    else if (is(info, node_l, NODENAME) ) { \
        anyRight = true; \
        MATCH_OP2_COPYREF_NODE(COMPUTEL,AnyPtr); \
    } 

#define MATCH_OP2_COPYREF(LNODENAME,RNODENAME,COMPUTEL,COMPUTER) \
    else if (is(info, node_l, LNODENAME) && is(info, node_r, RNODENAME)) { \
        MATCH_OP2_COPYREF_NODE(COMPUTEL,COMPUTER); \
    } 

    // copy reference value
    //  *a = *b
    struct FusionPoint_MiscCopyRefValue : FusionPointOp2 {
        FusionPoint_MiscCopyRefValue() {}
        IMPLEMENT_OP2_COPYREF_NODE(AnyPtr, AnyPtr);
        IMPLEMENT_OP2_COPYREF_NODE(Local, AnyPtr);
        IMPLEMENT_OP2_COPYREF_NODE(Local, Local);
        IMPLEMENT_OP2_COPYREF_NODE(CMResOfs, AnyPtr);
        IMPLEMENT_OP2_COPYREF_NODE(LocalRefOff, AnyPtr);
        IMPLEMENT_OP2_COPYREF_NODE(ArgumentRef, AnyPtr);
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode * node, SimNode * node_l, SimNode * node_r, Context * context) override {
            auto crnode = static_cast<SimNode_CopyRefValue *> (node);
            uint32_t size = crnode->size;
            if (false) { } 
            // *, *
            MATCH_OP2_COPYREF("GetLocal","GetLocal",Local, Local)
            // *, any
            MATCH_OP2_COPYREF_LEFT_ANY("GetLocal",Local)
            MATCH_OP2_COPYREF_LEFT_ANY("GetCMResOfs",CMResOfs)
            MATCH_OP2_COPYREF_LEFT_ANY("GetLocalRefOff",LocalRefOff)
            MATCH_OP2_COPYREF_LEFT_ANY("GetArgument",ArgumentRef)
            // fallback
            else {
                if (isFastCopyBytes(size)) {
                    anyLeft = anyRight = true;
                    context->code->makeNodeUnroll<SimNode_CopyRefValueFixed_AnyPtr_AnyPtr>(size);
                }
            }
            return nullptr;
        }
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) override {
            auto opcresult = static_cast<SimNode_Op2FusionCopyRef *>(result);
            auto crnode = static_cast<SimNode_CopyRefValue *> (node);
            opcresult->set("CopyRefValue",Type::none,node->debugInfo); 
            opcresult->size = crnode->size;
        }
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override {
            SimNode_CopyRefValue * crnode = static_cast<SimNode_CopyRefValue *>(node);
            return fuseOp2(info, node, crnode->l, crnode->r, context);
        }
    };

#define FUSION_OP2_PTR(CTYPE,expr)              (((CTYPE *)(expr)))
#define FUSION_OP2_RVALUE(CTYPE,expr)           (*((CTYPE *)(expr)))
#define FUSION_OP2_PTR_TO_LVALUE(expr)          (*(expr))

#define IMPLEMENT_OP2_SET_NODE_ANY(TYPE,CTYPE,COMPUTEL) \
    struct SimNode_CopyValue_##COMPUTEL##_Any : SimNode_Op2Fusion { \
        virtual vec4f eval ( Context & context ) override { \
            auto pl = FUSION_OP2_PTR(CTYPE,l.compute##COMPUTEL(context)); \
            auto rr = r.subexpr->eval##TYPE(context); \
            SimPolicy<CTYPE>::Set(FUSION_OP2_PTR_TO_LVALUE(pl),rr,context); \
            return v_zero(); \
        } \
    }; 

#define IMPLEMENT_OP2_SET_NODE(TYPE,CTYPE,COMPUTEL,COMPUTER) \
        struct SimNode_CopyValue_##COMPUTEL##_##COMPUTER : SimNode_Op2Fusion { \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = FUSION_OP2_PTR(CTYPE,l.compute##COMPUTEL(context)); \
                auto rr = FUSION_OP2_RVALUE(CTYPE,r.compute##COMPUTER(context)); \
                SimPolicy<CTYPE>::Set(FUSION_OP2_PTR_TO_LVALUE(pl),rr,context); \
                return v_zero(); \
            } \
        }; \

#define MATCH_OP2_SET(LNODENAME,RNODENAME,COMPUTEL,COMPUTER) \
    else if ( is(info, node_l,LNODENAME) &&  is(info,node_r,RNODENAME) ) { \
        return context->code->makeNode<SimNode_CopyValue_##COMPUTEL##_##COMPUTER>(); \
    } 

#define MATCH_OP2_SET_ANY(LNODENAME,COMPUTEL) \
    else if ( is(info, node_l,LNODENAME) ) { \
        anyRight = true; \
        return context->code->makeNode<SimNode_CopyValue_##COMPUTEL##_Any>(); \
    } 

    // copy value
    //  *a = b
#define IMPLEMENT_ANY_COPY_VALUE(TYPE,CTYPE) \
    struct FusionPoint_MiscCopyValue_##CTYPE : FusionPointOp2 { \
        FusionPoint_MiscCopyValue_##CTYPE() {} \
        IMPLEMENT_OP2_SET_NODE(TYPE,CTYPE,Local,Const); \
        IMPLEMENT_OP2_SET_NODE_ANY(TYPE,CTYPE,AnyPtr); \
        IMPLEMENT_OP2_SET_NODE_ANY(TYPE,CTYPE,Local); \
        IMPLEMENT_OP2_SET_NODE_ANY(TYPE,CTYPE,CMResOfs); \
        IMPLEMENT_OP2_SET_NODE_ANY(TYPE,CTYPE,LocalRefOff); \
        IMPLEMENT_OP2_SET_NODE_ANY(TYPE,CTYPE,ArgumentRef); \
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode *, SimNode * node_l, SimNode * node_r, Context * context) override { \
            if ( false ) {} \
            MATCH_OP2_SET("GetLocal","ConstValue",Local,Const) \
            MATCH_OP2_SET_ANY("GetLocal",Local) \
            MATCH_OP2_SET_ANY("GetCMResOfs",CMResOfs) \
            MATCH_OP2_SET_ANY("GetLocalRefOff",LocalRefOff) \
            MATCH_OP2_SET_ANY("GetArgument",ArgumentRef) \
            return nullptr; \
        } \
        virtual void set(SimNode_Op2Fusion * result, SimNode * node) override { \
            result->set("CopyValue",Type(ToBasicType<CTYPE>::type),node->debugInfo); \
        } \
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override { \
            auto cnode = static_cast<SimNode_CopyValue<CTYPE> *>(node); \
            return fuseOp2(info, node, cnode->l, cnode->r, context); \
        } \
    };

#define IMPLEMENT_VEC_COPY_VALUE(CTYPE) \
    IMPLEMENT_ANY_COPY_VALUE(,CTYPE)

#define REGISTER_COPY_VALUE(CTYPE) \
    (*g_fusionEngine)[fuseName("CopyValue",typeName<CTYPE>::name())].push_back(make_shared<FusionPoint_MiscCopyValue_##CTYPE>());

    IMPLEMENT_ANY_COPY_VALUE(Int,    int32_t);
    IMPLEMENT_ANY_COPY_VALUE(UInt,   uint32_t);
    IMPLEMENT_ANY_COPY_VALUE(Int64,  int64_t);
    IMPLEMENT_ANY_COPY_VALUE(UInt64, uint64_t);
    IMPLEMENT_ANY_COPY_VALUE(Float,  float);    
    IMPLEMENT_ANY_COPY_VALUE(Double, double);
    IMPLEMENT_ANY_COPY_VALUE(Bool,   bool);

#define FUSION_OP2_PTR(CTYPE,expr)              (((char *)(expr)))
#define FUSION_OP2_RVALUE(CTYPE,expr)           ((v_ldu((const float *)expr)))
#define FUSION_OP2_PTR_TO_LVALUE(expr)          ((expr))

    IMPLEMENT_VEC_COPY_VALUE(int2);
    IMPLEMENT_VEC_COPY_VALUE(int3);
    IMPLEMENT_VEC_COPY_VALUE(int4);
    IMPLEMENT_VEC_COPY_VALUE(uint2);
    IMPLEMENT_VEC_COPY_VALUE(uint3);
    IMPLEMENT_VEC_COPY_VALUE(uint4);
    IMPLEMENT_VEC_COPY_VALUE(float2);
    IMPLEMENT_VEC_COPY_VALUE(float3);
    IMPLEMENT_VEC_COPY_VALUE(float4);
    IMPLEMENT_VEC_COPY_VALUE(range);
    IMPLEMENT_VEC_COPY_VALUE(urange);

    void createFusionEngine_misc_copy_reference() {
        (*g_fusionEngine)["CopyReference"].push_back(make_shared<FusionPoint_MiscCopyReference>());
        (*g_fusionEngine)["CopyRefValue"].push_back(make_shared<FusionPoint_MiscCopyRefValue>());
        // scalar copy-value
        REGISTER_COPY_VALUE(int32_t);
        REGISTER_COPY_VALUE(uint32_t);
        REGISTER_COPY_VALUE(int64_t);
        REGISTER_COPY_VALUE(uint64_t);
        REGISTER_COPY_VALUE(float);
        REGISTER_COPY_VALUE(double);
        REGISTER_COPY_VALUE(bool);
        // vector copy-value
        REGISTER_COPY_VALUE(int2);
        REGISTER_COPY_VALUE(int3);
        REGISTER_COPY_VALUE(int4);
        REGISTER_COPY_VALUE(uint2);
        REGISTER_COPY_VALUE(uint3);
        REGISTER_COPY_VALUE(uint4);
        REGISTER_COPY_VALUE(float2);
        REGISTER_COPY_VALUE(float3);
        REGISTER_COPY_VALUE(float4);
        REGISTER_COPY_VALUE(range);
        REGISTER_COPY_VALUE(urange);
    }
}
