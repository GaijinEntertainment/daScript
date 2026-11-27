#pragma once

namespace das {

    // src/simulate/ARCHITECTURE.md#if-over-compare
    template <typename Cmp>
    struct SimNode_IfCmp : Cmp {
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return this->visitIf(vis, if_true, if_false);
        }
        SimNode * if_true = nullptr;
        SimNode * if_false = nullptr;
    };

    template <typename Cmp>
    struct SimNode_IfCmpThenElse : SimNode_IfCmp<Cmp> {
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override {
            DAS_PROFILE_NODE
            if ( this->compute(context) ) {
                DAS_NO_MERGE return this->if_true->eval(context);
            } else {
                DAS_NO_MERGE return this->if_false->eval(context);
            }
        }
#define EVAL_NODE(TYPE,CTYPE) \
        virtual CTYPE eval##TYPE ( Context & context ) override { \
            DAS_PROFILE_NODE \
            if ( this->compute(context) ) { \
                DAS_NO_MERGE return this->if_true->eval##TYPE(context); \
            } else { \
                DAS_NO_MERGE return this->if_false->eval##TYPE(context); \
            } \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    template <typename Cmp>
    struct SimNode_IfCmpThen : SimNode_IfCmp<Cmp> {
        virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override {
            DAS_PROFILE_NODE
            if ( this->compute(context) ) {
                return this->if_true->eval(context);
            } else {
                return v_zero();
            }
        }
        virtual bool evalBool ( Context & context ) override {
            return cast<bool>::to(eval(context));
        }
    };

    template <typename Cmp>
    SimNode * makeIfCmp ( Context * context, Cmp * cmp, SimNode_IfTheElseAny * ifNode ) {
        SimNode_IfCmp<Cmp> * result;
        if ( ifNode->if_false ) {
            result = context->code->makeNode<SimNode_IfCmpThenElse<Cmp>>();
        } else {
            result = context->code->makeNode<SimNode_IfCmpThen<Cmp>>();
        }
        result->set(cmp->op, cmp->baseType, cmp->debugInfo);
        result->errorMessage = cmp->errorMessage;
        result->l = cmp->l;
        result->r = cmp->r;
        result->if_true = ifNode->if_true;
        result->if_false = ifNode->if_false;
        return result;
    }
}
