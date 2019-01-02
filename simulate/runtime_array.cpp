#include "precomp.h"

#include "runtime_array.h"

namespace yzg
{
    void array_lock ( Context & context, Array & arr ) {
        arr.lock ++;
        if ( arr.lock==0 ) {
            context.throw_error("array lock overflow");
        }
    }
    
    void array_unlock ( Context & context, Array & arr ) {
        if ( arr.lock==0 ) {
            context.throw_error("array lock underflow");
        }
        arr.lock --;
    }
    
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride ) {
        if ( arr.lock ) {
            context.throw_error("changing capacity of a locked array");
            return;
        }
        if ( arr.capacity >= newCapacity ) return;
        arr.data = (char *) context.reallocate(arr.data, arr.capacity*stride, newCapacity*stride);
        arr.capacity = newCapacity;
    }
    
    void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero ) {
        if ( arr.lock ) {
            context.throw_error("resizing locked array");
            return;
        }
        if ( newSize > arr.capacity ) {
            uint32_t newCapacity = 1 << (32 - __builtin_clz (max(newSize,2u) - 1));
            newCapacity = max(newCapacity, 16u);
            array_reserve(context, arr, newCapacity, stride);
        }
        if ( zero && newSize>arr.size )
            memset ( arr.data + arr.size*stride, 0, (newSize-arr.size)*stride );
        arr.size = newSize;
    }
    
    // SimNode_Array
    
    __m128 SimNode_Array::eval ( Context & context ) {
		Array * pA = (Array *) l->evalPtr(context);
        YZG_EXCEPTION_POINT;
        __m128 rr = r->eval(context);
        YZG_EXCEPTION_POINT;
        uint32_t idx = cast<uint32_t>::to(rr);
        return apply(context, pA, idx);
    }
    
    // PUSH VALUE
    
    __m128 SimNode_ArrayPush::apply ( Context & context, Array * pA, uint32_t index ) {
        return _mm_setzero_ps();
    }
    
    __m128 SimNode_ArrayPush::eval ( Context & context ) {
        __m128 arr = l->eval(context);
        YZG_EXCEPTION_POINT;
        __m128 val = r->eval(context);
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
        copyValue ( pA->data + idx*stride, val );
        return _mm_setzero_ps();
    }
    
    // ERASE

    __m128 SimNode_ArrayErase::apply ( Context & context, Array * pA, uint32_t index ) {
        if ( index >= pA->size ) {
            context.throw_error("erase index out of range");
            return _mm_setzero_ps();
        }
        memmove ( pA->data+index*stride, pA->data+(index+1)*stride, (pA->size-index-1)*stride );
        array_resize(context, *pA, pA->size-1, stride, false);
        return _mm_setzero_ps();
    }
    
    // RESIZE

    __m128 SimNode_ArrayResize::apply ( Context & context, Array * pA, uint32_t newSize ) {
        array_resize(context, *pA, newSize, stride, true);
        return _mm_setzero_ps();
    }
    
    // RESERVE

    __m128 SimNode_ArrayReserve::apply ( Context & context, Array * pA, uint32_t newCapacity ) {
        array_reserve(context, *pA, newCapacity, stride);
        return _mm_setzero_ps();
    }
    
    // GoodArrayIterator
    
    bool GoodArrayIterator::first ( Context & context, IteratorContext & itc )  {
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

    bool GoodArrayIterator::next  ( Context & context, IteratorContext & itc )  {
        char * data = cast<char *>::to(itc.value) + stride;
        itc.value = cast<char *>::from(data);
        return data != itc.array_end;
    }

    void GoodArrayIterator::close ( Context & context, IteratorContext & itc )  {
        if ( itc.array ) {
            array_unlock(context, *itc.array);
        }
    }

    __m128 SimNode_GoodArrayIterator::eval ( Context & context ) {
        return cast<Iterator *>::from(static_cast<GoodArrayIterator *>(this));
    }
    
    // FixedArrayIterator
    
    bool FixedArrayIterator::first ( Context & context, IteratorContext & itc )  {
        __m128 ll = source->eval(context);
        YZG_ITERATOR_EXCEPTION_POINT;
        char * data = cast<char *>::to(ll);
        itc.value = cast<char *>::from(data);
        itc.fixed_array_end = data + size*stride;
        return size != 0;
    }

    bool FixedArrayIterator::next  ( Context & context, IteratorContext & itc )  {
        char * data = cast<char *>::to(itc.value) + stride;
        itc.value = cast<char *>::from(data);
        return data != itc.fixed_array_end;
    }

    void FixedArrayIterator::close ( Context & context, IteratorContext & itc )  {
    }

    __m128 SimNode_FixedArrayIterator::eval ( Context & context ) {
        return cast<Iterator *>::from(static_cast<FixedArrayIterator *>(this));
    }
}
