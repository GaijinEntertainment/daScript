#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "simulate_fusion.h"

#if DAS_FUSION>=2

#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    // src/simulate/ARCHITECTURE.md#pointer-index-family
    struct SimNode_PtrAtFusion : SimNode_Op2Index {
        SimNode_PtrAtFusion() : SimNode_Op2Index("") {}
    };

    struct SimNode_PtrSetFusion : SimNode_PtrAtFusion {
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            string name = op;
            name += getSimSourceName(l.type);
            if ( isIndexed ) name += getSimSourceName(r.type);
            vis.op(name.c_str(), getTypeBaseSize(baseType), das_to_string(baseType));
            l.visit(vis);
            if ( isIndexed ) {
                r.visit(vis);
                V_ARG(stride);
                V_ARG(offset);
            }
            V_SUB(value);
            V_END();
        }
        SimNode * value = nullptr;
        bool isIndexed = false;
    };

#define IMPLEMENT_PTR_BASE(BASE,COMPUTE) \
    struct PtrBase##BASE { \
        static constexpr bool fusesAnyIndex = true; \
        static __forceinline char * get ( const SimSource & base, Context & context ) { \
            return *(char **) base.compute##COMPUTE(context); \
        } \
    };

    IMPLEMENT_PTR_BASE(Loc,Local)
    IMPLEMENT_PTR_BASE(Arg,Argument)
    IMPLEMENT_PTR_BASE(Glob,Global)
    IMPLEMENT_PTR_BASE(GlobMnh,GlobalMnh)

    struct PtrBaseAny {
        static constexpr bool fusesAnyIndex = false;
        static __forceinline char * get ( const SimSource & base, Context & context ) {
            return base.computeAnyPtr(context);
        }
    };

#define IMPLEMENT_PTR_INDEX(INDEX,COMPUTE) \
    template <typename IDXT> \
    struct PtrIndex##INDEX { \
        static __forceinline IDXT get ( const SimSource & index, Context & context ) { \
            return *(IDXT *) index.compute##COMPUTE(context); \
        } \
    };

    IMPLEMENT_PTR_INDEX(Loc,Local)
    IMPLEMENT_PTR_INDEX(Arg,Argument)
    IMPLEMENT_PTR_INDEX(Const,Const)

    template <typename IDXT>
    struct PtrIndexAny {
        static __forceinline IDXT get ( const SimSource & index, Context & context ) {
            return evalNode<IDXT>::eval(context, index.subexpr);
        }
    };

    template <typename NODE, typename BASE, typename INDEX>
    struct SimNode_PtrAtAddr : NODE {
        __forceinline char * address ( Context & context ) {
            char * pValue = BASE::get(this->l, context);
            auto idx = INDEX::get(this->r, context);
            typedef decltype(idx) IDXT;
            return pValue + (idx*IDXT(this->stride) + IDXT(this->offset));
        }
    };

    template <typename NODE, typename BASE>
    struct SimNode_PtrDerefAddr : NODE {
        __forceinline char * address ( Context & context ) {
            char * ptr = BASE::get(this->l, context);
            if ( !ptr ) {
                context.throw_error_at(this->debugInfo,"dereferencing null pointer%s", this->errorMessage);
            }
            return ptr;
        }
    };

#define PTR_AT_WORKHORSES(STAMP) \
    STAMP(Bool,bool) \
    STAMP(Int,int32_t) \
    STAMP(UInt,uint32_t) \
    STAMP(Int64,int64_t) \
    STAMP(UInt64,uint64_t) \
    STAMP(Float,float) \
    STAMP(Double,double)

#define PTR_AT_VECTORS(STAMP) \
    STAMP(int2) \
    STAMP(uint2) \
    STAMP(int3) \
    STAMP(uint3) \
    STAMP(int4) \
    STAMP(uint4) \
    STAMP(float2) \
    STAMP(float3) \
    STAMP(float4)

    struct PtrAtRef {};

    template <typename ADDR, typename CTYPE>
    struct SimNode_PtrAtLoad;

#define IMPLEMENT_PTR_AT_LOAD(CTYPE,TAIL) \
    template <typename ADDR> \
    struct SimNode_PtrAtLoad<ADDR,CTYPE> : ADDR { \
        __forceinline CTYPE compute ( Context & context ) { \
            DAS_PROFILE_NODE \
            return *((CTYPE *) this->address(context)); \
        } \
        TAIL; \
    };

