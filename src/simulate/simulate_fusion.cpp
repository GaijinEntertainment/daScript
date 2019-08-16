#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/ast/ast.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    struct SimNodeInfo {
        string  name;
        string  typeName;
        size_t  typeSize;
    };

    string fuseName ( const string & name, const string & typeName ) {
        return typeName.empty() ? name : (name + "<" + typeName + ">");
    }

    struct SimNodeCollector : SimVisitor {
        virtual void preVisit ( SimNode * node ) override {
            SimVisitor::preVisit(node);
            thisNode = node;
        }
        virtual void op ( const char * name, size_t typeSize, const string & typeName ) override {
            SimNodeInfo ni;
            ni.name = name;
            ni.typeName = typeName;
            ni.typeSize = typeSize;
            info[thisNode] = ni;

        }
        map<SimNode *,SimNodeInfo>  info;
        SimNode * thisNode = nullptr;
    };

    struct FusionPoint {
        FusionPoint ( map<SimNode *,SimNodeInfo> & ni ) : info(ni) {
        }
        virtual ~FusionPoint() {}
        virtual SimNode * fuse ( SimNode * node, Context * ) {
            return node;
        }
        bool is ( SimNode * node, const char * name ) {
            auto & ni = info[node];
            return ni.name == name;
        }

        bool is ( SimNode * node, const char * name, const char * typeName ) {
            auto & ni = info[node];
            return (ni.name == name) && (ni.typeName==typeName);
        }
        map<SimNode *,SimNodeInfo> & info;
    };
    typedef shared_ptr<FusionPoint> FusionPointPtr;

