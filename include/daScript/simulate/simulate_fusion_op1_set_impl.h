#define IMPLEMENT_SET_OP1_FUSION_POINT(INLINE,OPNAME,TYPE,CTYPE) \
    struct Op1FusionPoint_##OPNAME##_##CTYPE : FusionPoint { \
        Op1FusionPoint_##OPNAME##_##CTYPE ( ) {} \
        IMPLEMENT_ANY_OP1_SET_NODE(INLINE,OPNAME,TYPE,CTYPE,Local); \
        IMPLEMENT_ANY_OP1_SET_NODE(INLINE,OPNAME,TYPE,CTYPE,Argument); \
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * context ) override { \
            SimNode_Op1Fusion * result = nullptr; \
            auto tnode = static_cast<SimNode_Op1 *>(node); \
            if ( false ) {} \
            MATCH_ANY_OP1_NODE(CTYPE,"GetLocal",Local) \
            MATCH_ANY_OP1_NODE(CTYPE,"GetArgument",Argument) \
            MATCH_ANY_OP1_NODE(CTYPE,"GetArgumentRef",Argument) \
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

