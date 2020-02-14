#pragma once

namespace das
{
    struct SimNode;
    struct TypeInfo;

    struct Block {
        uint32_t    stackOffset;
        uint32_t    argumentsOffset;
        SimNode *   body;
        vec4f *     functionArguments;
        __forceinline bool operator == ( const Block & b ) const {
            return b.stackOffset==stackOffset && b.argumentsOffset==argumentsOffset
                && b.body==body && b.functionArguments==functionArguments;
        }
    };

    template <typename Result, typename ...Args>
    struct TBlock : Block {
        TBlock() = default;
        TBlock( const TBlock & ) = default;
        TBlock( const Block & that ) { *(Block *)this = that; }
    };

    struct Func {
        int32_t     index;
        Func() = default;
        Func(int32_t idx) : index(idx) {}
        __forceinline bool operator == ( void * ptr ) const {
            return !ptr && (index==0);
        }
        __forceinline bool operator != ( void * ptr ) const {
            return ptr || index;
        }
        __forceinline bool operator == ( const Func & b ) const {
            return index == b.index;
        }
        __forceinline bool operator != ( const Func & b ) const {
            return index != b.index;
        }
    };
    static_assert(sizeof(Func)==sizeof(int32_t), "has to be castable");

    struct Lambda {
        Lambda() = default;
        Lambda(void * ptr) : capture((char *)ptr) {}
        char *      capture;
        __forceinline TypeInfo * getTypeInfo() const {
            return capture ? *(TypeInfo **)(capture-16) : nullptr;
        }
        __forceinline bool operator == ( const Lambda & b ) const {
            return capture == b.capture;
        }
        __forceinline bool operator != ( const Lambda & b ) const {
            return capture != b.capture;
        }
    };
    static_assert(sizeof(Lambda)==sizeof(void *), "has to be castable");

    struct Tuple {
        Tuple() = default;
    };

    struct Array {
        char *      data;
        uint32_t    size;
        uint32_t    capacity;
        uint32_t    lock;
        union {
            struct {
                bool    shared : 1;
            };
            uint32_t    flags;
        };
        __forceinline bool isLocked() const { return lock; }
    };

    class Context;

    void array_lock ( Context & context, Array & arr );
    void array_unlock ( Context & context, Array & arr );
    void array_reserve ( Context & context, Array & arr, uint32_t newCapacity, uint32_t stride );
    void array_resize ( Context & context, Array & arr, uint32_t newSize, uint32_t stride, bool zero );
    void array_clear ( Context & context, Array & arr );

    struct Table : Array {
        char *      keys;
        uint32_t *  hashes;
        uint32_t    maxLookups;
        uint32_t    shift;
    };

    void table_clear ( Context & context, Table & arr );
    void table_lock ( Context & context, Table & arr );
    void table_unlock ( Context & context, Table & arr );

    struct Iterator;

    Iterator * builtin_table_keys ( const Table & tab, int32_t stride, Context * __context__ );
    Iterator * builtin_table_values ( const Table & tab, int32_t stride, Context * __context__ );

    template <typename TT>
    struct EnumStubAny  {
        typedef TT  BaseType;
        TT          value;
    };

    typedef EnumStubAny<int32_t> EnumStub;
    typedef EnumStubAny<int8_t>  EnumStub8;
    typedef EnumStubAny<int16_t> EnumStub16;
    typedef EnumStubAny<int64_t> EnumStub64;
}
