#include "precomp.h"

#include "runtime_array.h"

namespace yzg
{
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity )
    {
        if ( arr.capacity >= newCapacity ) return;
        arr.data = (char *) context.reallocate(arr.data, arr.capacity*arr.fieldSize, newCapacity*arr.fieldSize);
        arr.capacity = newCapacity;
    }
    
    void array_resize ( Context & context, Array & arr, uint32_t newSize )
    {
        if ( newSize > arr.capacity ) {
            uint32_t newCapacity = 1 << (32 - __builtin_clz (newSize - 1));
            newCapacity = max(newCapacity, 16u);
            array_reserve(context, arr, newCapacity);
        }
        if ( newSize > arr.size )
            memset ( arr.data + arr.size*arr.fieldSize, 0, (newSize-arr.size)*arr.fieldSize );
        arr.size = newSize;
    }
}
