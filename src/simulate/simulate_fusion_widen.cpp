#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "simulate_fusion.h"

#if DAS_FUSION

#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    struct SimNode_WideRead : SimNode_Op1Fusion {
        SimNode_WideRead() : SimNode_Op1Fusion("") {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            vis.op(op, getTypeBaseSize(baseType), das_to_string(baseType));
            subexpr.visit(vis);
            V_END();
        }
    };

#define IMPLEMENT_WIDE_READ_SOURCE(SOURCE) \
    struct WideRead##SOURCE { \
        static constexpr const char * source = "Get" #SOURCE "R2V"; \
        static constexpr const char * op = "Get" #SOURCE "R2VToInt"; \
        static __forceinline char * get ( const SimSource & subexpr, Context & context ) { \
            return subexpr.compute##SOURCE(context); \
        } \
    };

    IMPLEMENT_WIDE_READ_SOURCE(Local)
    IMPLEMENT_WIDE_READ_SOURCE(LocalRefOff)
    IMPLEMENT_WIDE_READ_SOURCE(ArgumentRefOff)

    template <typename SOURCE, typename CTYPE>
    struct SimNode_WideReadT : SimNode_WideRead {
        __forceinline CTYPE compute ( Context & context ) {
            DAS_PROFILE_NODE
            return *((CTYPE *) SOURCE::get(subexpr, context));
        }
        DAS_FUSION_WIDE_NODE;
    };

    // src/simulate/ARCHITECTURE.md#fused-small-int-reads
    static const char * const g_fusedSmallIntReads[] = {
        "AtR2V", "AtR2VU", "ArrayAtR2V", "ArrayAtR2VU",
        PtrAtR2VName<int32_t>::name(), PtrAtR2VName<uint32_t>::name()
    };

    template <typename CTYPE>
    struct FusionPoint_Widen final : FusionPointOp1 {
        template <typename SOURCE>
        static SimNode * wideRead ( Context * context ) {
            auto result = context->code->makeNode<SimNode_WideReadT<SOURCE,CTYPE>>();
            result->op = SOURCE::op;
            return result;
        }
        virtual SimNode * match ( const SimNodeInfoLookup & info, SimNode *, SimNode * arg, Context * context ) const override {
            const char * from = typeName<CTYPE>::name();
            if ( is(info,arg,WideReadLocal::source,from) ) {
                return wideRead<WideReadLocal>(context);
            } else if ( is(info,arg,WideReadLocalRefOff::source,from) ) {
                return wideRead<WideReadLocalRefOff>(context);
            } else if ( is(info,arg,WideReadArgumentRefOff::source,from) ) {
                return wideRead<WideReadArgumentRefOff>(context);
            }
            return nullptr;
        }
        virtual void set ( SimNode_Op1Fusion * result, SimNode * node ) const override {
            result->set(result->op, Type(ToBasicType<CTYPE>::type), node->debugInfo);
        }
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) const override {
            auto arg = static_cast<SimNode_CallBase *>(node)->arguments[0];
            for ( auto op : g_fusedSmallIntReads ) {
                if ( isFusedOp2(info,arg,op,typeName<CTYPE>::name()) ) return arg;
            }
            return fuseOp1(info, node, arg, context);
        }
    };

#define REGISTER_WIDEN(CAST,CTYPE) \
    FUSION_ENTRY(CAST,typeName<CTYPE>::name(),FusionPoint_Widen<CTYPE>)

    static constexpr FusionEntry fusionEntries_widen[] = {
        DAS_FUSION_SMALL_INTS(REGISTER_WIDEN,"Cast_to_int")
        DAS_FUSION_SMALL_INTS(REGISTER_WIDEN,"Cast_to_uint")
    };

    FUSION_TABLE(fusionTable_widen, fusionEntries_widen)
}

#endif
