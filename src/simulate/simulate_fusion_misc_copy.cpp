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
                return context->code->makeNode<SimNode_CopyReferenceLocAny>(node->debugInfo, lnode_l->stackTop, crnode->r); 
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
                V_OP(CopyRefValue);
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
        virtual SimNode * fuse(const SimNodeInfoLookup & info, SimNode * node, Context * context) override {
            auto crnode = static_cast<SimNode_CopyRefValue *> (node);
            /* CopyRefValue(GetLocal,*,size) */
            if ( is(info, crnode->l, "GetLocal" )) {
                auto lnode_l = static_cast<SimNode_GetLocal *>(crnode->l);
                if (isFastCopyBytes(crnode->size)) {
                    return context->code->makeNodeUnroll<SimNode_CopyRefValueFixedLocAny>(crnode->size, node->debugInfo, lnode_l->stackTop, crnode->r);
                } else {
                    return context->code->makeNode<SimNode_CopyRefValueLocAny>(node->debugInfo, lnode_l->stackTop, crnode->r, crnode->size);
                }
            } else {
                if (isFastCopyBytes(crnode->size)) {
                    context->code->makeNodeUnroll<SimNode_CopyRefValueFixed>(crnode->size, node->debugInfo, crnode->l, crnode->r);
                }
            }
            return node;
        }
    };


    void createFusionEngine_misc_copy_reference() {
        (*g_fusionEngine)["CopyReference"].push_back(make_shared<FusionPoint_MiscCopyReference>());
        (*g_fusionEngine)["CopyRefValue"].push_back(make_shared<FusionPoint_MiscCopyRefValue>());
    }
}