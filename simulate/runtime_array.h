#pragma once

#include "simulate.h"
#include "arraytype.h"

namespace yzg
{
    void array_lock ( Context & context, Array & arr );
    void array_unlock ( Context & context, Array & arr );
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride );
    void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero );
    
    // BASIC ARRAY NODE
    struct SimNode_Array : SimNode {
        SimNode_Array(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t s) : SimNode(at), l(ll), r(rr), stride(s) {}
        virtual __m128 apply ( Context & context, Array * pA, uint32_t index ) = 0;
        virtual __m128 eval ( Context & context ) override;
        SimNode * l, * r;
        uint32_t stride;
    };
    
    // AT (INDEX)
    struct SimNode_ArrayAt : SimNode_Array {
        YZG_PTR_NODE;
        SimNode_ArrayAt ( const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,ll,rr,sz) {}
        virtual __m128 apply ( Context & context, Array * pA, uint32_t index ) override {
            assert(0 && "we should not even be here");
            return _mm_setzero_ps();
        }
        __forceinline char * compute ( Context & context ) {
            Array * pA = (Array *) l->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_PTR_EXCEPTION_POINT;
            uint32_t idx = cast<uint32_t>::to(rr);
            if ( idx >= pA->size ) {
                context.throw_error("index out of range");
                return nullptr;
            } else {
                return pA->data + idx*stride;
            }
        }
    };
    
    // ERASE(INDEX)
    struct SimNode_ArrayErase : SimNode_Array {
        SimNode_ArrayErase(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,ll,rr,sz) {}
        virtual __m128 apply ( Context & context, Array * pA, uint32_t index ) override;
    };
    
    // RESIZE(SIZE)
    struct SimNode_ArrayResize : SimNode_Array {
        SimNode_ArrayResize(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,ll,rr,sz) {}
        virtual __m128 apply ( Context & context, Array * pA, uint32_t index ) override;
    };
    
    // RESERVE(CAPACITY)
    struct SimNode_ArrayReserve : SimNode_Array {
        SimNode_ArrayReserve(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,ll,rr,sz) {};
        virtual __m128 apply ( Context & context, Array * pA, uint32_t index ) override;
    };
    
    // PUSH VALUE
    struct SimNode_ArrayPush : SimNode_Array {
        SimNode_ArrayPush(const LineInfo & at, SimNode * ll, SimNode * rr, SimNode * ii, uint32_t sz)
            : SimNode_Array(at, ll, rr, sz), index(ii) {};
        virtual void copyValue ( char * at, __m128 value ) = 0;
        virtual __m128 eval ( Context & context ) override;
        virtual __m128 apply ( Context & context, Array * pA, uint32_t index ) override;
        SimNode *index;
    };
    
    // PUSH VALUE
    template <typename TT>
    struct SimNode_ArrayPushValue : SimNode_ArrayPush {
        SimNode_ArrayPushValue(const LineInfo & at, SimNode * ll, SimNode * rr, SimNode * ii)
            : SimNode_ArrayPush(at, ll, rr, ii, sizeof(TT)) {};
        virtual void copyValue ( char * at, __m128 value ) override {
            TT * pl = (TT *) at;
            TT * pr = (TT *) &value;
            *pl = *pr;
        }
    };
    
    // PUSH REFERENCE VALUE
    struct SimNode_ArrayPushRefValue : SimNode_ArrayPush {
        SimNode_ArrayPushRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, SimNode * ii, uint32_t sz)
            : SimNode_ArrayPush(at, ll, rr, ii, sz) {};
        virtual void copyValue ( char * at, __m128 value ) override {
            auto pr = cast<void *>::to(value);
            memcpy ( at, pr, stride );
        }
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
        virtual __m128 eval ( Context & context ) override;
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
        virtual __m128 eval ( Context & context ) override;
    };
    
    template <int total>
    struct SimNode_ForGoodArray : public SimNode_ForBase {
        SimNode_ForGoodArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual __m128 eval ( Context & context ) override {
            Array * pha[total];
            char * ph[total];
            for ( int t=0; t!=total; ++t ) {
                pha[t] = cast<Array *>::to(sources[t]->eval(context));
                YZG_EXCEPTION_POINT;
                array_lock(context, *pha[t]);
                YZG_EXCEPTION_POINT;
                ph[t]  = pha[t]->data;
            }
            char ** pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stackTop + stackTop[t]);
            }
            for (int i = 0; !context.stopFlags; ++i) {
                for (int t = 0; t != total; ++t) {
                    if (i >= int(pha[t]->size)) goto loopOver;
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                body->eval(context);
            }
        loopOver:
            for ( int t=0; t!=total; ++t ) {
                array_unlock(context, *pha[t]);
                YZG_EXCEPTION_POINT;
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
    };
    
    // FOR
    template <int total>
    struct SimNode_ForFixedArray : SimNode_ForBase {
        SimNode_ForFixedArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual __m128 eval ( Context & context ) override {
            char * ph[total];
            for ( int t=0; t!=total; ++t ) {
                ph[t] = cast<char *>::to(sources[t]->eval(context));
                YZG_EXCEPTION_POINT;
            }
            char ** pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stackTop + stackTop[t]);
            }
            for (int i = 0; i != size && !context.stopFlags; ++i) {
                for (int t = 0; t != total; ++t) {
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                body->eval(context);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
    };

}