#define IMPLEMENT_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    struct Op2FusionPoint_##OPNAME##_##TYPE : FusionPoint { \
        Op2FusionPoint_##OPNAME##_##TYPE ( map<SimNode *,SimNodeInfo> & ni ) : FusionPoint(ni) {} \
        struct SimNode_Op2R2VR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2R2VR2V ( const LineInfo & at, uint32_t sp_l, uint32_t sp_r ) \
                : SimNode(at),stackTop_l(sp_l), stackTop_r(sp_r) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VR2V", sizeof(CTYPE), #TYPE); \
                V_SP(stackTop_l); \
                V_SP(stackTop_r); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop_l); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop_r); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            uint32_t stackTop_l; \
            uint32_t stackTop_r; \
        }; \
        struct SimNode_Op2R2VAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2R2VAny ( const LineInfo & at, SimNode * rn, uint32_t sp ) \
                : SimNode(at), r(rn), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VAny", sizeof(CTYPE), #TYPE); \
                V_SUB(r); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                CTYPE rv =  r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * r; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2AnyR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2AnyR2V ( const LineInfo & at, SimNode * ln, uint32_t sp ) \
                : SimNode(at), l(ln), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyR2V", sizeof(CTYPE), #TYPE); \
                V_SUB(l); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv =  l->eval##TYPE(context); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            SimNode * l; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ArgR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2ArgR2V ( const LineInfo & at, int32_t i, uint32_t sp ) \
                : SimNode(at), index(i), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgR2V", sizeof(CTYPE), #TYPE); \
                V_ARG(index); \
                V_SP(stackTop); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv =  cast<CTYPE>::to(context.abiArguments()[index]); \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,rv,context); \
            } \
            int32_t  index; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2R2VConst : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2R2VConst ( const LineInfo & at, CTYPE cv, uint32_t sp ) \
                : SimNode(at), c(cv), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "R2VConst", sizeof(CTYPE), #TYPE); \
                V_SP(stackTop); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            CTYPE c; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ArgConst : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2ArgConst ( const LineInfo & at, CTYPE cv, int32_t i ) \
                : SimNode(at), c(cv), index(i) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ArgConst", sizeof(CTYPE), #TYPE); \
                V_ARG(index); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv =  cast<CTYPE>::to(context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            CTYPE c; \
            int32_t index; \
        }; \
        struct SimNode_Op2AnyConst : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2AnyConst ( const LineInfo & at, SimNode * ll, CTYPE cv ) \
                : SimNode(at), l(ll), c(cv) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "AnyConst", sizeof(CTYPE), #TYPE); \
                V_SUB(l); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE lv = l->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (lv,c,context); \
            } \
            SimNode * l; \
            CTYPE c; \
        }; \
        struct SimNode_Op2ConstAny : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2ConstAny ( const LineInfo & at, SimNode * rr, CTYPE cv ) \
                : SimNode(at), r(rr), c(cv) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstAny", sizeof(CTYPE), #TYPE); \
                V_SUB(r); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE rv = r->eval##TYPE(context); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            SimNode * r; \
            CTYPE c; \
        }; \
        struct SimNode_Op2ConstR2V : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2ConstR2V ( const LineInfo & at, CTYPE cv, uint32_t sp ) \
                : SimNode(at), c(cv), stackTop(sp) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstR2V", sizeof(CTYPE), #TYPE); \
                V_SP(stackTop); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE rv =  *(CTYPE *)(context.stack.sp() + stackTop); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            CTYPE c; \
            uint32_t stackTop; \
        }; \
        struct SimNode_Op2ConstArg : SimNode { \
            DAS_NODE(TYPE,CTYPE); \
            SimNode_Op2ConstArg ( const LineInfo & at, CTYPE cv, int32_t i ) \
                : SimNode(at), c(cv), index(i) {} \
            virtual SimNode * visit ( SimVisitor & vis ) override { \
                V_BEGIN(); \
                vis.op(#OPNAME "ConstArg", sizeof(CTYPE), #TYPE); \
                V_ARG(index); \
                V_ARG(c); \
                V_END(); \
            } \
            __forceinline CTYPE compute ( Context & context ) { \
                CTYPE rv = cast<CTYPE>::to(context.abiArguments()[index]); \
                return SimPolicy<CTYPE>:: OPNAME (c,rv,context); \
            } \
            CTYPE c; \
            int32_t index; \
        }; \
        virtual SimNode * fuse ( SimNode * node, Context * context ) override { \
            auto tnode = static_cast<SimNode_Op2 *>(node); \
            /* OP(*,ConstValue) */ \
            if ( is(tnode->r,"ConstValue") ) { \
                auto cnode = static_cast<SimNode_ConstValue *>(tnode->r); \
                auto cvalue = cast<CTYPE>::to(cnode->value); \
                /* OP(GetLocalR2V,ConstValue) */ \
                if ( is(tnode->l,"GetLocalR2V") ) { \
                    auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2R2VConst>(node->debugInfo, cvalue, r2vnode_l->stackTop); \
                /* OP(ConstValue,GetArgument) */ \
                } else if ( is(tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgConst>(node->debugInfo, cvalue, argnode_l->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyConst>(node->debugInfo, tnode->l, cvalue); \
                } \
            } \
            /* OP(ConstValue,*) */ \
            if ( is(tnode->l,"ConstValue") ) { \
                auto cnode = static_cast<SimNode_ConstValue *>(tnode->l); \
                auto cvalue = cast<CTYPE>::to(cnode->value); \
                /* OP(ConstValue,GetLocalR2V) */ \
                if ( is(tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstR2V>(node->debugInfo, cvalue, r2vnode_r->stackTop); \
                /* OP(ConstValue,GetArgument) */ \
                } else if ( is(tnode->r,"GetArgument") ) { \
                    auto argnode_r = static_cast<SimNode_GetArgument *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2ConstArg>(node->debugInfo, cvalue, argnode_r->index); \
                } else { \
                    return context->code->makeNode<SimNode_Op2ConstAny>(node->debugInfo, tnode->r, cvalue); \
                } \
            /* OP(GetLocalR2V,*) */ \
            } else if ( is(tnode->l,"GetLocalR2V") ) { \
                auto r2vnode_l = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->l); \
                /* OP(GetLocalR2V,GetLocalR2V) */ \
                if ( is(tnode->r,"GetLocalR2V") ) { \
                    auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                    return context->code->makeNode<SimNode_Op2R2VR2V>(node->debugInfo, r2vnode_l->stackTop, r2vnode_r->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2R2VAny>(node->debugInfo, tnode->r, r2vnode_l->stackTop); \
                } \
            /* OP(*,GetLocalR2V) */ \
            } else if ( is(tnode->r, "GetLocalR2V") ) { \
                auto r2vnode_r = static_cast<SimNode_GetLocalR2V<CTYPE> *>(tnode->r); \
                /* OP(GetArgument,GetLocalR2V) */ \
                if ( is(tnode->l,"GetArgument") ) { \
                    auto argnode_l = static_cast<SimNode_GetArgument *>(tnode->l); \
                    return context->code->makeNode<SimNode_Op2ArgR2V>(node->debugInfo, argnode_l->index, r2vnode_r->stackTop); \
                } else { \
                    return context->code->makeNode<SimNode_Op2AnyR2V>(node->debugInfo, tnode->l, r2vnode_r->stackTop); \
                } \
            } \
            return node; \
        } \
    };