#define IMPLEMENT_PTR_AT_LOAD_WORKHORSE(TYPE,CTYPE) \
    IMPLEMENT_PTR_AT_LOAD(CTYPE,DAS_NODE(TYPE,CTYPE))

#define IMPLEMENT_PTR_AT_LOAD_SMALL_INT(UNUSED,CTYPE) \
    IMPLEMENT_PTR_AT_LOAD(CTYPE,DAS_FUSION_WIDE_NODE)

#define IMPLEMENT_PTR_AT_LOAD_VECTOR(CTYPE) \
    template <typename ADDR> \
    struct SimNode_PtrAtLoad<ADDR,CTYPE> : ADDR { \
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override { \
            DAS_PROFILE_NODE \
            vec4f __r; \
            DAS_LDU_WORKHORSE(__r, this->address(context), CTYPE); \
            return __r; \
        } \
    };

    PTR_AT_WORKHORSES(IMPLEMENT_PTR_AT_LOAD_WORKHORSE)
    DAS_FUSION_SMALL_INTS(IMPLEMENT_PTR_AT_LOAD_SMALL_INT,)
    PTR_AT_VECTORS(IMPLEMENT_PTR_AT_LOAD_VECTOR)

    template <typename ADDR>
    struct SimNode_PtrAtLoad<ADDR,PtrAtRef> : ADDR {
        __forceinline char * compute ( Context & context ) {
            DAS_PROFILE_NODE
            return this->address(context);
        }
        DAS_PTR_NODE;
    };

    template <typename ADDR, typename CTYPE>
    struct SimNode_PtrSet : ADDR {
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override {
            DAS_PROFILE_NODE
            auto R = EvalTT<CTYPE>::eval(context, this->value);
            CTYPE * pl = (CTYPE *) this->address(context);
            *pl = R;
            return v_zero();
        }
    };

    template <typename CTYPE>
    struct PtrAtLoadOf {
        template <typename ADDR> using node = SimNode_PtrAtLoad<ADDR,CTYPE>;
    };

    template <typename CTYPE>
    struct PtrSetOf {
        template <typename ADDR> using node = SimNode_PtrSet<ADDR,CTYPE>;
    };

    template <typename IDXT> struct PtrAtOps;

    template <> struct PtrAtOps<int32_t> {
        static constexpr const char * at = "PtrAt";
        static constexpr const char * set = "PtrAtSet";
    };

    template <> struct PtrAtOps<uint32_t> {
        static constexpr const char * at = "PtrAt_U32";
        static constexpr const char * set = "PtrAtSet_U32";
    };

    template <typename IDXT, typename CTYPE>
    struct PtrAtLoadType {
        static const char * op() { return PtrAtR2VName<IDXT>::name(); }
        static Type type() { return Type(ToBasicType<CTYPE>::type); }
    };

    template <typename IDXT>
    struct PtrAtLoadType<IDXT,PtrAtRef> {
        static const char * op() { return PtrAtOps<IDXT>::at; }
        static Type type() { return Type::none; }
    };

    static SimSource ptrBaseSource ( const SimNodeInfoLookup & info, SimNode * node ) {
        SimSource source;
        if ( FusionPoint::is(info,node,"GetLocalR2V") || FusionPoint::is(info,node,"GetArgument")
            || FusionPoint::is(info,node,"GetGlobalR2V") || FusionPoint::is(info,node,"GetGlobalMnhR2V") ) {
            source = static_cast<SimNode_SourceBase *>(node)->subexpr;
        } else {
            source.setSimNode(node);
        }
        return source;
    }

    static SimSourceType ptrIndexSourceType ( const SimNodeInfoLookup & info, SimNode * node ) {
        if ( FusionPoint::is(info,node,"GetLocalR2V") || FusionPoint::is(info,node,"GetArgument")
            || FusionPoint::is(info,node,"ConstValue") ) {
            return static_cast<SimNode_SourceBase *>(node)->subexpr.type;
        }
        return SimSourceType::sSimNode;
    }

    template <typename WRAP, typename NODE, typename BASE, typename INDEX>
    NODE * makePtrAtNode ( Context * context ) {
        return context->code->makeNode<typename WRAP::template node<SimNode_PtrAtAddr<NODE,BASE,INDEX>>>();
    }

    template <typename WRAP, typename NODE, typename IDXT, typename BASE>
    NODE * makePtrAtForIndex ( Context * context, SimSourceType index ) {
        switch ( index ) {
        case SimSourceType::sSimNode:
            if constexpr ( BASE::fusesAnyIndex ) {
                return makePtrAtNode<WRAP,NODE,BASE,PtrIndexAny<IDXT>>(context);
            } else {
                return nullptr;
            }
        case SimSourceType::sLocal:         return makePtrAtNode<WRAP,NODE,BASE,PtrIndexLoc<IDXT>>(context);
        case SimSourceType::sArgument:      return makePtrAtNode<WRAP,NODE,BASE,PtrIndexArg<IDXT>>(context);
        case SimSourceType::sConstValue:    return makePtrAtNode<WRAP,NODE,BASE,PtrIndexConst<IDXT>>(context);
        default:                            return nullptr;
        }
    }

    template <typename WRAP, typename NODE, typename IDXT>
    NODE * makePtrAtForBase ( Context * context, SimSourceType base, SimSourceType index ) {
        switch ( base ) {
        case SimSourceType::sLocal:         return makePtrAtForIndex<WRAP,NODE,IDXT,PtrBaseLoc>(context, index);
        case SimSourceType::sArgument:      return makePtrAtForIndex<WRAP,NODE,IDXT,PtrBaseArg>(context, index);
        case SimSourceType::sGlobal:        return makePtrAtForIndex<WRAP,NODE,IDXT,PtrBaseGlob>(context, index);
        case SimSourceType::sGlobalMnh:     return makePtrAtForIndex<WRAP,NODE,IDXT,PtrBaseGlobMnh>(context, index);
        case SimSourceType::sSimNode:       return makePtrAtForIndex<WRAP,NODE,IDXT,PtrBaseAny>(context, index);
        default:                            return nullptr;
        }
    }

    template <typename WRAP, typename NODE>
    NODE * makePtrDerefNode ( Context * context, SimSourceType base ) {
        auto & code = *(context->code);
        switch ( base ) {
        case SimSourceType::sLocal:
            return code.makeNode<typename WRAP::template node<SimNode_PtrDerefAddr<NODE,PtrBaseLoc>>>();
        case SimSourceType::sArgument:
            return code.makeNode<typename WRAP::template node<SimNode_PtrDerefAddr<NODE,PtrBaseArg>>>();
        default:
            return nullptr;
        }
    }

    template <typename IDXT, typename CTYPE>
    struct FusionPoint_PtrAt final : FusionPointOp2 {
        virtual SimNode * match ( const SimNodeInfoLookup & info, SimNode *, SimNode * node_l, SimNode * node_r, Context * context, bool & anyLeft, bool & anyRight ) const override {
            SimSourceType base = ptrBaseSource(info, node_l).type;
            SimSourceType index = ptrIndexSourceType(info, node_r);
            anyLeft = base==SimSourceType::sSimNode;
            anyRight = index==SimSourceType::sSimNode;
            return makePtrAtForBase<PtrAtLoadOf<CTYPE>,SimNode_PtrAtFusion,IDXT>(context, base, index);
        }
        virtual void set ( SimNode_Op2Fusion * result, SimNode * node ) const override {
            auto at = static_cast<SimNode_PtrAt<IDXT> *>(node);
            auto fused = static_cast<SimNode_PtrAtFusion *>(result);
            fused->set(PtrAtLoadType<IDXT,CTYPE>::op(), PtrAtLoadType<IDXT,CTYPE>::type(), node->debugInfo);
            fused->stride = at->stride;
            fused->offset = at->offset;
        }
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) const override {
            auto at = static_cast<SimNode_PtrAt<IDXT> *>(node);
            return fuseOp2(info, node, at->value, at->index, context);
        }
    };

    template <typename CTYPE> struct IsSmallInt { enum { value = false }; };

