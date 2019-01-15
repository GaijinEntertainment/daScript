#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/arraytype.h"

namespace das
{
    void array_lock ( Context & context, Array & arr );
    void array_unlock ( Context & context, Array & arr );
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride );
    void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero );
    
    // AT (INDEX)
    struct SimNode_ArrayAt : SimNode {
        DAS_PTR_NODE;
        SimNode_ArrayAt ( const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), stride(sz) {}
        __forceinline char * compute ( Context & context ) {
            Array * pA = (Array *) l->evalPtr(context);
            DAS_PTR_EXCEPTION_POINT;
            vec4f rr = r->eval(context);
            DAS_PTR_EXCEPTION_POINT;
            uint32_t idx = cast<uint32_t>::to(rr);
            if ( idx >= pA->size ) {
                context.throw_error("index out of range");
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
        virtual vec4f eval ( Context & context ) override;
    };
    
    template <int total>
    struct SimNode_ForGoodArray : public SimNode_ForBase {
        SimNode_ForGoodArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            Array * __restrict pha[total];
            char * __restrict ph[total];
            for ( int t=0; t!=total; ++t ) {
                pha[t] = cast<Array *>::to(sources[t]->eval(context));
                DAS_EXCEPTION_POINT;
                array_lock(context, *pha[t]);
                DAS_EXCEPTION_POINT;
                ph[t]  = pha[t]->data;
            }
            char ** __restrict pi[total];
            int szz = INT_MAX;
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stack.sp() + stackTop[t]);
                szz = min(szz, int(pha[t]->size));
            }
            for (int i = 0; i!=szz && !context.stopFlags; ++i) {
                for (int t = 0; t != total; ++t) {
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                body->eval(context);
            }
            for ( int t=0; t!=total; ++t ) {
                array_unlock(context, *pha[t]);
                DAS_EXCEPTION_POINT;
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };
    
    // FOR
    template <int total>
    struct SimNode_ForFixedArray : SimNode_ForBase {
        SimNode_ForFixedArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            char * __restrict ph[total];
            for ( int t=0; t!=total; ++t ) {
                ph[t] = cast<char *>::to(sources[t]->eval(context));
                DAS_EXCEPTION_POINT;
            }
            char ** __restrict pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stack.sp() + stackTop[t]);
            }
            for (uint32_t i = 0; i != size && !context.stopFlags; ++i) {
                for (int t = 0; t != total; ++t) {
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                body->eval(context);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

}
