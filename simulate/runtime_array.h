#pragma once

#include "simulate.h"
#include "arraytype.h"

namespace yzg
{
    __forceinline void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride ) {
        if ( arr.capacity >= newCapacity ) return;
        arr.data = (char *) context.reallocate(arr.data, arr.capacity*stride, newCapacity*stride);
        arr.capacity = newCapacity;
    }
    
    __forceinline void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero ) {
        if ( newSize > arr.capacity ) {
            uint32_t newCapacity = 1 << (32 - __builtin_clz (newSize - 1));
            newCapacity = max(newCapacity, 16u);
            array_reserve(context, arr, newCapacity, stride);
        }
        if ( zero && newSize>arr.size )
            memset ( arr.data + arr.size*stride, 0, (newSize-arr.size)*stride );
        arr.size = newSize;
    }
    
    struct SimNode_Array : SimNode {
        SimNode_Array(const LineInfo & at, uint32_t s) : SimNode(at), stride(s) {}
        uint32_t stride;
    };
    
    // AT (INDEX)
    struct SimNode_ArrayAt : SimNode_Array {
        SimNode_ArrayAt ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t sz )
            : SimNode_Array(at,sz), value(rv), index(idx) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = value->eval(context);
            YZG_EXCEPTION_POINT;
            uint32_t idx = cast<uint32_t>::to(index->eval(context));
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            if ( idx >= pA->size ) {
                context.throw_error("index out of range");
                return _mm_setzero_ps();
            } else {
                return cast<char *>::from(pA->data + idx*stride);
            }
        }
        SimNode * value, * index;
    };
    
    // RESIZE
    struct SimNode_ArrayResize : SimNode_Array {
        SimNode_ArrayResize(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            uint32_t newSize = cast<uint32_t>::to(rr);
            array_resize(context, *pA, newSize, stride, true);
            return _mm_setzero_ps();
        }
        SimNode * l, * r;
    };
    
    // RESERVE
    struct SimNode_ArrayReserve : SimNode_Array {
        SimNode_ArrayReserve(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            uint32_t newSize = cast<uint32_t>::to(rr);
            array_reserve(context, *pA, newSize, stride);
            return _mm_setzero_ps();
        }
        SimNode * l, * r;
    };
    
    // PUSH VALUE
    template <typename TT>
    struct SimNode_ArrayPushValue : SimNode_Array {
        SimNode_ArrayPushValue(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode_Array(at, sizeof(TT)), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            auto * pA = cast<Array *>::to(ll);
            uint32_t index = pA->size;
            array_resize(context, *pA, index + 1, stride, false);
            TT * pl = (TT *) ( pA->data + index*stride );
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return _mm_setzero_ps();
        }
        SimNode * l, * r;
    };
    
    // PUSH REFERENCE VALUE
    struct SimNode_ArrayPushRefValue : SimNode_Array {
        SimNode_ArrayPushRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz) : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            uint32_t index = pA->size;
            array_resize(context, *pA, index + 1, stride, false);
            void * pl = pA->data + index*stride;
            auto pr = cast<void *>::to(rr);
            memcpy ( pl, pr, stride );
            return _mm_setzero_ps();
        }
        SimNode * l, * r;
    };
}