#define IMPLEMENT_IS_SMALL_INT(UNUSED,CTYPE) \
    template <> struct IsSmallInt<CTYPE> { enum { value = true }; };

    DAS_FUSION_SMALL_INTS(IMPLEMENT_IS_SMALL_INT,)

    template <typename CTYPE>
    struct FusionPoint_PtrSet final : FusionPoint {
        template <typename IDXT>
        static SimNode_PtrSetFusion * indexed ( SimNode * node, SimNode * left, Context * context ) {
            auto at = static_cast<SimNode_PtrAtFusion *>(left);
            auto result = makePtrAtForBase<PtrSetOf<CTYPE>,SimNode_PtrSetFusion,IDXT>(context, at->l.type, at->r.type);
            if ( !result ) return nullptr;
            result->set(PtrAtOps<IDXT>::set, Type(ToBasicType<CTYPE>::type), node->debugInfo);
            result->l = at->l;
            result->r = at->r;
            result->stride = at->stride;
            result->offset = at->offset;
            result->isIndexed = true;
            return result;
        }
        static SimNode_PtrSetFusion * dereferenced ( const SimNodeInfoLookup & info, SimNode * left, Context * context ) {
            auto ref = static_cast<SimNode_Ptr2Ref *>(left);
            SimSource base = ptrBaseSource(info, ref->subexpr);
            auto result = makePtrDerefNode<PtrSetOf<CTYPE>,SimNode_PtrSetFusion>(context, base.type);
            if ( !result ) return nullptr;
            result->set("PtrSet", Type(ToBasicType<CTYPE>::type), ref->debugInfo);
            result->l = base;
            result->errorMessage = ref->errorMessage;
            return result;
        }
        static SimNode * storedValue ( const SimNodeInfoLookup & info, SimNode * value ) {
            if ( IsSmallInt<CTYPE>::value ) {
                string narrow = string("Cast_to_") + typeName<CTYPE>::name();
                if ( is(info,value,narrow.c_str(),"int") || is(info,value,narrow.c_str(),"uint") ) {
                    return static_cast<SimNode_CallBase *>(value)->arguments[0];
                }
            }
            return value;
        }
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) const override {
            auto set = static_cast<SimNode_Set<CTYPE> *>(node);
            SimNode_PtrSetFusion * result = nullptr;
            if ( is(info,set->l,"Ptr2Ref") ) {
                result = dereferenced(info, set->l, context);
            } else if ( isFusedOp2(info,set->l,PtrAtOps<int32_t>::at,"") ) {
                result = indexed<int32_t>(node, set->l, context);
            } else if ( isFusedOp2(info,set->l,PtrAtOps<uint32_t>::at,"") ) {
                result = indexed<uint32_t>(node, set->l, context);
            }
            if ( !result ) return node;
            result->value = storedValue(info, set->r);
            return result;
        }
    };

