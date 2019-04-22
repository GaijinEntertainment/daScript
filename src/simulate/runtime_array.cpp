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
            uint32_t newCapacity = 1 << (32 - __builtin_clz (das::max(newSize,2u) - 1));
            newCapacity = das::max(newCapacity, 16u);
            array_reserve(context, arr, newCapacity, stride);
        }
        if ( zero && newSize>arr.size )
            memset ( arr.data + arr.size*stride, 0, (newSize-arr.size)*stride );
        arr.size = newSize;
    }

    // GoodArrayIterator

    bool GoodArrayIterator::first ( Context & context, char * _value )  {
        char ** value = (char **) _value;
        array_lock(context, *array);
        char * data    = array->data;
        *value         = data;
        array_end      = data + array->size * stride;
        return (bool) array->size;
    }

    bool GoodArrayIterator::next  ( Context &, char * _value )  {
        char ** value = (char **) _value;
        char * data = *value + stride;
        *value = data;
        return data != array_end;
    }

    void GoodArrayIterator::close ( Context & context, char * _value )  {
        char ** value = (char **) _value;
        *value = nullptr;
        array_unlock(context, *array);
    }

    vec4f SimNode_GoodArrayIterator::eval ( Context & context ) {
        vec4f ll = source->eval(context);
        Array * arr = cast<Array *>::to(ll);
        char * iter = context.heap.allocate(sizeof(GoodArrayIterator));
        new (iter) GoodArrayIterator(arr, stride);
        return cast<char *>::from(iter);
    }

    // FixedArrayIterator

    bool FixedArrayIterator::first ( Context &, char * _value )  {
        char ** value = (char **) _value;
        *value = data;
        fixed_array_end = data + size*stride;
        return (bool) size;
    }

    bool FixedArrayIterator::next  ( Context & , char * _value )  {
        char ** value = (char **) _value;
        char * dataV = *value + stride;
        *value = dataV;
        return dataV != fixed_array_end;
    }

    void FixedArrayIterator::close ( Context &, char * _value )  {
        char ** value = (char **) _value;
        *value = nullptr;
    }

    vec4f SimNode_FixedArrayIterator::eval ( Context & context ) {
        vec4f ll = source->eval(context);
        char * data = cast<char *>::to(ll);
        char * iter = context.heap.allocate(sizeof(GoodArrayIterator));
        new (iter) FixedArrayIterator(data, size, stride);
        return cast<char *>::from(iter);
    }

    // delete

    vec4f SimNode_DeleteArray::eval ( Context & context ) {
        auto pArray = (Array *) subexpr->evalPtr(context);
        pArray = pArray + total - 1;
        for ( uint32_t i=0; i!=total; ++i, pArray-- ) {
            if ( pArray->data ) {
                if ( !pArray->lock ) {
                    uint32_t oldSize = pArray->capacity*stride;
                    context.heap.free(pArray->data, oldSize);
                } else {
                    context.throw_error("deleting locked array");
                    return v_zero();
                }
            }
            memset ( pArray, 0, sizeof(Array) );
        }
        return v_zero();
    }
}
