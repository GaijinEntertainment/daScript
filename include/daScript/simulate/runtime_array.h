#pragma once

#include <climits>
#include "daScript/simulate/simulate.h"
#include "daScript/misc/arraytype.h"

#include "daScript/simulate/simulate_visit_op.h"

namespace das
{
    void array_lock ( Context & context, Array & arr );
    void array_unlock ( Context & context, Array & arr );
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride );
    void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero );
    void array_clear ( Context & context, Array & arr );

    // AT (INDEX)
    struct SimNode_ArrayAt : SimNode {
        DAS_PTR_NODE;
        SimNode_ArrayAt ( const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), stride(sz) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        __forceinline char * compute ( Context & context ) {
            Array * pA = (Array *) l->evalPtr(context);
            vec4f rr = r->eval(context);
            uint32_t idx = cast<uint32_t>::to(rr);
            if ( idx >= pA->size ) {
                context.throw_error_at(debugInfo,"index out of range");
                return nullptr;
            } else {
                return pA->data + idx*stride;
            }
        }
        SimNode * l, * r;
        uint32_t stride;
    };

    struct GoodArrayIterator : Iterator {
        virtual bool first ( Context & context, IteratorContext & itc ) override;
        virtual bool next  ( Context & context, IteratorContext & itc ) override;
        virtual void close ( Context & context, IteratorContext & itc ) override;
        SimNode *   source;
        uint32_t    stride;
    };

    struct SimNode_GoodArrayIterator : SimNode, GoodArrayIterator {
        SimNode_GoodArrayIterator ( const LineInfo & at, SimNode * s, uint32_t st )
            : SimNode(at) { source = s; stride = st; }
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };

    struct FixedArrayIterator : Iterator {
        virtual bool first ( Context & context, IteratorContext & itc ) override;
        virtual bool next  ( Context & context, IteratorContext & itc ) override;
        virtual void close ( Context & context, IteratorContext & itc ) override;
        SimNode *   source;
        uint32_t    size;
        uint32_t    stride;
    };

    struct SimNode_FixedArrayIterator : SimNode, FixedArrayIterator {
        SimNode_FixedArrayIterator ( const LineInfo & at, SimNode * s, uint32_t sz, uint32_t st )
            : SimNode(at) { source = s; size = sz; stride = st; }
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };

    template <int total>
    struct SimNode_ForGoodArray : public SimNode_ForBase {
        SimNode_ForGoodArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitFor(vis, total, "ForGoodArray");
        }
        virtual vec4f eval ( Context & context ) override {
            Array * __restrict pha[total];
            char * __restrict ph[total];
            for ( int t=0; t!=total; ++t ) {
                pha[t] = cast<Array *>::to(sources[t]->eval(context));
                array_lock(context, *pha[t]);
                ph[t]  = pha[t]->data;
            }
            char ** __restrict pi[total];
            int szz = INT_MAX;
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stack.sp() + stackTop[t]);
                szz = das::min(szz, int(pha[t]->size));
            }
            SimNode * __restrict pbody = body;
            for (int i = 0; i!=szz && !context.stopFlags; ++i) {
                for (int t = 0; t != total; ++t) {
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                pbody->eval(context);
            }
            for ( int t=0; t!=total; ++t ) {
                array_unlock(context, *pha[t]);
            }
            evalFinal(context);
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    template <>
    struct SimNode_ForGoodArray<0> : public SimNode_ForBase {
        SimNode_ForGoodArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN_CR();
            V_OP(ForGoodArray_0);
            V_FINAL();
            V_END();
        }
        virtual vec4f eval ( Context & context ) override {
            evalFinal(context);
            return v_zero();
        }
    };

    template <>
    struct SimNode_ForGoodArray<1> : public SimNode_ForBase {
        SimNode_ForGoodArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitFor(vis, 1, "ForGoodArray");
        }
        virtual vec4f eval ( Context & context ) override {
            Array * __restrict pha;
            char * __restrict ph;
            pha = cast<Array *>::to(sources[0]->eval(context));
            array_lock(context, *pha);
            ph = pha->data;
            char ** __restrict pi;
            int szz = int(pha->size);
            pi = (char **)(context.stack.sp() + stackTop[0]);
            auto stride = strides[0];
            SimNode * __restrict pbody = body;
            for (int i = 0; i!=szz && !context.stopFlags; ++i) {
                *pi = ph;
                ph += stride;
                pbody->eval(context);
            }
            evalFinal(context);
            array_unlock(context, *pha);
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    // FOR
    template <int total>
    struct SimNode_ForFixedArray : SimNode_ForBase {
        SimNode_ForFixedArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitFor(vis, total, "ForFixedArray");
        }
        virtual vec4f eval ( Context & context ) override {
            char * __restrict ph[total];
            for ( int t=0; t!=total; ++t ) {
                ph[t] = cast<char *>::to(sources[t]->eval(context));
            }
            char ** __restrict pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stack.sp() + stackTop[t]);
            }
            SimNode * __restrict pbody = body;
            for (uint32_t i = 0; i != size && !context.stopFlags; ++i) {
                for (int t = 0; t != total; ++t) {
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                pbody->eval(context);
            }
            evalFinal(context);
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    template <>
    struct SimNode_ForFixedArray<0> : SimNode_ForBase {
        SimNode_ForFixedArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN_CR();
            V_OP(ForFixedArray_0);
            V_FINAL();
            V_END();
        }
        virtual vec4f eval ( Context & context ) override {
            evalFinal(context);
            return v_zero();
        }
    };

    template <>
    struct SimNode_ForFixedArray<1> : SimNode_ForBase {
        SimNode_ForFixedArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitFor(vis, 1, "ForFixedArray");
        }
        virtual vec4f eval ( Context & context ) override {
            char * __restrict ph = cast<char *>::to(sources[0]->eval(context));
            char ** __restrict pi = (char **)(context.stack.sp() + stackTop[0]);
            auto stride = strides[0];
            SimNode * __restrict pbody = body;
            for (uint32_t i = 0; i != size && !context.stopFlags; ++i) {
                *pi = ph;
                ph += stride;
                pbody->eval(context);
            }
            evalFinal(context);
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    struct SimNode_DeleteArray : SimNode_Delete {
        SimNode_DeleteArray ( const LineInfo & a, SimNode * s, uint32_t t, uint32_t st )
            : SimNode_Delete(a,s,t), stride(st) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
        uint32_t stride;
    };
}

#include "daScript/simulate/simulate_visit_op_undef.h"
