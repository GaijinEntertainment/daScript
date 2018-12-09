#pragma once

#include "simulate.h"
#include "arraytype.h"

namespace yzg
{
    __forceinline void array_lock ( Context & context, Array & arr ) {
        arr.lock ++;
        if ( arr.lock==0 ) {
            context.throw_error("array lock overflow");
        }
    }
    
    __forceinline void array_unlock ( Context & context, Array & arr ) {
        if ( arr.lock==0 ) {
            context.throw_error("array lock underflow");
        }
        arr.lock --;
    }
    
    __forceinline void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride ) {
        if ( arr.lock ) {
            context.throw_error("changing capacity of a locked array");
            return;
        }
        if ( arr.capacity >= newCapacity ) return;
        arr.data = (char *) context.reallocate(arr.data, arr.capacity*stride, newCapacity*stride);
        arr.capacity = newCapacity;
    }
    
    __forceinline void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero ) {
        if ( arr.lock ) {
            context.throw_error("resizing locked array");
            return;
        }
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
    
    // ERASE
    // [1,2,3,4,5]
    // erase(2)
    //  move(i,i+1,oldSize-i-1)
    struct SimNode_ArrayErase : SimNode_Array {
        SimNode_ArrayErase(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode_Array(at,sz), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(ll);
            uint32_t idx = cast<uint32_t>::to(rr);
            if ( idx >= pA->size ) {
                context.throw_error("erase index out of range");
                return _mm_setzero_ps();
            }
            memmove ( pA->data+idx*stride, pA->data+(idx+1)*stride, (pA->size-idx-1)*stride );
            array_resize(context, *pA, pA->size-1, stride, false);
            return _mm_setzero_ps();
        }
        SimNode * l, * r;
    };
    
    // RESIZE
    struct SimNode_ArrayResize : SimNode_Array {
        SimNode_ArrayResize(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode_Array(at,sz), l(ll), r(rr) {};
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
        SimNode_ArrayReserve(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode_Array(at,sz), l(ll), r(rr) {};
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
    
    // [1,2,i,4,5]
    // insert(2)
    //  move ( data + 3, data + 2, old_length - 2 );
    
    // PUSH VALUE
    template <typename TT>
    struct SimNode_ArrayPushValue : SimNode_Array {
        SimNode_ArrayPushValue(const LineInfo & at, SimNode * ll, SimNode * rr, SimNode * ii)
            : SimNode_Array(at, sizeof(TT)), array(ll), value(rr), index(ii) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 arr = array->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 val = value->eval(context);
            YZG_EXCEPTION_POINT;
            auto * pA = cast<Array *>::to(arr);
            uint32_t idx = pA->size;
            array_resize(context, *pA, idx + 1, stride, false);
            YZG_EXCEPTION_POINT;
            if ( index ) {
                __m128 ati = index->eval(context);
                YZG_EXCEPTION_POINT;
                uint32_t i = cast<uint32_t>::to(ati);
                if ( i >= pA->size ) {
                    context.throw_error("insert index out of range");
                    return _mm_setzero_ps();
                }
                memmove ( pA->data+(i+1)*stride, pA->data+i*stride, (idx-i)*stride );
                idx = i;
            }
            TT * pl = (TT *) ( pA->data + idx*stride );
            TT * pr = (TT *) &val;
            *pl = *pr;
            return _mm_setzero_ps();
        }
        SimNode * array, *value, *index;
    };
    
    // PUSH REFERENCE VALUE
    struct SimNode_ArrayPushRefValue : SimNode_Array {
        SimNode_ArrayPushRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, SimNode * ii, uint32_t sz)
            : SimNode_Array(at, sz), array(ll), value(rr), index(ii) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 arr = array->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 val = value->eval(context);
            YZG_EXCEPTION_POINT;
            Array * pA = cast<Array *>::to(arr);
            uint32_t idx = pA->size;
            array_resize(context, *pA, idx + 1, stride, false);
            YZG_EXCEPTION_POINT;
            if ( index ) {
                __m128 ati = index->eval(context);
                YZG_EXCEPTION_POINT;
                uint32_t i = cast<uint32_t>::to(ati);
                if ( i >= pA->size ) {
                    context.throw_error("insert index out of range");
                    return _mm_setzero_ps();
                }
                memmove ( pA->data+(i+1)*stride, pA->data+i*stride, (idx-i)*stride );
                idx = i;
            }
            void * pl = pA->data + idx*stride;
            auto pr = cast<void *>::to(val);
            memcpy ( pl, pr, stride );
            return _mm_setzero_ps();
        }
        SimNode * array, *value, *index;
    };
    
    struct GoodArrayIterator : Iterator {
        virtual bool first ( Context & context, IteratorContext & itc ) override {
            __m128 ll = source->eval(context);
            YZG_ITERATOR_EXCEPTION_POINT;
            auto pArray = cast<Array *>::to(ll);
            array_lock(context, *pArray);
            YZG_ITERATOR_EXCEPTION_POINT;
            char * data    = pArray->data;
            itc.value      = cast<char *>::from(data);
            itc.array_end  = data + pArray->size * stride;
            itc.array      = pArray;
            return pArray->size != 0;
        }
        virtual bool next  ( Context & context, IteratorContext & itc ) override {
            char * data = cast<char *>::to(itc.value) + stride;
            itc.value = cast<char *>::from(data);
            return data != itc.array_end;
        }
        virtual void close ( Context & context, IteratorContext & itc ) override {
            if ( itc.array ) {
                array_unlock(context, *itc.array);
            }
        }
        SimNode *   source;
        uint32_t    stride;
    };
}
