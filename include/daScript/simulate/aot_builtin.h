#pragma once

namespace das {
    void builtin_throw ( char * text, Context * context );
    void builtin_print ( char * text, Context * context );
    vec4f builtin_breakpoint ( Context & context, SimNode_CallBase * call, vec4f * );
    void builtin_stackwalk ( Context * context);
    void builtin_terminate ( Context * context );
    int builtin_table_size ( const Table & arr );
    int builtin_table_capacity ( const Table & arr );
    void builtin_table_clear ( Table & arr, Context * context );
    vec4f _builtin_hash ( Context & context, SimNode_CallBase * call, vec4f * args );
    uint32_t heap_bytes_allocated ( Context * context );
    uint32_t heap_high_watermark ( Context * context );
    int32_t heap_depth ( Context * context );
    void builtin_table_lock ( Table & arr, Context * context );
    void builtin_table_unlock ( Table & arr, Context * context );
    int builtin_array_size ( const Array & arr );
    int builtin_array_capacity ( const Array & arr );
    void builtin_array_resize ( Array & pArray, int newSize, int stride, Context * context );
    void builtin_array_reserve ( Array & pArray, int newSize, int stride, Context * context );
    int builtin_array_push ( Array & pArray, int index, int stride, Context * context );
    void builtin_array_erase ( Array & pArray, int index, int stride, Context * context ) ;
    void builtin_array_clear ( Array & pArray, Context * context );

    bool builtin_iterator_first ( Iterator * it, void * data, Context * context );
    bool builtin_iterator_next ( Iterator * it, void * data, Context * context );
    void builtin_iterator_close ( Iterator * it, void * data, Context * context );

    Iterator * builtin_make_iterator ( const Array & arr, int stride, Context * context );
}
