#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/misc/copy_bytes.h"
#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    // copy reference
    //  &a = b
    struct FusionPoint_MiscCopyReference : FusionPoint {
        FusionPoint_MiscCopyReference() {}
        struct SimNode_CopyReferenceLocAny : SimNode {
            SimNode_CopyReferenceLocAny(const LineInfo & at, uint32_t spl, SimNode * rr)
                : SimNode(at), stackTop_l(spl), r(rr) {};
            virtual SimNode * visit(SimVisitor & vis) override {
                V_BEGIN();
                V_OP(CopyReferenceLocAny);
                V_SP(stackTop_l);
                V_SUB(r);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                char  ** pl = (char **) (context.stack.sp() + stackTop_l);
                char * pr = r->evalPtr(context);
                *pl = pr;
                return v_zero();
            }
            uint32_t stackTop_l;
            SimNode * r;
        };
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override {
            auto crnode = static_cast<SimNode_CopyReference *>(node);
            /* CopyReference(GetLocal,*) */
            if ( is(info, crnode->l, "GetLocal" )) {
                auto lnode_l = static_cast<SimNode_GetLocal *>(crnode->l);
                return context->code->makeNode<SimNode_CopyReferenceLocAny>(node->debugInfo, lnode_l->subexpr.stackTop, crnode->r); 
            }
            return node;
        }
    };

    // copy reference value
    //  *a = *b
    struct FusionPoint_MiscCopyRefValue : FusionPoint {
        FusionPoint_MiscCopyRefValue() {}
        template <int typeSize>
        struct SimNode_CopyRefValueFixed : SimNode_CopyRefValue {
            SimNode_CopyRefValueFixed(const LineInfo & at, SimNode * ll, SimNode * rr)
                : SimNode_CopyRefValue(at, ll, rr, typeSize) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueFixed);
                V_SUB(l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = l->evalPtr(context);
                auto pr = r->evalPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedLocAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueFixedLocAny(const LineInfo & at, uint32_t lsp, SimNode * rr)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), stackTop_l(lsp) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueFixedLocAny);
                V_SP(stackTop_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = context.stack.sp() + stackTop_l;
                auto pr = r->evalPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
            uint32_t stackTop_l;
        };
        struct SimNode_CopyRefValueLocAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueLocAny(const LineInfo & at, uint32_t lsp, SimNode * rr, uint32_t typeSize)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), stackTop_l(lsp) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueLocAny);
                V_SP(stackTop_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = context.stack.sp() + stackTop_l;
                auto pr = r->evalPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
            uint32_t stackTop_l;
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedLocLoc : SimNode_CopyRefValue {
            SimNode_CopyRefValueFixedLocLoc(const LineInfo & at, uint32_t lsp, uint32_t rsp)
                : SimNode_CopyRefValue(at, nullptr, nullptr, typeSize), stackTop_l(lsp), stackTop_r(rsp) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueFixedLocLoc);
                V_SP(stackTop_l);
                V_SP(stackTop_r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = context.stack.sp() + stackTop_l;
                auto pr = context.stack.sp() + stackTop_r;
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
            uint32_t stackTop_l, stackTop_r;
        };
        struct SimNode_CopyRefValueLocLoc : SimNode_CopyRefValue {
            SimNode_CopyRefValueLocLoc(const LineInfo & at, uint32_t lsp, uint32_t rsp, uint32_t typeSize)
                : SimNode_CopyRefValue(at, nullptr, nullptr, typeSize), stackTop_l(lsp), stackTop_r(rsp) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueLocLoc);
                V_SP(stackTop_l);
                V_SP(stackTop_r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = context.stack.sp() + stackTop_l;
                auto pr = context.stack.sp() + stackTop_r;
                memcpy(pl, pr, size);
                return v_zero();
            }
            uint32_t stackTop_l, stackTop_r;
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedCmroAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueFixedCmroAny(const LineInfo & at, uint32_t ol, SimNode * rr)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), offset_l(ol) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueFixedCmroAny);
                V_ARG(offset_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = context.abiCopyOrMoveResult() + offset_l;
                auto pr = r->evalPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
            uint32_t offset_l;
        };
        struct SimNode_CopyRefValueCmroAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueCmroAny(const LineInfo & at, uint32_t ol, SimNode * rr, uint32_t typeSize)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), offset_l(ol) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueCmroAny);
                V_ARG(offset_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = context.abiCopyOrMoveResult() + offset_l;
                auto pr = r->evalPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
            uint32_t offset_l;
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedGlrfAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueFixedGlrfAny(const LineInfo & at, uint32_t spl, uint32_t ol, SimNode * rr)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), stackTop_l(spl), offset_l(ol) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueFixedGlrfAny);
                V_SP(stackTop_l);
                V_ARG(offset_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = (*(char **)(context.stack.sp() + stackTop_l)) + offset_l;
                auto pr = r->evalPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
            uint32_t stackTop_l;
            uint32_t offset_l;
        };
        struct SimNode_CopyRefValueGlrfAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueGlrfAny(const LineInfo & at, uint32_t spl, uint32_t ol, SimNode * rr, uint32_t typeSize)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), stackTop_l(spl), offset_l(ol) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueGlrfAny);
                V_SP(stackTop_l);
                V_ARG(offset_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = (*(char **)(context.stack.sp() + stackTop_l)) + offset_l;
                auto pr = r->evalPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
            uint32_t stackTop_l;
            uint32_t offset_l;
        };
        template <int typeSize>
        struct SimNode_CopyRefValueFixedArgAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueFixedArgAny(const LineInfo & at, int32_t ill, SimNode * rr)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), index_l(ill) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueFixedArgAny);
                V_ARG(index_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = *(char **)(context.abiArguments()+index_l);
                auto pr = r->evalPtr(context);
                CopyBytes<typeSize>::copy(pl, pr);
                return v_zero();
            }
            int32_t index_l;
        };
        struct SimNode_CopyRefValueArgAny : SimNode_CopyRefValue {
            SimNode_CopyRefValueArgAny(const LineInfo & at, int32_t ill, SimNode * rr, uint32_t typeSize)
                : SimNode_CopyRefValue(at, nullptr, rr, typeSize), index_l(ill) {};
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP(CopyRefValueArgAny);
                V_ARG(index_l);
                V_SUB(r);
                V_ARG(size);
                V_END();
            }
            virtual vec4f eval ( Context & context ) override {
                auto pl = *(char **)(context.abiArguments()+index_l);
                auto pr = r->evalPtr(context);
                memcpy(pl, pr, size);
                return v_zero();
            }
            int32_t index_l;
        };
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override {
            auto crnode = static_cast<SimNode_CopyRefValue *> (node);
            /* CopyRefValue(GetLocal,*,size) */
            if (is(info, crnode->l, "GetLocal")) {
                auto lnode_l = static_cast<SimNode_GetLocal *>(crnode->l);
                /* CopyRefValue(GetLocal,GetLocal,size) */
                if (is(info, crnode->r, "GetLocal")) {
                    auto lnode_r = static_cast<SimNode_GetLocal *>(crnode->r);
                    if (isFastCopyBytes(crnode->size)) {
                        return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedLocLoc>(crnode->size, node->debugInfo, lnode_l->subexpr.stackTop, lnode_r->subexpr.stackTop);
                    } else {
                        return context->code->makeNode<SimNode_CopyRefValueLocLoc>(node->debugInfo, lnode_l->subexpr.stackTop, lnode_r->subexpr.stackTop, crnode->size);
                    }
                } else {
                    if (isFastCopyBytes(crnode->size)) {
                        return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedLocAny>(crnode->size, node->debugInfo, lnode_l->subexpr.stackTop, crnode->r);
                    } else {
                        return context->code->makeNode<SimNode_CopyRefValueLocAny>(node->debugInfo, lnode_l->subexpr.stackTop, crnode->r, crnode->size);
                    }
                }
            /* CopyRefValue(GetCMResOfs,*,size) */
            } else if (is(info, crnode->l, "GetCMResOfs")) {
                auto cmnode_l = static_cast<SimNode_GetCMResOfs *>(crnode->l);
                if (isFastCopyBytes(crnode->size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedCmroAny>(crnode->size, node->debugInfo, cmnode_l->subexpr.offset, crnode->r);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueCmroAny>(node->debugInfo, cmnode_l->subexpr.offset, crnode->r, crnode->size);
                }
            /* CopyRefValue(GetLocalRefOff,*,size) */
            } else if (is(info, crnode->l, "GetLocalRefOff")) {
                auto glrfnode_l = static_cast<SimNode_GetLocalRefOff *>(crnode->l);
                if (isFastCopyBytes(crnode->size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedGlrfAny>(crnode->size, node->debugInfo, glrfnode_l->subexpr.stackTop, glrfnode_l->subexpr.offset, crnode->r);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueGlrfAny>(node->debugInfo, glrfnode_l->subexpr.stackTop, glrfnode_l->subexpr.offset, crnode->r, crnode->size);
                }
            /* CopyRefValue(GetArgument,*,size) */
            } else if (is(info, crnode->l, "GetArgument")) {
                auto argnode_l = static_cast<SimNode_GetArgument *>(crnode->l); 
                if (isFastCopyBytes(crnode->size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedArgAny>(crnode->size, node->debugInfo, argnode_l->index, crnode->r);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueArgAny>(node->debugInfo, argnode_l->index, crnode->r, crnode->size);
                }
            /* size allows fast copy */
            } else {
                if (isFastCopyBytes(crnode->size)) {
                    context->code->makeNodeUnroll<SimNode_CopyRefValueFixed>(crnode->size, node->debugInfo, crnode->l, crnode->r);
                }
            }
            return node;
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
                return context->code->makeNode<SimNode_CopyValueArgAny>(node->debugInfo, argnode_l->index, cnode->r); \
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
