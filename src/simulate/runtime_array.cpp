#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_array.h"

namespace das
{
    void array_clear ( Context & context, Array & arr ) {
        if ( arr.lock ) {
            context.throw_error("clearing locked array");
            return;
        }
        arr.size = 0;
    }

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

    void array_reserve(Context & context, Array & arr, uint32_t newCapacity, uint32_t stride) {
        if (arr.lock) {
            context.throw_error("changing capacity of a locked array");
            return;
        }
        if (arr.capacity >= newCapacity) {
            return;
        }
        auto newData = (char *)context.heap.reallocate(arr.data, arr.capacity*stride, newCapacity*stride);
        if (!newData) {
            context.throw_error("out of linear allocator memory");
            return;
        }
        if (newData != arr.data) {
            memcpy(newData, arr.data, arr.capacity);
            arr.data = newData;
        }
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

    // GoodArrayIterator

    bool GoodArrayIterator::first ( Context & context, IteratorContext & itc )  {
        vec4f ll = source->eval(context);
        auto pArray = cast<Array *>::to(ll);
        array_lock(context, *pArray);
        char * data    = pArray->data;
        itc.value      = cast<char *>::from(data);
        itc.array_end  = data + pArray->size * stride;
        itc.array      = pArray;
        return (bool) pArray->size;
    }

    bool GoodArrayIterator::next  ( Context &, IteratorContext & itc )  {
        char * data = cast<char *>::to(itc.value) + stride;
        itc.value = cast<char *>::from(data);
        return data != itc.array_end;
    }

    void GoodArrayIterator::close ( Context & context, IteratorContext & itc )  {
        if ( itc.array ) {
            array_unlock(context, *itc.array);
        }
    }

    vec4f SimNode_GoodArrayIterator::eval ( Context & ) {
        return cast<Iterator *>::from(static_cast<GoodArrayIterator *>(this));
    }

    // FixedArrayIterator

    bool FixedArrayIterator::first ( Context & context, IteratorContext & itc )  {
        vec4f ll = source->eval(context);
        char * data = cast<char *>::to(ll);
        itc.value = cast<char *>::from(data);
        itc.fixed_array_end = data + size*stride;
        return (bool) size;
    }

    bool FixedArrayIterator::next  ( Context & , IteratorContext & itc )  {
        char * data = cast<char *>::to(itc.value) + stride;
        itc.value = cast<char *>::from(data);
        return data != itc.fixed_array_end;
    }

    void FixedArrayIterator::close ( Context &, IteratorContext &  )  {
    }

    vec4f SimNode_FixedArrayIterator::eval ( Context & ) {
        return cast<Iterator *>::from(static_cast<FixedArrayIterator *>(this));
    }
}