#define REGISTER_PTR_AT(CTYPE) \
    { PtrAtR2VName<int32_t>::name(), typeName<CTYPE>::name(), &fusionPoint<FusionPoint_PtrAt<int32_t,CTYPE>> }, \
    { PtrAtR2VName<uint32_t>::name(), typeName<CTYPE>::name(), &fusionPoint<FusionPoint_PtrAt<uint32_t,CTYPE>> },

#define REGISTER_PTR_AT_TYPED(UNUSED,CTYPE) \
    REGISTER_PTR_AT(CTYPE)

#define REGISTER_PTR_SET(UNUSED,CTYPE) \
    { "Set", typeName<CTYPE>::name(), &fusionPoint<FusionPoint_PtrSet<CTYPE>> },

    static constexpr FusionEntry fusionEntries_ptr_at[] = {
        PTR_AT_WORKHORSES(REGISTER_PTR_AT_TYPED)
        DAS_FUSION_SMALL_INTS(REGISTER_PTR_AT_TYPED,)
        PTR_AT_VECTORS(REGISTER_PTR_AT)
        { PtrAtOps<int32_t>::at, typeName<int32_t>::name(), &fusionPoint<FusionPoint_PtrAt<int32_t,PtrAtRef>> },
        { PtrAtOps<int32_t>::at, typeName<uint32_t>::name(), &fusionPoint<FusionPoint_PtrAt<uint32_t,PtrAtRef>> },
        PTR_AT_WORKHORSES(REGISTER_PTR_SET)
        DAS_FUSION_SMALL_INTS(REGISTER_PTR_SET,)
    };

    FUSION_TABLE(fusionTable_ptr_at, fusionEntries_ptr_at)
}

#endif
