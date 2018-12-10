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
        SimNode_Array(const LineInfo & at, uint32_t s) : SimNode(at), stride(s) {}
        uint32_t stride;
    };
    
    // AT (INDEX)
    struct SimNode_ArrayAt : SimNode_Array {
        SimNode_ArrayAt ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t sz )
            : SimNode_Array(at,sz), value(rv), index(idx) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode * value, * index;
    };
    
    // ERASE
    // [1,2,3,4,5]
    // erase(2)
    //  move(i,i+1,oldSize-i-1)
    struct SimNode_ArrayErase : SimNode_Array {
        SimNode_ArrayErase(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override;
        SimNode * l, * r;
    };
    
    // RESIZE(SIZE)
    struct SimNode_ArrayResize : SimNode_Array {
        SimNode_ArrayResize(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override;
        SimNode * l, * r;
    };
    
    // RESERVE(CAPACITY)
    struct SimNode_ArrayReserve : SimNode_Array {
        SimNode_ArrayReserve(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override;
        SimNode * l, * r;
    };
    
    // PUSH VALUE
    struct SimNode_ArrayPush : SimNode_Array {
        SimNode_ArrayPush(const LineInfo & at, SimNode * ll, SimNode * rr, SimNode * ii, uint32_t sz)
            : SimNode_Array(at, sz), array(ll), value(rr), index(ii) {};
        virtual void copyValue ( char * at, __m128 value ) = 0;
        virtual __m128 eval ( Context & context ) override;
        SimNode * array, *value, *index;
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
}