#define REGISTER_OP2_FUSION_POINT(OPNAME,TYPE,CTYPE) \
    elements[fuseName(#OPNAME,#TYPE)].push_back(make_shared<Op2FusionPoint_##OPNAME##_##TYPE>(info));

#define IMPLEMENT_OP_INTEGER_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define IMPLEMENT_OP_NUMERIC_FUSION_POINT(OPNAME) \
    IMPLEMENT_OP_INTEGER_FUSION_POINT(OPNAME); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Float,float); \
    IMPLEMENT_OP2_FUSION_POINT(OPNAME,Double,double);

#define REGISTER_OP_INTEGER_FUSION_POINT(OPNAME) \
    REGISTER_OP2_FUSION_POINT(OPNAME,Int,int32_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,UInt,uint32_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Int64,int64_t); \
    REGISTER_OP2_FUSION_POINT(OPNAME,UInt64,uint64_t);

#define REGISTER_OP_NUMERIC_FUSION_POINT(OPNAME) \
    REGISTER_OP_INTEGER_FUSION_POINT(OPNAME); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Float,float); \
    REGISTER_OP2_FUSION_POINT(OPNAME,Double,double);

    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinAnd);
    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinOr);
    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinXor);
    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinShl);
    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinShr);
    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinRotl);
    IMPLEMENT_OP_INTEGER_FUSION_POINT(BinRotr);

    IMPLEMENT_OP_NUMERIC_FUSION_POINT(Add);
    IMPLEMENT_OP_NUMERIC_FUSION_POINT(Sub);
    IMPLEMENT_OP_NUMERIC_FUSION_POINT(Div);
    IMPLEMENT_OP_NUMERIC_FUSION_POINT(Mod);
    IMPLEMENT_OP_NUMERIC_FUSION_POINT(Mul);

    struct SimFusion : SimVisitor {
        SimFusion ( Context * ctx, TextWriter & wr,  map<SimNode *,SimNodeInfo> && ni )
            : context(ctx), ss(wr), info(ni)
        {
            REGISTER_OP_INTEGER_FUSION_POINT(BinAnd);
            REGISTER_OP_INTEGER_FUSION_POINT(BinOr);
            REGISTER_OP_INTEGER_FUSION_POINT(BinXor);
            REGISTER_OP_INTEGER_FUSION_POINT(BinShl);
            REGISTER_OP_INTEGER_FUSION_POINT(BinShr);
            REGISTER_OP_INTEGER_FUSION_POINT(BinRotl);
            REGISTER_OP_INTEGER_FUSION_POINT(BinRotr);

            REGISTER_OP_NUMERIC_FUSION_POINT(Add);
            REGISTER_OP_NUMERIC_FUSION_POINT(Sub);
            REGISTER_OP_NUMERIC_FUSION_POINT(Div);
            REGISTER_OP_NUMERIC_FUSION_POINT(Mod);
            REGISTER_OP_NUMERIC_FUSION_POINT(Mul);
        }
        void fuse() {
            fused = true;
        }
        virtual SimNode * visit ( SimNode * node ) override {
            auto & ni = info[node];
            auto & nv = elements[fuseName(ni.name, ni.typeName)];
            for ( const auto & fe : nv ) {
                auto newNode = fe->fuse(node, context);
                if ( newNode != node ) {
                    fuse();
                    return newNode;
                }
            }
            return SimVisitor::visit(node);
        }
        Context * context = nullptr;
        TextWriter & ss;
        bool fused = false;
        map<SimNode *,SimNodeInfo> & info;
        map<string,vector<FusionPointPtr>> elements;
    };

    void Program::fusion ( Context & context, TextWriter & logs ) {
        // log all functions
        if ( options.getOption("fusion",true) ) {
            bool anyFusion = true;
            while ( anyFusion) {
                anyFusion = false;
                for ( int i=0; i!=context.totalFunctions; ++i ) {
                    SimFunction * fn = context.getFunction(i);
                    SimNodeCollector collector;
                    fn->code->visit(collector);
                    SimFusion fuse(&context, logs, move(collector.info));
                    fn->code = fn->code->visit(fuse);
                    anyFusion |= fuse.fused;
                }
            }
        }
    }
}

