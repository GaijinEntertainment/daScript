#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/misc/copy_bytes.h"
#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"

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

    // copy reference value
    //  *a = *b
    struct FusionPoint_MiscCopyRefValue : FusionPointOp2 {
        FusionPoint_MiscCopyRefValue() {}
        template <int typeSize>
        struct SimNode_CopyRefValueFixed : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeAnyPtr(context);
                auto pr = r.computeAnyPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedLocAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeLocal(context);
                auto pr = r.computeAnyPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        struct SimNode_CopyRefValueLocAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeLocal(context);
                auto pr = r.computeAnyPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
            uint32_t stackTop_l;
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedLocLoc : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeLocal(context);
                auto pr = r.computeLocal(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        struct SimNode_CopyRefValueLocLoc : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeLocal(context);
                auto pr = r.computeLocal(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedCmroAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeCMResOfs(context);
                auto pr = r.computeAnyPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        struct SimNode_CopyRefValueCmroAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeCMResOfs(context);
                auto pr = r.computeAnyPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedGlrfAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeLocalRefOff(context);
                auto pr = r.computeAnyPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        struct SimNode_CopyRefValueGlrfAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeLocalRefOff(context);
                auto pr = r.computeAnyPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedArgAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeArgumentRef(context);
                auto pr = r.computeAnyPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        struct SimNode_CopyRefValueArgAny : SimNode_Op2FusionCopyRef {
            virtual vec4f eval ( Context & context ) override {
                auto pl = l.computeArgumentRef(context);
                auto pr = r.computeAnyPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
        };
        virtual SimNode * match(const SimNodeInfoLookup & info, SimNode * node, SimNode * node_l, SimNode * node_r, Context * context) override {
            auto crnode = static_cast<SimNode_CopyRefValue *> (node);
            uint32_t size = crnode->size;
            if (false) {
            } else if (is(info, node_l, "GetLocal")) {
                if (is(info, node_r, "GetLocal")) {
                    if (isFastCopyBytes(size)) {
                        return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedLocLoc>(size);
                    } else {
                        return context->code->makeNode<SimNode_CopyRefValueLocLoc>();
                    }
                } else {
                    anyRight = true;
                    if (isFastCopyBytes(size)) {
                        return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedLocAny>(size);
                    } else {
                        return context->code->makeNode<SimNode_CopyRefValueLocAny>();
                    }
                }
            } else if (is(info, node_l, "GetCMResOfs")) {
                anyRight = true;
                if (isFastCopyBytes(size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedCmroAny>(size);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueCmroAny>();
                }
            } else if (is(info, node_l, "GetLocalRefOff")) {
                anyRight = true;
                if (isFastCopyBytes(size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedGlrfAny>(size);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueGlrfAny>();
                }
            } else if (is(info, node_l, "GetArgument")) {
                anyRight = true;
                if (isFastCopyBytes(size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedArgAny>(size);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueArgAny>();
                }
            } else {
                if (isFastCopyBytes(size)) {
                    anyLeft = anyRight = true;
                    context->code->makeNodeUnroll<SimNode_CopyRefValueFixed>(size);
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

#define CAST_COPY_RESULT(CTYPE,expr)        ((expr))

    // copy value
    //  *a = b
#define IMPLEMENT_ANY_COPY_VALUE(TYPE,CTYPE,CONSTTYPE) \
    struct FusionPoint_MiscCopyValue_##CTYPE : FusionPoint { \
        FusionPoint_MiscCopyValue_##CTYPE() {} \
        struct SimNode_CopyValueAnyAny : SimNode { \
            SimNode_CopyValueAnyAny(const LineInfo & at, SimNode * ll, SimNode * rr) \
                : SimNode(at), l(ll), r(rr) {}; \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op("CopyValueAnyAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SUB(l); \
                V_SUB(r); \
                V_END(); \
            } \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = (CTYPE *) l->evalPtr(context); \
                auto rr = r->eval##TYPE(context); \
                *pl = CAST_COPY_RESULT(CTYPE,rr); \
                return v_zero(); \
            } \
            SimNode * l, * r; \
        }; \
        struct SimNode_CopyValueLocAny : SimNode { \
            SimNode_CopyValueLocAny(const LineInfo & at, uint32_t lsp, SimNode * rr) \
                : SimNode(at), stackTop_l(lsp), r(rr) {}; \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op("CopyValueLocAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_SUB(r); \
                V_END(); \
            } \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = (CTYPE *)(context.stack.sp() + stackTop_l); \
                auto rr = r->eval##TYPE(context); \
                *pl = CAST_COPY_RESULT(CTYPE,rr); \
                return v_zero(); \
            } \
            uint32_t stackTop_l; \
            SimNode * r; \
        }; \
        struct SimNode_CopyValueLocConst : SimNode { \
            SimNode_CopyValueLocConst(const LineInfo & at, uint32_t lsp, CONSTTYPE crv) \
                : SimNode(at), stackTop_l(lsp), c_r(crv) {}; \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op("CopyValueLocConst", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_ARG(c_r); \
                V_END(); \
            } \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = (CTYPE *)(context.stack.sp() + stackTop_l); \
                *pl = CAST_COPY_RESULT(CTYPE,c_r); \
                return v_zero(); \
            } \
            uint32_t stackTop_l; \
            CONSTTYPE c_r; \
        }; \
        struct SimNode_CopyValueCmroAny : SimNode { \
            SimNode_CopyValueCmroAny(const LineInfo & at, uint32_t ol, SimNode * rr) \
                : SimNode(at), offset_l(ol), r(rr) {}; \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op("CopyValueCmroAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(offset_l); \
                V_SUB(r); \
                V_END(); \
            } \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = (CTYPE *)(context.abiCopyOrMoveResult() + offset_l); \
                auto rr = r->eval##TYPE(context); \
                *pl = CAST_COPY_RESULT(CTYPE,rr); \
                return v_zero(); \
            } \
            uint32_t offset_l; \
            SimNode * r; \
        }; \
        struct SimNode_CopyValueGlrfAny : SimNode { \
            SimNode_CopyValueGlrfAny(const LineInfo & at, uint32_t spl, uint32_t ol, SimNode * rr) \
                : SimNode(at), stackTop_l(spl), offset_l(ol), r(rr) {}; \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op("CopyValueGlrfAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_SP(stackTop_l); \
                V_ARG(offset_l); \
                V_SUB(r); \
                V_END(); \
            } \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = (CTYPE *)((*(char **)(context.stack.sp() + stackTop_l)) + offset_l); \
                auto rr = r->eval##TYPE(context); \
                *pl = CAST_COPY_RESULT(CTYPE,rr); \
                return v_zero(); \
            } \
            uint32_t stackTop_l; \
            uint32_t offset_l; \
            SimNode * r; \
        }; \
        struct SimNode_CopyValueArgAny : SimNode { \
            SimNode_CopyValueArgAny(const LineInfo & at, int32_t ill, SimNode * rr) \
                : SimNode(at), index_l(ill), r(rr) {}; \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op("CopyValueArgAny", sizeof(CTYPE), typeName<CTYPE>::name()); \
                V_ARG(index_l); \
                V_SUB(r); \
                V_END(); \
            } \
            virtual vec4f eval ( Context & context ) override { \
                auto pl = *(CTYPE **)(context.abiArguments()+index_l); \
                auto rr = r->eval##TYPE(context); \
                *pl = CAST_COPY_RESULT(CTYPE,rr); \
                return v_zero(); \
            } \
            int32_t index_l; \
            SimNode * r; \
        }; \
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override { \
            auto cnode = static_cast<SimNode_CopyValue<CTYPE> *> (node); \
            /* CopyValue(GetLocal,*,size) */ \
            if ( is(info, cnode->l, "GetLocal" )) { \
                auto lnode_l = static_cast<SimNode_GetLocal *>(cnode->l); \
                /* CopyValue(GetLocal,ConstValue) */ \
                if ( is(info,cnode->r,"ConstValue") ) { \
                    auto cnnode = static_cast<SimNode_ConstValue *>(cnode->r); \
                    auto cvalue = cast<CONSTTYPE>::to(cnnode->value); \
                    return context->code->makeNode<SimNode_CopyValueLocConst>(node->debugInfo, lnode_l->subexpr.stackTop, cvalue); \
                } else { \
                    return context->code->makeNode<SimNode_CopyValueLocAny>(node->debugInfo, lnode_l->subexpr.stackTop, cnode->r); \
                } \
            /* CopyValue(GetCMResOfs,*,size) */ \
            } else if ( is(info, cnode->l, "GetCMResOfs" )) { \
                auto cmnode_l = static_cast<SimNode_GetCMResOfs *>(cnode->l); \
                return context->code->makeNode<SimNode_CopyValueCmroAny>(node->debugInfo, cmnode_l->subexpr.offset, cnode->r); \
            /* CopyValue(GetLocalRefOff,*,size) */ \
            } else if ( is(info, cnode->l, "GetLocalRefOff" )) { \
                auto glrfnode_l = static_cast<SimNode_GetLocalRefOff *>(cnode->l); \
                return context->code->makeNode<SimNode_CopyValueGlrfAny>(node->debugInfo, glrfnode_l->subexpr.stackTop, glrfnode_l->subexpr.offset, cnode->r); \
            /* CopyValue(GetArgument,*,size) */ \
            } else if ( is(info, cnode->l, "GetArgument" )) { \
                auto argnode_l = static_cast<SimNode_GetArgument *>(cnode->l); \
                return context->code->makeNode<SimNode_CopyValueArgAny>(node->debugInfo, argnode_l->subexpr.index, cnode->r); \
            } \
            /* promote to CopyValueAnyAny, for it is faster */ \
            return context->code->makeNode<SimNode_CopyValueAnyAny>(node->debugInfo, cnode->l, cnode->r); \
        } \
    };

// (CopyValueAnyAny_TT<float3> (GetLocalRefOff #208 #16)

#define REGISTER_COPY_VALUE(CTYPE) \
    (*g_fusionEngine)[fuseName("CopyValue",typeName<CTYPE>::name())].push_back(make_shared<FusionPoint_MiscCopyValue_##CTYPE>());

#define IMPLEMENT_COPY_VALUE(TYPE,CTYPE) \
    IMPLEMENT_ANY_COPY_VALUE(TYPE,CTYPE,CTYPE)

    IMPLEMENT_COPY_VALUE(Int,    int32_t);
    IMPLEMENT_COPY_VALUE(UInt,   uint32_t);
    IMPLEMENT_COPY_VALUE(Int64,  int64_t);
    IMPLEMENT_COPY_VALUE(UInt64, uint64_t);
    IMPLEMENT_COPY_VALUE(Float,  float);
    IMPLEMENT_COPY_VALUE(Double, double);
    IMPLEMENT_COPY_VALUE(Bool,   bool);

#undef  CAST_COPY_RESULT
#define CAST_COPY_RESULT(CTYPE,expr)        (*((CTYPE *)(&(expr))))

#define IMPLEMENT_VEC_COPY_VALUE(CTYPE)     IMPLEMENT_ANY_COPY_VALUE(,CTYPE,vec4f)

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
